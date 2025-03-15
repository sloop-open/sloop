/**
 ******************************************************************************
 * @file    console
 * @author  xuan
 * @date    2025-1-21
 * @brief   系统控制台
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "common.h"

#if !SYS_RTT_ENABLE

#undef BHV_LOG_ENABLE
#undef SYS_CMD_ENABLE

#define BHV_LOG_ENABLE 0
#define SYS_CMD_ENABLE 0

#endif

#if SYS_CMD_ENABLE

/* cpu 打印 */
void cpu_print(void);
/* 打印运行中的任务 */
void task_print(void);
/* 系统复位 */
void cmd_reboot(void);
/* 查询版本 */
void cmd_version(void);
/* 查询当前是哪个互斥任务 */
void cmd_task(void);
/* 查询 cpu 负载 */
void cmd_cpu(void);
/* 串口发送 */
void cmd_uart(void);
/* can 发送 */
void cmd_can(void);
/* gpio 输出 */
void cmd_gpio(void);

/* 控制台命令字符上限 */
#define BUFFER_SIZE SYS_CMD_SIZE
/* 命令头字符上限 */
#define HEAD_SIZE 16

/* 控制台接收字符缓存 */
static char str_buff[BUFFER_SIZE];

/* 系统命令结构体 */
typedef struct
{
    char str[HEAD_SIZE + 1];

    pfunc callback;

} sys_cmd_typ;

/* 系统命令注册表 */
sys_cmd_typ sys_cmd_reg[SYS_CMD_LIMIT] = {

    {"reboot", cmd_reboot},
    {"version", cmd_version},
    {"task", cmd_task},
    {"cpu", cmd_cpu},
    {"uart", cmd_uart},
    {"can", cmd_can},
    {"gpio", cmd_gpio},

};

/* 实际命令数量 */
static int cmd_num = sizeof sys_cmd_reg / sizeof sys_cmd_reg[0];

/* 添加控制台命令 */
void sys_add_cmd(char *str, pfunc task)
{
    if (task == NULL)
    {
        sys_error("The task is null");

        return;
    }

    if (strlen(str) > HEAD_SIZE)
    {
        sys_error("The cmd is too long, more than %2d", HEAD_SIZE);
        sys_error("The cmd: %s", str);
    }

    for (int i = 0; i < SYS_CMD_LIMIT; i++)
    {
        if (str[0] == '\0')
        {
            sys_error("The command string cannot be empty");

            return;
        }

        if (sys_cmd_reg[i].callback == task)
        {
            sys_error("this command has already been registered");

            return;
        }
    }

    /* 如未注册，在这里注册 */
    for (int i = 0; i < SYS_CMD_LIMIT; i++)
    {
        if (sys_cmd_reg[i].callback == NULL)
        {
            strncpy(sys_cmd_reg[i].str, str, HEAD_SIZE);

            sys_cmd_reg[i].callback = task;

            cmd_num++;

            sys_printf("system cmd added: %s", str);

            return;
        }
    }

    sys_error("system cmd overflow, limit %2d", SYS_CMD_LIMIT);
}

/* 获取控制台输入字符串。可用于获取命令参数，推荐用 sscanf */
char *sys_get_cmd_str(void)
{
    return str_buff;
}

