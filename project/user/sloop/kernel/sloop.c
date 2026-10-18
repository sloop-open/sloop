/**
 ******************************************************************************
 * @file    sloop
 * @author  sloop
 * @date    2024-12-16
 * @brief   提供 sloop 系统API,比如超时/周期任务的创建、并发任务创建、互斥任务切换等
 *
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "sloop.h"

#define sl_check_task_not_null()      \
    if (task == NULL)                 \
    {                                 \
        sl_error("The task is null"); \
        return;                       \
    }

#define sl_backup_reg(name1, name2)                    \
    static name1##_typ backup_reg[SL_##name2##_LIMIT]; \
    memcpy(backup_reg, name1##_reg, sizeof backup_reg);

void print_null(const char *sFormat, ...) {}

/* 超时任务运行 */
void timeout_run(void);
/* 周期任务运行 */
void cycle_run(void);
/* 多次任务运行 */
void multiple_run(void);
/* 并发任务运行 */
void concurrent_task_run(void);
/* 单次任务运行 */
void once_task_run(void);
/* 软件定定时器 */
void soft_timer(void);

/* ============================================================== */

/* 系统心跳 */
void system_heartbeat(void);
/* 计算 cpu 负载 */
void calcul_cpu_load(void);
/* 栈水位监测 */
void stack_monitor(void);

static volatile uint32_t tick;

static int loop;

static int load;

static int loop_us;

/* ============================================================== */

/* sloop 系统初始化 */
void sloop_init(void)
{
    sl_prt_brYellow("==================================");
    sl_prt_brYellow("========= sloop  (^-^) ==========");
    sl_prt_brYellow("==================================");

    /* 启动栈水位监测 */
    sl_task_start(stack_monitor);

    /* 启用单次任务 */
    sl_task_start(once_task_run);

    /* 启动 cpu 负载计算 */
    sl_task_start(calcul_cpu_load);

    /* 启用系统心跳 */
    sl_cycle_start(1000, system_heartbeat);

    sl_printf("system heartbeat start");
}

/* sloop 系统运行 */
void sloop_run(void)
{
    /* 并发任务运行 */
    concurrent_task_run();
}

/* ============================================================== */

/* mcu tick 中断 */
void sl_tick_irq(void)
{
    tick++;

    /* 软件定时器 1ms 启动一次 */
    sl_task_once(soft_timer);
}

/* 软件定定时器 */
void soft_timer(void)
{
    /* 超时任务运行 */
    timeout_run();

    /* 周期任务运行 */
    cycle_run();

    /* 多次任务运行 */
    multiple_run();
}

/* ============================================================== */

/* 系统心跳 */
void system_heartbeat(void)
{
    static int count;

    SEGGER_RTT_SetTerminal(1);

    sl_prt_var(count);

    SEGGER_RTT_SetTerminal(0);

    count++;
}

/* ============================================================== */

void load_calcu(void);
void load_warning(void);
void judge_warning(void);

/* 计算 cpu 负载 */
void calcul_cpu_load(void)
{
    loop++;

    load_calcu();

    judge_warning();
}

/* CPU 负载基数，即 CPU = 100% 时，一个 loop 的时间 单位0.1us，约定当loop 达到100us时，CPU 占用为100% */

/* 计算周期 ms */
#define LOOP_CYCLE (100)
/* 负载基数 us */
#define LOOP_BASE (100)

void load_calcu(void)
{
    SL_BEGIN;

    for (;;)
    {
        sl_wait_ms(LOOP_CYCLE);

        loop_us = LOOP_CYCLE * 1000 / loop;

        load = loop_us * 100 / LOOP_BASE;

        loop = 0;
    }

    SL_END;
}

/* 负载警告 */
void load_warning(void)
{
    SL_BEGIN;

    for (;;)
    {
        sl_error("cpu load over 80%%, reach %2d, average loop time: %d us", load, loop_us);

        sl_wait_ms(1000);
    }

    SL_END;
}

