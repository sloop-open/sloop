/**
 ******************************************************************************
 * @file    sloop
 * @author  xuan
 * @date    2024-12-16
 * @brief   �ṩ sloop ϵͳAPI,���糬ʱ/��������Ĵ������������񴴽������������л���
 *
 * ==���ļ��û���Ӧ���==
 *****************************************************************************/

#include "sloop.h"
#include "mcu_base_timer.h"

#if !SYS_RTT_ENABLE

#undef BHV_LOG_ENABLE
#undef SYS_CMD_ENABLE

#define BHV_LOG_ENABLE 0
#define SYS_CMD_ENABLE 0

#endif

#if BHV_LOG_ENABLE

#define bhv_prt(sFormat, ...) sys_focus(sFormat, ##__VA_ARGS__)

#else

#define bhv_prt(sFormat, ...) print_null(sFormat, ##__VA_ARGS__)

#endif

#define check_task_not_null()          \
    if (task == NULL)                  \
    {                                  \
        sys_error("The task is null"); \
        return;                        \
    }

#define backup_reg(name1, name2)                  \
    static name1##_typ backup_reg[name2##_LIMIT]; \
    memcpy(backup_reg, name1##_reg, sizeof backup_reg);

void print_null(const char *sFormat, ...) {}

/* ��ʱ�������� */
void timeout_run(void);
/* ������������ */
void cycle_run(void);
/* ����������� */
void multiple_run(void);
/* ������������ */
void parallel_task_run(void);
/* ������������ */
void once_task_run(void);
/* ������������ */
void mutex_task_run(void);
/* �������ʱ�� */
void soft_timer(void);

/* ============================================================== */

/* ����ʱ�ӳ�ʼ�� */
weak_define(mcu_base_timer_init);
/* ι�� */
weak_define(mcu_reload_wdg);
/* ϵͳ���� */
void system_heartbeat(void);
/* ����ѭ������ */
void loop_counter(void);
/* ���� cpu ���� */
void calcul_cpu_load(void);
/* ���ؾ��� */
void load_warning(void);
/* ϵͳ����̨ */
void system_console(void);
/* ������������ */
void update_task_us(pfunc task, uint16_t us_start);
/* ����������������ʱ�� */
void update_cycle_us(pfunc task, uint16_t us_start);
/* ���������ӡ */
void once_task_print(pfunc task);

static volatile uint32_t tick;

static int loop;

static int load;

static int loop_us;

/* ============================================================== */

/* sloop ϵͳ��ʼ�� */
void sloop_init(void)
{
    sys_prt_brYellow("==================================");
    sys_prt_brYellow("========= sloop  (^-^) ==========");
    sys_prt_brYellow("==================================");

    mcu_base_timer_init();

    /* ���õ������� */
    sys_task_start(once_task_run);

    /* ���� loop ���� */
    sys_task_start(loop_counter);

    /* ���� cpu ���ؼ��� */
    sys_cycle_start(100, calcul_cpu_load);

    /* ��������������ι�� */
    sys_cycle_start(100, mcu_reload_wdg);

    /* ����ϵͳ���� */
    sys_cycle_start(1000, system_heartbeat);

    sys_prt_withFunc("system heartbeat start");

#if SYS_CMD_ENABLE

    /* ����ϵͳ����̨ */
    sys_cycle_start(100, system_console);

    sys_prt_withFunc("system console start");

#endif
}

/* sloop ϵͳ���� */
void sloop(void)
{
    /* ������������ */
    mutex_task_run();

    /* ������������ */
    parallel_task_run();
}

/* ============================================================== */

#undef sys_task_once

/* mcu tick �ж� */
void mcu_tick_irq(void)
{
    tick++;

    /* �����ʱ�� 1ms ����һ�� */
    sys_task_once(soft_timer);
}

/* �������ʱ�� */
void soft_timer(void)
{
    /* ��ʱ�������� */
    timeout_run();

    /* ������������ */
    cycle_run();

    /* ����������� */
    multiple_run();
}

/* ============================================================== */

/* ϵͳ���� */
void system_heartbeat(void)
{
    static int count;

    SEGGER_RTT_SetTerminal(1);

    sys_prt_var(count);

    SEGGER_RTT_SetTerminal(0);

    count++;
}

/* ���� */
void sys_reboot(void)
{
    NVIC_SystemReset();
}

/* ============================================================== */

/* CPU ���ػ������� CPU = 100% ʱ��һ�� loop ��ʱ�� ��λ0.1us��Լ����loop �ﵽ100usʱ��CPU ռ��Ϊ100% */
#define LOOP_BASE 1000

/* ����ѭ������ */
void loop_counter(void)
{
    loop++;
}

/* ���� cpu ���� */
void calcul_cpu_load(void)
{
    if (loop == 0)
        return;

    static char warning;

    loop_us = 1000000 / loop;

    load = loop_us * 1000 / LOOP_BASE;

    loop = 0;

    /* ���س��� 80% ,��־���� */
    if (warning == 0)
    {
        if (load > 800)
        {
            sys_cycle_start(1000, load_warning);

            warning = 1;
        }

        return;
    }

    /* ������� */
    if (load < 600)
    {
        sys_cycle_stop(load_warning);

        warning = 0;
    }
}

/* ���ؾ��� */
void load_warning(void)
{
    sys_error("cpu load over 80%%, reach %2d.%d%%, average loop time: %d.%d us", load / 10, load % 10, loop_us / 10, loop_us % 10);
}

/* ============================================================== */

#undef sys_delay

/* ��ȡʱ��� */
uint32_t sys_get_tick(void)
{
    return tick;
}

/* ����ʽ��ʱ */
void sys_delay(int ms)
{
    uint32_t tick_start = tick;

    /* ����1ms��ʵ����ʱ����1ms */
    ms == 1 ? ms++ : ms;

    while (1)
    {
        if ((uint32_t)(tick - tick_start) >= ms)
        {
            return;
        }

        mcu_reload_wdg();
    }
}

/* ============================================================== */

/* ��ȡ us ��ʱ��� */
uint16_t sys_get_us(void)
{
    return mcu_get_100nsRes() / 10;
}

/* ����ʽ��ʱ */
void sys_delay_us(int us)
{
    uint16_t tick_start = sys_get_us();

    /* ����1us��ʵ����ʱ����1us */
    us == 1 ? us++ : us;

    while (1)
    {
        if ((uint16_t)(sys_get_us() - tick_start) >= us)
        {
            return;
        }
    }
}

/* ============================================================== */

#undef sys_timeout_start
#undef sys_timeout_stop

/* ��ʱ�������� */
typedef struct
{
    uint32_t tick_start;

    int delay_ms;

    pfunc callback;

} timeout_typ;

/* ��ʱ����ע��� */
static timeout_typ timeout_reg[TIMEOUT_LIMIT];

/* ��ʱ�������� */
void timeout_run(void)
{
    uint32_t tick_start;
    int delay_ms;

    /* ��ֹ�ص��и�дע��� */
    backup_reg(timeout, TIMEOUT);

    for (int i = 0; i < TIMEOUT_LIMIT; i++)
    {
        if (backup_reg[i].callback == NULL)
            continue;

        tick_start = backup_reg[i].tick_start;

        delay_ms = backup_reg[i].delay_ms;

        if ((uint32_t)(tick - tick_start) >= delay_ms)
        {
            /* ��ʱ������ɣ��ͷ���Դ */
            timeout_reg[i].callback = NULL;

            backup_reg[i].callback();
        }
    }
}

/* ��ʱ���� */
void sys_timeout_start(int ms, pfunc task)
{
    check_task_not_null();

    /* ����1ms��ʵ����ʱ����1ms */
    ms == 1 ? ms++ : ms;

    for (int i = 0; i < TIMEOUT_LIMIT; i++)
    {
        if (timeout_reg[i].callback == task)
        {
            /* ��ע�ᣬ����ʱ��� */
            timeout_reg[i].tick_start = tick;

            return;
        }
    }

    /* ��δע�ᣬ������ע�� */
    for (int i = 0; i < TIMEOUT_LIMIT; i++)
    {
        if (timeout_reg[i].callback == NULL)
        {
            timeout_reg[i].tick_start = tick;

            timeout_reg[i].delay_ms = ms;

            timeout_reg[i].callback = task;

            return;
        }
    }

    sys_error("timeout task overflow, limit %2d", TIMEOUT_LIMIT);
}

void sys_timeout_stop(pfunc task)
{
    check_task_not_null();

    for (int i = 0; i < TIMEOUT_LIMIT; i++)
    {
        if (timeout_reg[i].callback == task)
        {
            /* Ĩ��callback���ͷ���Դ */
            timeout_reg[i].callback = NULL;

            return;
        }
    }
}

/* ============================================================== */

#undef sys_cycle_start
#undef sys_cycle_stop

/* ������������ */
typedef struct
{
    uint32_t tick_start;

    int delay_ms;

    pfunc callback;

} cycle_typ;

/* ��������ע��� */
static cycle_typ cycle_reg[CYCLE_LIMIT];

/* ������������ */
void cycle_run(void)
{
    uint32_t tick_start;
    int delay_ms;

    /* ��ֹ�ص��и�дע��� */
    backup_reg(cycle, CYCLE);

    for (int i = 0; i < CYCLE_LIMIT; i++)
    {
        if (backup_reg[i].callback == NULL)
            continue;

        tick_start = backup_reg[i].tick_start;

        delay_ms = backup_reg[i].delay_ms;

        if ((uint32_t)(tick - tick_start) >= delay_ms)
        {
            /* ����������ɣ�����ʱ�����������һ���� */
            cycle_reg[i].tick_start = tick;

            /* ��ʼ�����ʱ */
            uint16_t _us = mcu_get_100nsRes();

            backup_reg[i].callback();

            /* ������ʱ���������������� */
            update_cycle_us(backup_reg[i].callback, _us);
        }
    }
}

/* �������� */
void sys_cycle_start(int ms, pfunc task)
{
    check_task_not_null();

    for (int i = 0; i < CYCLE_LIMIT; i++)
    {
        if (cycle_reg[i].callback == task)
        {
            if (cycle_reg[i].delay_ms == ms)
                return;

            /* �����и��£�����������ע�� */
            cycle_reg[i].callback = NULL;

            break;
        }
    }

    /* ��δע�ᣬ������ע�� */
    for (int i = 0; i < CYCLE_LIMIT; i++)
    {
        if (cycle_reg[i].callback == NULL)
        {
            cycle_reg[i].tick_start = tick;

            cycle_reg[i].delay_ms = ms;

            cycle_reg[i].callback = task;

            /* ��������ʼʱ��ִ��һ�� */
            if (task != NULL)
                task();

            return;
        }
    }

    sys_error("cycle task overflow, limit %2d", CYCLE_LIMIT);
}

void sys_cycle_stop(pfunc task)
{
    check_task_not_null();

    for (int i = 0; i < CYCLE_LIMIT; i++)
    {
        if (cycle_reg[i].callback == task)
        {
            /* Ĩ��callback���ͷ���Դ */
            cycle_reg[i].callback = NULL;

            return;
        }
    }
}

/* ============================================================== */

/* ����������� */
typedef struct
{
    uint32_t tick_start;

    int delay_ms;

    int num;

    pfunc callback;

} multiple_typ;

/* �������ע��� */
static multiple_typ multiple_reg[MULTIPLE_LIMIT];

/* ����������� */
void multiple_run(void)
{
    uint32_t tick_start;
    int delay_ms;

    /* ��ֹ�ص��и�дע��� */
    backup_reg(multiple, MULTIPLE);

    for (int i = 0; i < MULTIPLE_LIMIT; i++)
    {
        if (backup_reg[i].callback == NULL)
            continue;

        tick_start = backup_reg[i].tick_start;

        delay_ms = backup_reg[i].delay_ms;

        if ((uint32_t)(tick - tick_start) >= delay_ms)
        {
            /* ���������ɣ�����ʱ�����������һ��� */
            multiple_reg[i].tick_start = tick;

            backup_reg[i].callback();

            multiple_reg[i].num--;

            /* ���д������ֹͣ */
            if (backup_reg[i].num == 0)
            {
                multiple_reg[i].callback = NULL;
            }
        }
    }
}

/* ������� */
void sys_multiple_start(int num, int ms, pfunc task)
{
    check_task_not_null();

    if (num == 0)
        return;

    /* ִ��һ�� */
    if (num == 1)
    {
        if (task != NULL)
            task();

        return;
    }

    for (int i = 0; i < MULTIPLE_LIMIT; i++)
    {
        if (multiple_reg[i].callback == task)
        {
            if ((multiple_reg[i].num == num) && (multiple_reg[i].delay_ms == ms))
                return;

            /* �����и��£�����������ע�� */
            multiple_reg[i].callback = NULL;

            break;
        }
    }

    /* ��δע�ᣬ������ע�� */
    for (int i = 0; i < MULTIPLE_LIMIT; i++)
    {
        if (multiple_reg[i].callback == NULL)
        {
            multiple_reg[i].tick_start = tick;

            multiple_reg[i].delay_ms = ms;

            multiple_reg[i].num = num - 1;

            multiple_reg[i].callback = task;

            /* �������ʼʱ��ִ��һ�� */
            if (task != NULL)
                task();

            return;
        }
    }

    sys_error("multiple task overflow, limit %2d", MULTIPLE_LIMIT);
}

void sys_multiple_stop(pfunc task)
{
    check_task_not_null();

    for (int i = 0; i < MULTIPLE_LIMIT; i++)
    {
        if (multiple_reg[i].callback == task)
        {
            /* Ĩ��callback���ͷ���Դ */
            multiple_reg[i].callback = NULL;

            return;
        }
    }
}

/* ============================================================== */

#undef sys_task_start
#undef sys_task_stop

/* ��������ע��� */
static pfunc task_reg[PARALLEL_TASK_LIMIT];

/* ������������ */
void parallel_task_run(void)
{
    for (int i = 0; i < PARALLEL_TASK_LIMIT; i++)
    {
        if (task_reg[i] == NULL)
            continue;

        /* ��ʼ�����ʱ */
        uint16_t _us = mcu_get_100nsRes();

        task_reg[i]();

        /* ������ʱ���������������� */
        update_task_us(task_reg[i], _us);
    }
}

/* �������� */
void sys_task_start(pfunc task)
{
    check_task_not_null();

    for (int i = 0; i < PARALLEL_TASK_LIMIT; i++)
    {
        if (task_reg[i] == task)
        {
            return;
        }
    }

    for (int i = 0; i < PARALLEL_TASK_LIMIT; i++)
    {
        if (task_reg[i] == NULL)
        {
            task_reg[i] = task;

            return;
        }
    }

    sys_error("parallel task overflow, limit %2d", PARALLEL_TASK_LIMIT);
}

void sys_task_stop(pfunc task)
{
    check_task_not_null();

    for (int i = 0; i < PARALLEL_TASK_LIMIT; i++)
    {
        if (task_reg[i] == task)
        {
            task_reg[i] = NULL;

            return;
        }
    }
}

/* ============================================================== */

#undef sys_task_once

/* ��������ע��� */
static pfunc once_task_reg[ONCE_TASK_LIMIT];

static int soft_timer_count;

/* ������������ */
void once_task_run(void)
{
    static pfunc backup_reg[ONCE_TASK_LIMIT];

    /* ��ֹ�ص��и�дע��� */
    memcpy(backup_reg, once_task_reg, sizeof backup_reg);

    for (int i = 0; i < ONCE_TASK_LIMIT; i++)
    {
        if (backup_reg[i] == NULL)
            continue;

        /* ����һ�κ�ȡ��ע�� */
        once_task_reg[i] = NULL;

        backup_reg[i]();

        if (backup_reg[i] == soft_timer)
            soft_timer_count = 0;
    }
}

/* �������� */
void sys_task_once(pfunc task)
{
    check_task_not_null();

    for (int i = 0; i < ONCE_TASK_LIMIT; i++)
    {
        if (once_task_reg[i] == task)
        {
            /* ��ʱ���� */
            if (task == soft_timer)
            {
                soft_timer_count++;

                if (soft_timer_count <= 3)
                    return;
            }

            once_task_print(task);

            return;
        }
    }

    for (int i = 0; i < ONCE_TASK_LIMIT; i++)
    {
        if (once_task_reg[i] == NULL)
        {
            once_task_reg[i] = task;

            return;
        }
    }

    sys_error("once task overflow, limit %2d", ONCE_TASK_LIMIT);
}

/* ============================================================== */

#undef sys_goto

static char wait;
char _init;
char _free;
static pfunc run_task;
static pfunc pre_task;

/* ���������л� */
void sys_goto(pfunc task)
{
    check_task_not_null();

    if (wait)
    {
        /* �л�����ǿ���жϵȴ� */
        sys_wait_break();
    }

    /* ��һ��δ���ع����������⴦�� */
    if (run_task == NULL)
    {
        run_task = task;

        _init = 1;

        return;
    }

    pre_task = task;

    _free = 1;
}

/* ���������� */
void sys_load_new_task(void)
{
    run_task = pre_task;
}

/* ������������ */
void mutex_task_run(void)
{
    if (run_task != NULL)
    {
        /* ��ʼ�����ʱ */
        uint16_t _us = mcu_get_100nsRes();

        run_task();

        /* ������ʱ���������������� */
        update_task_us(run_task, _us);
    }
}

/* ============================================================== */

static volatile char break_wait;
static volatile char _continue;

/* �������ȴ���ʱ���ֻ���ڻ���������ʹ�ã� */
char sys_wait(int ms)
{
    if (wait)
    {
        sys_error("sys_wait cannot be called nested");

        return 1;
    }

    /* ��λ��ϱ�־ */
    break_wait = 0;
    _continue = 0;

    wait = 1;

    uint32_t tick_start = tick;

    /* ����1ms��ʵ����ʱ����1ms */
    ms == 1 ? ms++ : ms;

    bhv_prt("enter wait: %d ms", ms);

    while (1)
    {
        /* ��ѯ����ǰ��������Ĳ������� */
        parallel_task_run();

        /* �жϵȴ� */
        if (break_wait)
        {
            break_wait = 0;

            wait = 0;

            return 1;
        }

        /* ���Եȴ� */
        if (_continue)
        {
            _continue = 0;

            wait = 0;

            return 0;
        }

        if ((uint32_t)(tick - tick_start) >= ms)
        {
            break;
        }
    }

    bhv_prt("exit wait");

    wait = 0;

    return 0;
}

/* ��������ȴ���ֱ�� break or continue */
char sys_wait_bare(void)
{
    if (wait)
    {
        sys_error("sys_wait_bare cannot be called nested");

        return 1;
    }

    /* ��λ��ϱ�־ */
    break_wait = 0;
    _continue = 0;

    wait = 1;

    bhv_prt("enter wait bare");

    char r = 0;

    while (1)
    {
        /* ��ѯ����ǰ��������Ĳ������� */
        parallel_task_run();

        /* �жϵȴ� */
        if (break_wait)
        {
            break_wait = 0;

            r = 1;

            break;
        }

        /* ���Եȴ� */
        if (_continue)
        {
            _continue = 0;

            r = 0;

            break;
        }
    }

    bhv_prt("exit wait bare");

    wait = 0;

    return r;
}

/* ��ȡ�ȴ�״̬ */
char sys_is_waiting(void)
{
    return wait;
}

/* �жϵȴ� */
void sys_wait_break(void)
{
    break_wait = 1;

    sys_printf("break wait");
}

/* ���Եȴ��������ִ�еȴ���Ĳ��� */
void sys_wait_continue(void)
{
    _continue = 1;

    sys_printf("ignore wait and continue");
}

/* ============================================================== */

#if BHV_LOG_ENABLE

/* ��������󳤶� */
#define MAX_CHAR 32

/* �������� */
typedef struct
{
    pfunc task;

    char str[MAX_CHAR + 1];

    /* ������������ʱ��,��λ0.1us */
    int _us;

    int sum;

    int count;

} task_data_typ;

static task_data_typ task_data[TASK_DATA_LIMIT] = {

    [0].task = soft_timer,
    [0].str = "soft_timer",

};

/* ����������ע��� */
void add_task_name(pfunc task, char *str)
{
    check_task_not_null();

    if (strlen(str) > MAX_CHAR)
    {
        sys_error("The task name is too long, more than %2d", MAX_CHAR);
        sys_error("The task name: %s", str);
    }

    for (int i = 0; i < TASK_DATA_LIMIT; i++)
    {
        if (task_data[i].task == task)
        {
            return;
        }
    }

    /* ��δע�ᣬ������ע�� */
    for (int i = 0; i < TASK_DATA_LIMIT; i++)
    {
        if (task_data[i].task == NULL)
        {
            task_data[i].task = task;

            strncpy(task_data[i].str, str, MAX_CHAR);

            return;
        }
    }

    sys_error("task data overflow, limit %2d", TASK_DATA_LIMIT);
}

/* ������������ */
task_data_typ *find_task_data(pfunc task)
{
    for (int i = 0; i < TASK_DATA_LIMIT; i++)
    {
        if (task_data[i].task == task)
        {
            return &task_data[i];
        }
    }

    return NULL;
}

/* ������������ */
void update_task_us(pfunc task, uint16_t us_start)
{
    static int count;

    /* ִ��100�Σ�����һ�Σ����ͼ������ڵ��µ� CPU �������� */
    add(count, 100);

    if (count != 0)
        return;

    /* ������ʱ */
    uint16_t us_end = mcu_get_100nsRes();

    uint16_t us = (uint16_t)(us_end - us_start);

    for (int i = 0; i < TASK_DATA_LIMIT; i++)
    {
        if (task_data[i].task == task)
        {
            task_data[i].sum += us;

            task_data[i].count++;

            if (task_data[i].count >= 100)
            {
                task_data[i]._us = task_data[i].sum / 100;

                task_data[i].sum = 0;

                task_data[i].count = 0;
            }

            return;
        }
    }
}

/* ����������������ʱ�� */
void update_cycle_us(pfunc task, uint16_t us_start)
{
    /* ������ʱ */
    uint16_t us_end = mcu_get_100nsRes();

    uint16_t us = (uint16_t)(us_end - us_start);

    for (int i = 0; i < TASK_DATA_LIMIT; i++)
    {
        if (task_data[i].task == task)
        {
            task_data[i].sum += us;

            task_data[i].count++;

            if (task_data[i].count >= 10)
            {
                task_data[i]._us = task_data[i].sum / 10;

                task_data[i].sum = 0;

                task_data[i].count = 0;
            }

            return;
        }
    }
}

/* ��ӡ�����е����� */
void task_print(void)
{
    sys_prt_brWhite("====== Currently running tasks ======");

    sys_focus("parallel tasks:");

    int count = 0;

    /* ��ӡ�������� */
    for (int i = 0; i < PARALLEL_TASK_LIMIT; i++)
    {
        if (task_reg[i] == NULL)
            continue;

        char *str = find_task_data(task_reg[i])->str;

        if (str == NULL)
            continue;

        count++;

        int _us = find_task_data(task_reg[i])->_us;

        sys_prt_brYellow("%d: %s, cycle: %d.%d us", count, str, _us / 10, _us % 10);
    }

    sys_focus("cycle tasks:");

    count = 0;

    /* ��ӡ�������� */
    for (int i = 0; i < CYCLE_LIMIT; i++)
    {
        if (cycle_reg[i].callback == NULL)
            continue;

        char *str = find_task_data(cycle_reg[i].callback)->str;

        int ms = cycle_reg[i].delay_ms;

        if (str == NULL)
            continue;

        count++;

        int _us = find_task_data(cycle_reg[i].callback)->_us;

        sys_prt_brYellow("%d: %s, period: %d ms, cycle: %d.%d us", count, str, ms, _us / 10, _us % 10);
    }

    char *str = find_task_data(run_task)->str;

    int _us = find_task_data(run_task)->_us;

    /* ��ӡ�������� */
    sys_focus("mutex task:");

    sys_prt_brYellow("%s, cycle: %d.%d us", str, _us / 10, _us % 10);

    sys_prt_brYellow("cpu load: %2d.%d%%, average loop time: %d.%d us", load / 10, load % 10, loop_us / 10, loop_us % 10);
}

/* ���������ӡ */
void once_task_print(pfunc task)
{
    char *str = find_task_data(task)->str;

    sys_error("The last once task: %s has a delay and not been executed yet", str);
}

#else /* BHV_LOG_ENABLE */

/* �����壬��ֹ RTT �ر�ʱ�����뱨�� */

void update_task_us(pfunc task, uint16_t us_start) {}

void update_cycle_us(pfunc task, uint16_t us_start) {}

void once_task_print(pfunc task) {}

#endif /* BHV_LOG_ENABLE */

/* cpu ��ӡ */
void cpu_print(void)
{
    sys_prt_brYellow("cpu load: %2d.%d%%, average loop time: %d.%d us", load / 10, load % 10, loop_us / 10, loop_us % 10);
}

/************************** END OF FILE **************************/