/* 系统控制台 */
void system_console(void)
{
    static char RTT_buff[BUFFER_SIZE + 1];

    char r = SEGGER_RTT_HasKey();

    if (r == 0)
        return;

    SEGGER_RTT_Read(0u, RTT_buff, BUFFER_SIZE);

    strncpy(str_buff, RTT_buff, BUFFER_SIZE);

    if (RTT_buff[BUFFER_SIZE - 2] != 0)
    {
        sys_error("Input more than %d characters", BUFFER_SIZE);
    }

    /* 清除缓存 */
    memset(RTT_buff, 0, BUFFER_SIZE);

    char buff[BUFFER_SIZE + 1] = {0};

    strncpy(buff, str_buff, BUFFER_SIZE);

    /* 如果有空格，空格前的是命令，提取出来 */
    strtok(buff, " ");

    /* 解析命令 */
    for (int i = 0; i < cmd_num; i++)
    {
        if (sys_cmd_reg[i].str[0] == '\0')
            continue;

        /* 匹配并且出现在字符串头部 */
        if (strncmp(buff, sys_cmd_reg[i].str, strlen(sys_cmd_reg[i].str)) == 0)
        {
            sys_printf("cmd%d <%s> will be executed", i, sys_cmd_reg[i].str);

            if (sys_cmd_reg[i].callback != NULL)
                sys_cmd_reg[i].callback();

            return;
        }
    }

    /* 多个字符，不匹配，直接返回 */
    if (strlen(buff) != 1)
    {
        /* 无效命令 */
        sys_error("Invalid command");

        return;
    }

    char index = buff[0];

    /* 内置简写命令 */
    switch (index)
    {

    /* 显示命令菜单 */
    case 'm':
    case 'M':
    {
        sys_focus("cmd menu");

        for (int i = 0; i < SYS_CMD_LIMIT; i++)
        {
            if (sys_cmd_reg[i].callback != NULL)
            {
                sys_prt_brYellow("%d: %s", i, sys_cmd_reg[i].str);
            }
            else
            {
                break;
            }
        }
    }
        return;

    case 'r':
    case 'R':
    {
        cmd_reboot();
    }
        return;

    case 'v':
    case 'V':
    {
        cmd_version();
    }
        return;

    case 't':
    case 'T':
    {
        cmd_task();
    }
        return;

    case 'c':
    case 'C':
    {
        cmd_cpu();
    }
        return;

    default:
    {
        if (index < '0')
        {
            sys_error("Invalid command");

            return;
        }

        index -= '0';

        if (index < SYS_CMD_LIMIT)
        {
            if (sys_cmd_reg[index].callback != NULL)
            {
                sys_printf("cmd%d <%s> will be executed", index, sys_cmd_reg[index].str);

                sys_cmd_reg[index].callback();

                return;
            }
        }
    }
    break;
    }

    sys_error("Invalid command");
}

/* ============================================================== */

/* 系统复位 */
void cmd_reboot(void)
{
    sys_timeout_start(10, NVIC_SystemReset);
}

/* 版本号长度 */
#define VER_LEN 32

static char version[VER_LEN + 1];

/* 查询版本 */
void cmd_version(void)
{
    sys_prt_brYellow("Version: %s, Build Date: %s, %s", version, __DATE__, __TIME__);
}

/* 设置程序版本 */
void sys_set_version(char *str)
{
    if (strlen(str) > VER_LEN)
    {
        sys_error("The version is too long, more than %2d", VER_LEN);
        sys_error("The version: %s", str);
    }

    strncpy(version, str, VER_LEN);

    sys_prt_brYellow("version: %s", str);
}

/* 查询程序版本 */
char *sys_get_version(void)
{
    return version;
}

/* ============================================================== */

/* 查询当前是哪个互斥任务 */
void cmd_task(void)
{
#if BHV_LOG_ENABLE

    char *str = sys_get_cmd_str();

    str = str_next(str);

    if (str == NULL)
    {
        task_print();

        return;
    }

    if (strcmp(str, "on") == 0)
    {
        sys_cycle_start(100, task_print);
    }
    else if (strcmp(str, "off") == 0)
    {
        sys_cycle_stop(task_print);
    }
    else if (strcmp(str, "-h") == 0)
    {
        sys_prt_brYellow("e.g. \"task\", \"task on\" or \"task off\"");
    }

#else

    sys_error("Enable behavior log to activate task commands");

#endif
}

/* ============================================================== */

/* 查询 cpu 负载 */
void cmd_cpu(void)
{
    char *str = sys_get_cmd_str();

    str = str_next(str);

    if (str == NULL)
    {
        cpu_print();

        return;
    }

    if (strcmp(str, "on") == 0)
    {
        sys_cycle_start(100, cpu_print);
    }
    else if (strcmp(str, "off") == 0)
    {
        sys_cycle_stop(cpu_print);
    }
    else if (strcmp(str, "-h") == 0)
    {
        sys_prt_brYellow("e.g. \"cpu\", \"cpu on\" or \"cpu off\"");
    }
}