void judge_warning(void)
{
    SL_BEGIN;

    for (;;)
    {
        sl_wait_until(load > 800);

        sl_task_start(load_warning);

        sl_wait_until(load < 600);

        sl_task_stop(load_warning);
    }

    SL_END;
}

/* ============================================================== */

extern uint8_t _estack;
extern uint32_t _Min_Stack_Size;
static uint32_t stack_max_usage;
static uint32_t stack_top;
static uint32_t stack_size;
static uint32_t stack_bottom;
static uint32_t fill_size;
static uint8_t *p;
static uint32_t used;

/* 栈水位染色值 */
#define STACK_SENTINEL 0xCC
/* 栈水位染色区域 */
#define STACK_FILL_PERCENT 50

void stack_monitor(void)
{
    SL_BEGIN;

    /* 填充栈顶作为染色区域 */
    stack_top = (uint32_t)&_estack;
    stack_size = (uint32_t)&_Min_Stack_Size;
    fill_size = stack_size * STACK_FILL_PERCENT / 100;
    stack_bottom = stack_top - stack_size;
    p = (uint8_t *)stack_bottom;
    while (fill_size--)
    {
        *p++ = STACK_SENTINEL;
    }

    for (;;)
    {
        fill_size = stack_size * STACK_FILL_PERCENT / 100;
        p = (uint8_t *)stack_bottom;

        while (fill_size--)
        {
            if (*p != STACK_SENTINEL)
            {
                used = stack_top - (uint32_t)p;
                stack_max_usage = used * 100 / stack_size;
                break;
            }
            p++;
        }

        if (stack_max_usage > 80)
        {
            sl_error("stack max usage over 80%%, reach %2d, used: %d bytes, total: %d bytes",
                     stack_max_usage,
                     used, stack_size);
        }

        sl_wait_ms(1000);
    }

    SL_END;
}

/* ============================================================== */

/* 获取时间戳 */
uint32_t sl_get_tick(void)
{
    return tick;
}

/* 阻塞式延时 */
void sl_delay_ms(int ms)
{
    uint32_t tick_start = tick;

    /* 传入1ms，实际延时至少1ms */
    ms == 1 ? ms++ : ms;

    while (1)
    {
        if ((uint32_t)(tick - tick_start) >= ms)
        {
            return;
        }
    }
}

/* ============================================================== */

/* 超时任务数据 */
typedef struct
{
    uint32_t tick_start;

    int delay_ms;

    pfunc callback;

} timeout_typ;

/* 超时任务注册表 */
static timeout_typ timeout_reg[SL_TIMEOUT_LIMIT];

/* 超时任务运行 */
void timeout_run(void)
{
    uint32_t tick_start;
    int delay_ms;

    /* 防止回调中改写注册表 */
    sl_backup_reg(timeout, TIMEOUT);

    for (int i = 0; i < SL_TIMEOUT_LIMIT; i++)
    {
        if (backup_reg[i].callback == NULL)
            continue;

        tick_start = backup_reg[i].tick_start;

        delay_ms = backup_reg[i].delay_ms;

        if ((uint32_t)(tick - tick_start) >= delay_ms)
        {
            /* 超时任务完成，释放资源 */
            timeout_reg[i].callback = NULL;

            backup_reg[i].callback();
        }
    }
}

/* 超时任务 */
void sl_timeout_start(int ms, pfunc task)
{
    sl_check_task_not_null();

    /* 传入1ms，实际延时至少1ms */
    ms == 1 ? ms++ : ms;

    for (int i = 0; i < SL_TIMEOUT_LIMIT; i++)
    {
        if (timeout_reg[i].callback == task)
        {
            /* 已注册，更新时间戳 */
            timeout_reg[i].tick_start = tick;

            return;
        }
    }

    /* 如未注册，这里先注册 */
    for (int i = 0; i < SL_TIMEOUT_LIMIT; i++)
    {
        if (timeout_reg[i].callback == NULL)
        {
            timeout_reg[i].tick_start = tick;

            timeout_reg[i].delay_ms = ms;

            timeout_reg[i].callback = task;

            return;
        }
    }

    sl_error("timeout task overflow, limit %2d", SL_TIMEOUT_LIMIT);
}