/* ============================================================== */

/* 最大的 hex 数量 */
#define MAX_HEX 16

/* 命令示例：
ASCLL 模式： uart6 hello
HEX 模式： uart6 -hex 68 65 6C 6C 6F */
void cmd_uart(void)
{
    char *str = sys_get_cmd_str();

    int id = 0;

    char *_str = str;

    str = str_next(str);

    if (str != NULL)
    {
        if (strcmp(str, "-h") == 0)
        {
            sys_prt_brYellow("e.g. \"uart6 hello\" or \"uart6 -hex 68 65 6C 6C 6F\"");

            return;
        }
    }

    /* 获取 串口号 */
    if (sscanf(strtok(_str, " "), "uart%d", &id) != 1)
    {
        sys_error("Please specify the valid serial port");

        sys_error("e.g. \"uart6 hello\" or \"uart6 -hex 68 65 6C 6C 6F\"");

        return;
    }

    if (str == NULL)
    {
        sys_error("The content sent cannot be empty");

        return;
    }

    /* ASCLL 码，直接发送 */
    if (strncmp(str, "-hex", strlen("-hex")) != 0)
    {
        switch (id)
        {
        case 1:
            asp_uart1_send((void *)str, strlen(str));
            break;

        case 2:
            asp_uart2_send((void *)str, strlen(str));
            break;

        case 3:
            asp_uart3_send((void *)str, strlen(str));
            break;

        case 4:
            asp_uart4_send((void *)str, strlen(str));
            break;

        case 5:
            asp_uart5_send((void *)str, strlen(str));
            break;

        case 6:
            asp_uart6_send((void *)str, strlen(str));
            break;

        default:
            sys_error("serial port %d has not been registered", id);
            break;
        }

        return;
    }

    char hex[MAX_HEX] = {0};

    str = str_next(str);

    if (str == NULL)
    {
        sys_error("The content sent cannot be empty");

        return;
    }

    int count = 0;

    while (str != NULL)
    {
        if (sscanf(str, "%x", (uint32_t *)(hex + count)) == 1)
        {
            count++;
        }

        str = str_next(str);
    }

    switch (id)
    {
    case 1:
        asp_uart1_send((void *)hex, count);
        break;

    case 2:
        asp_uart2_send((void *)hex, count);
        break;

    case 3:
        asp_uart3_send((void *)hex, count);
        break;

    case 4:
        asp_uart4_send((void *)hex, count);
        break;

    case 5:
        asp_uart5_send((void *)hex, count);
        break;

    case 6:
        asp_uart6_send((void *)hex, count);
        break;

    default:
        sys_error("serial port %d has not been registered", id);
        break;
    }
}

/* ============================================================== */