void sl_timeout_stop(pfunc task)
{
    sl_check_task_not_null();

    for (int i = 0; i < SL_TIMEOUT_LIMIT; i++)
    {
        if (timeout_reg[i].callback == task)
        {
            /* 抹掉callback，释放资源 */
            timeout_reg[i].callback = NULL;

            return;
        }
    }
}

/* ============================================================== */

/* 周期任务数据 */
typedef struct
{
    uint32_t tick_start;

    int delay_ms;

    pfunc callback;

} cycle_typ;

/* 周期任务注册表 */
static cycle_typ cycle_reg[SL_CYCLE_LIMIT];

/* 周期任务运行 */
void cycle_run(void)
{
    uint32_t tick_start;
    int delay_ms;

    /* 防止回调中改写注册表 */
    sl_backup_reg(cycle, CYCLE);

    for (int i = 0; i < SL_CYCLE_LIMIT; i++)
    {
        if (backup_reg[i].callback == NULL)
            continue;

        tick_start = backup_reg[i].tick_start;

        delay_ms = backup_reg[i].delay_ms;

        if ((uint32_t)(tick - tick_start) >= delay_ms)
        {
            /* 周期任务完成，更新时间戳，开启下一周期 */
            cycle_reg[i].tick_start = tick;

            backup_reg[i].callback();
        }
    }
}

/* 周期任务 */
void sl_cycle_start(int ms, pfunc task)
{
    sl_check_task_not_null();

    for (int i = 0; i < SL_CYCLE_LIMIT; i++)
    {
        if (cycle_reg[i].callback == task)
        {
            if (cycle_reg[i].delay_ms == ms)
                return;

            /* 参数有更新，在下面重新注册 */
            cycle_reg[i].callback = NULL;

            break;
        }
    }

    /* 如未注册，这里先注册 */
    for (int i = 0; i < SL_CYCLE_LIMIT; i++)
    {
        if (cycle_reg[i].callback == NULL)
        {
            cycle_reg[i].tick_start = tick;

            cycle_reg[i].delay_ms = ms;

            cycle_reg[i].callback = task;

            /* 周期任务开始时，执行一次 */
            if (task != NULL)
                task();

            return;
        }
    }

    sl_error("cycle task overflow, limit %2d", SL_CYCLE_LIMIT);
}

void sl_cycle_stop(pfunc task)
{
    sl_check_task_not_null();

    for (int i = 0; i < SL_CYCLE_LIMIT; i++)
    {
        if (cycle_reg[i].callback == task)
        {
            /* 抹掉callback，释放资源 */
            cycle_reg[i].callback = NULL;

            return;
        }
    }
}

/* ============================================================== */

/* 多次任务数据 */
typedef struct
{
    uint32_t tick_start;

    int delay_ms;

    int num;

    pfunc callback;

} multiple_typ;

/* 多次任务注册表 */
static multiple_typ multiple_reg[SL_MULTIPLE_LIMIT];

/* 多次任务运行 */
void multiple_run(void)
{
    uint32_t tick_start;
    int delay_ms;

    /* 防止回调中改写注册表 */
    sl_backup_reg(multiple, MULTIPLE);

    for (int i = 0; i < SL_MULTIPLE_LIMIT; i++)
    {
        if (backup_reg[i].callback == NULL)
            continue;

        tick_start = backup_reg[i].tick_start;

        delay_ms = backup_reg[i].delay_ms;

        if ((uint32_t)(tick - tick_start) >= delay_ms)
        {
            /* 多次任务完成，更新时间戳，开启下一多次 */
            multiple_reg[i].tick_start = tick;

            backup_reg[i].callback();

            multiple_reg[i].num--;

            /* 运行次数到达，停止 */
            if (multiple_reg[i].num == 0)
            {
                multiple_reg[i].callback = NULL;
            }
        }
    }
}