/* 命令示例：
ASCLL 模式： can1 -id 1 hello
HEX 模式： can1 -id 1 -hex 68 65 6C 6C 6F */
void cmd_can(void)
{
    CanTxMsg msg = {0};

    char *str = sys_get_cmd_str();

    int _id = 0;
    int id = 0;

    char *_str = str;

    str = str_next(str);

    if (str != NULL)
    {
        if (strcmp(str, "-h") == 0)
        {
            sys_prt_brYellow("e.g. \"can1 -id 1 hello\" or \"can1 -id 1 -hex 68 65 6C 6C 6F\"");

            return;
        }
    }

    if (sscanf(strtok(_str, " "), "can%d", &_id) != 1)
    {
        sys_error("Please specify can1 or can2.");

        sys_error("e.g. \"can1 -id 1 hello\" or \"can1 -id 1 -hex 68 65 6C 6C 6F\"");

        return;
    }

    /* can1 或 can2 */
    if (!((_id == 1) || (_id == 2)))
    {
        sys_error("Please specify can1 or can2.");

        sys_error("e.g. \"can1 -id 1 hello\" or \"can1 -id 1 -hex 68 65 6C 6C 6F\"");

        return;
    }

    /* 获取 can id */
    if (sscanf(str, "-id %d", &id) != 1)
    {
        sys_error("Please specify a valid id.");

        sys_error("e.g. \"can1 -id 1 hello\" or \"can1 -id 1 -hex 68 65 6C 6C 6F\"");

        return;
    }

    str = str_next(str);
    str = str_next(str);

    if (str == NULL)
    {
        sys_error("The content sent cannot be empty");

        return;
    }

    /* ASCLL 码，直接发送 */
    if (strncmp(str, "-hex", strlen("-hex")) != 0)
    {
        char len = strlen(str);

        if (len > 8)
        {
            sys_error("can tx len over 8, reach %2d", len);

            len = 8;
        }
        else if (len == 0)
        {
            sys_error("Send length is 0");

            return;
        }

        msg.StdId = id;
        msg.ExtId = 0x00;
        msg.RTR = CAN_RTR_DATA;
        msg.IDE = CAN_ID_STD;
        msg.DLC = len;
        strncpy((void *)msg.Data, str, len);

        if (_id == 1)
            asp_can1_send(&msg);
        else
            asp_can2_send(&msg);

        return;
    }

    char hex[MAX_HEX] = {0};

    str = str_next(str);

    if (str == NULL)
    {
        sys_error("The content sent cannot be empty");

        return;
    }

    int count = 0;

    while (str != NULL)
    {
        if (sscanf(str, "%x", (uint32_t *)(hex + count)) == 1)
        {
            count++;
        }

        str = str_next(str);
    }

    char len = count;

    if (len > 8)
    {
        sys_error("can tx len over 8, reach %2d", len);

        len = 8;
    }
    else if (len == 0)
    {
        sys_error("Send length is 0");

        return;
    }

    msg.StdId = id;
    msg.ExtId = 0x00;
    msg.RTR = CAN_RTR_DATA;
    msg.IDE = CAN_ID_STD;
    msg.DLC = len;
    memcpy((void *)msg.Data, hex, len);

    if (_id == 1)
        asp_can1_send(&msg);
    else
        asp_can2_send(&msg);
}

#else /* SYS_CMD_ENABLE */

/* 弱定义，防止 RTT 关闭时，编译报错 */

void sys_add_cmd(char *str, pfunc task) {}

char *sys_get_cmd_str(void)
{
    return NULL;
}

void sys_set_version(char *str) {}

char *sys_get_version(void)
{
    return NULL;
}

#endif /* SYS_CMD_ENABLE */

/* 按空格索引下一个字符串 */
char *str_next(char *str)
{
    char *r = strchr(str, ' ');

    if (r == NULL)
    {
        return NULL;
    }

    if (r[1] == 0)
    {
        return NULL;
    }

    return r + 1;
}

/* ============================================================== */

/* gpio 输出 */
void cmd_gpio(void)
{
    char *str = sys_get_cmd_str();

    str = str_next(str);

    if (str == NULL)
    {
        sys_error("e.g. \"gpio pin_beep H\", \"gpio input on\" or \"gpio input off\"");

        return;
    }

    if (strcmp(str, "-h") == 0)
    {
        sys_prt_brYellow("e.g. \"gpio pin_beep H\", \"gpio input on\" or \"gpio input off\"");

        return;
    }
    else if (strcmp(str, "input on") == 0)
    {
        /* 开启 GPIO 输入回显 */
        sys_cycle_start(10, gpio_input_echo);

        sys_prt_withFunc("gpio input echo on");
    }
    else if (strcmp(str, "input off") == 0)
    {
        /* 关闭 GPIO 输入回显 */
        sys_cycle_stop(gpio_input_echo);

        sys_prt_withFunc("gpio input echo off");
    }

    /* 解析命令 */
    for (int i = 0; i < PIN_NUM; i++)
    {
        /* 匹配并且出现在字符串头部 */
        if (strncmp(str, gpio_map[i].str, strlen(gpio_map[i].str)) == 0)
        {
            str = str_next(str);

            gpio_write(gpio_map[i]._enum, str[0] == 'H' ? H : L);

            sys_printf("pin_beep");

            return;
        }
    }
}

/************************** END OF FILE **************************/