/* 多次任务 */
void sl_multiple_start(int num, int ms, pfunc task)
{
    sl_check_task_not_null();

    if (num == 0)
        return;

    /* 执行一次 */
    if (num == 1)
    {
        if (task != NULL)
            task();

        return;
    }

    for (int i = 0; i < SL_MULTIPLE_LIMIT; i++)
    {
        if (multiple_reg[i].callback == task)
        {
            if ((multiple_reg[i].num == num) && (multiple_reg[i].delay_ms == ms))
                return;

            /* 参数有更新，在下面重新注册 */
            multiple_reg[i].callback = NULL;

            break;
        }
    }

    /* 如未注册，这里先注册 */
    for (int i = 0; i < SL_MULTIPLE_LIMIT; i++)
    {
        if (multiple_reg[i].callback == NULL)
        {
            multiple_reg[i].tick_start = tick;

            multiple_reg[i].delay_ms = ms;

            multiple_reg[i].num = num - 1;

            multiple_reg[i].callback = task;

            /* 多次任务开始时，执行一次 */
            if (task != NULL)
                task();

            return;
        }
    }

    sl_error("multiple task overflow, limit %2d", SL_MULTIPLE_LIMIT);
}

void sl_multiple_stop(pfunc task)
{
    sl_check_task_not_null();

    for (int i = 0; i < SL_MULTIPLE_LIMIT; i++)
    {
        if (multiple_reg[i].callback == task)
        {
            /* 抹掉callback，释放资源 */
            multiple_reg[i].callback = NULL;

            return;
        }
    }
}

/* ============================================================== */

/* 并发任务注册表 */
static pfunc task_reg[SL_CONCURRENT_LIMIT];

/* 并发任务运行 */
void concurrent_task_run(void)
{
    for (int i = 0; i < SL_CONCURRENT_LIMIT; i++)
    {
        if (task_reg[i] == NULL)
            continue;

        task_reg[i]();
    }
}

/* 并发任务 */
void sl_task_start(pfunc task)
{
    sl_check_task_not_null();

    for (int i = 0; i < SL_CONCURRENT_LIMIT; i++)
    {
        if (task_reg[i] == task)
        {
            return;
        }
    }

    for (int i = 0; i < SL_CONCURRENT_LIMIT; i++)
    {
        if (task_reg[i] == NULL)
        {
            task_reg[i] = task;

            return;
        }
    }

    sl_error("concurrent task overflow, limit %2d", SL_CONCURRENT_LIMIT);
}

void sl_task_stop(pfunc task)
{
    sl_check_task_not_null();

    for (int i = 0; i < SL_CONCURRENT_LIMIT; i++)
    {
        if (task_reg[i] == task)
        {
            task_reg[i] = NULL;

            return;
        }
    }
}

/* ============================================================== */

/* 单次任务注册表 */
static pfunc once_task_reg[SL_ONCE_LIMIT];

/* 单次任务运行 */
void once_task_run(void)
{
    static pfunc backup_reg[SL_ONCE_LIMIT];

    __disable_irq();

    /* 防止回调中改写注册表 */
    memcpy(backup_reg, once_task_reg, sizeof backup_reg);

    __enable_irq();

    for (int i = 0; i < SL_ONCE_LIMIT; i++)
    {
        if (backup_reg[i] == NULL)
            continue;

        /* 运行一次后，取消注册 */
        once_task_reg[i] = NULL;

        backup_reg[i]();
    }
}

/* 单次任务 */
void sl_task_once(pfunc task)
{
    sl_check_task_not_null();

    for (int i = 0; i < SL_ONCE_LIMIT; i++)
    {
        if (once_task_reg[i] == task)
        {
            return;
        }
    }

    for (int i = 0; i < SL_ONCE_LIMIT; i++)
    {
        if (once_task_reg[i] == NULL)
        {
            once_task_reg[i] = task;

            return;
        }
    }

    sl_error("once task overflow, limit %2d", SL_ONCE_LIMIT);
}

/************************** END OF FILE **************************/
