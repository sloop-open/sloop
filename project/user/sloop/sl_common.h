/**
 ******************************************************************************
 * @file    sl_common
 * @author  sloop
 * @date    2024-12-24
 * @brief   杂项包含
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __sl_common_H
#define __sl_common_H

#include "stm32f2xx.h"
#include "SEGGER_RTT.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "sl_config.h"

#ifndef __weak
#define __weak __attribute__((weak))
#endif /* __weak */
#ifndef __packed
#define __packed __attribute__((packed))
#endif /* __packed */

/* 状态机枚举 */
enum
{
    step1,
    step2,
    step3,
    step4,
    step5,
    step6,
    step7,
    step8
};

/* 中断优先级 */
#define PRIO_HIGHEST 0
#define PRIO_LOWEST 15
#define PRIO_DEFAULT 5

/* 简化函数指针定义 */
typedef void (*pfunc)(void);

/* 弱定义func */
#define weak_define(func) \
    __weak void func(void) {}

/* 循环自增，举例：add(i, 6) i = [0,6] 循环自增 */
#define add(i, upper) i = (i == upper) ? 0 : i + 1
/* 循环自减，举例：sub(i, 6) i = [6,0] 循环自减 */
#define sub(i, upper) i = (i == 0) ? upper : i - 1

/* ============================================================== */
/* 打印 API */

uint32_t sl_get_tick(void);

#if !SL_RTT_ENABLE

void print_null(const char *sFormat, ...);

#define SEGGER_RTT_printf(sFormat, ...) print_null(sFormat, ##__VA_ARGS__)

#endif

/* 系统打印（带时间戳），RTT 简化版本 */
#define sl_printf(sFormat, ...) SEGGER_RTT_printf(0, RTT_CTRL_TEXT_GREEN "\n[%02d %02d:%02d:%02d.%03d] " RTT_CTRL_TEXT_YELLOW sFormat "\n" RTT_CTRL_RESET, \
                                                  sl_get_tick() / 1000 / 60 / 60 / 24,                                                                     \
                                                  sl_get_tick() / 1000 / 60 / 60 % 24,                                                                     \
                                                  sl_get_tick() / 1000 / 60 % 60,                                                                          \
                                                  sl_get_tick() / 1000 % 60,                                                                               \
                                                  sl_get_tick() % 1000,                                                                                    \
                                                  ##__VA_ARGS__)

/* 带函数名的打印 */
#define sl_prt_withFunc(sFormat, ...) sl_printf(sFormat RTT_CTRL_TEXT_GREEN " <func: %s>" RTT_CTRL_RESET, ##__VA_ARGS__, __func__)

/* 错误日志 */
#define sl_error(sFormat, ...) sl_prt_withFunc(RTT_CTRL_TEXT_BRIGHT_RED "[error] " sFormat RTT_CTRL_RESET, ##__VA_ARGS__)

/* 白色高亮日志，带 === ==== */
#define sl_focus(sFormat, ...) sl_printf(RTT_CTRL_TEXT_BRIGHT_WHITE "=== " sFormat " ===" RTT_CTRL_RESET, ##__VA_ARGS__)

/* 白色高亮日志 */
#define sl_prt_brWhite(sFormat, ...) sl_printf(RTT_CTRL_TEXT_BRIGHT_WHITE sFormat RTT_CTRL_RESET, ##__VA_ARGS__)

/* 黄色高亮日志 */
#define sl_prt_brYellow(sFormat, ...) sl_printf(RTT_CTRL_TEXT_BRIGHT_YELLOW sFormat RTT_CTRL_RESET, ##__VA_ARGS__)

/* 打印变量 */
#define sl_prt_var(var) sl_prt_withFunc(#var " = %d", (int)var)

/* 打印hex */
#define sl_prt_hex(hex) sl_prt_withFunc(#hex " = 0x%x", (int)hex)

/* 打印字符串 */
#define sl_prt_str(str) sl_prt_withFunc("%s", str)

/* 打印浮点数 */
#define sl_prt_float(var) sl_prt_withFunc(#var " = %d.%02d", (int)var, abs((int)(var * 100) % 100))

/* 连续打印（末尾不带换行），用于不换行连续输出 */
#define sl_prt_noNewLine(sFormat, ...) SEGGER_RTT_printf(0, RTT_CTRL_TEXT_YELLOW sFormat RTT_CTRL_RESET, ##__VA_ARGS__, __func__)

/* ============================================================== */
/* 互斥任务相关服务 */

extern char _init;
extern char _free;

/* 加载新任务 */
void sl_load_new_task(void);

/* 任务初始化宏 */
#define _INIT                           \
    if (_init == 1)                     \
    {                                   \
        sl_focus("enter %s", __func__); \
        _init = 0;

/* 任务释放宏 */
#define _FREE       \
    }               \
    if (_free == 1) \
    {

#define _RUN                       \
    sl_focus("exit %s", __func__); \
    sl_load_new_task();            \
    _init = 1;                     \
    _free = 0;                     \
    return;                        \
    }

/* ============================================================== */
/* Flow-based 协作式工作流编程 */

/* 注意事项
 *  1.Flow 内部不能使用局部变量
    2.flow 内部的用户状态机不能使用flow api,如FLOW_UNTIL
    3.严格遵循 flow 模板
    _FLOW_CONTEXT(flow_name)
    _FLOW_INIT
    _FLOW_FREE(flow_name)
    _FLOW_RUN
    _FLOW_END
    4.事件是单一消费模型
 */

/* 用于隔离用户状态机 ID 与flow ID */
#define FLOW_OFFSET 4201

/* Flow 状态 */
enum
{
    FLOW_INIT = FLOW_OFFSET,
    FLOW_FREE,
    FLOW_RUN,
    FLOW_IDLE,
};

/* Flow 状态定义 */
#define FLOW_STATE_DEFINE(flow_name) uint32_t flow_state_##flow_name;
#define FLOW_STATE_DECLARE(flow_name) extern uint32_t flow_state_##flow_name;

/* Flow 启动 */
#define FLOW_START(flow_name)               \
    do                                      \
    {                                       \
        flow_state_##flow_name = FLOW_INIT; \
        sl_task_start(flow_name);           \
    } while (0);

/* Flow 停止（外部） */
#define FLOW_STOP(flow_name) flow_state_##flow_name = FLOW_FREE

/* Flow 内部上下文 */
#define _FLOW_CONTEXT(flow_name)                  \
    static uint32_t _flow_tick;                   \
    static uint32_t _flow_state;                  \
    static uint32_t _state_backup;                \
    if (flow_state_##flow_name == FLOW_INIT)      \
    {                                             \
        _flow_state = FLOW_INIT;                  \
        flow_state_##flow_name = FLOW_IDLE;       \
    }                                             \
    else if (flow_state_##flow_name == FLOW_FREE) \
    {                                             \
        _flow_state = FLOW_FREE;                  \
        flow_state_##flow_name = FLOW_IDLE;       \
    }

/* 初始化区 */
#define _FLOW_INIT       \
    switch (_flow_state) \
    {                    \
    case FLOW_INIT:      \
    {

/* 清理区 */
#define _FLOW_FREE(flow_name) \
    _flow_state = FLOW_RUN;   \
    break;                    \
    }                         \
    case FLOW_FREE:           \
    {                         \
        sl_task_stop(flow_name);

/* 运行区 */
#define _FLOW_RUN            \
    _flow_state = FLOW_INIT; \
    break;                   \
    }                        \
    case FLOW_RUN:           \
    {

/* 结束 */
#define _FLOW_END \
    break;        \
    }             \
    }

/* ===================== */
/*      FLOW 原语        */
/* ===================== */

#define __FLOW_LINE__ (FLOW_OFFSET + 1024 + __LINE__)

/* 条件等待（核心原语） */
#define FLOW_UNTIL(cond)             \
    do                               \
    {                                \
        _state_backup = _flow_state; \
        _flow_state = __FLOW_LINE__; \
    case __FLOW_LINE__:              \
        if (!(cond))                 \
            return;                  \
        _flow_state = _state_backup; \
    } while (0);

/* 时间等待 */
#define FLOW_WAIT(ms)                                              \
    do                                                             \
    {                                                              \
        _flow_tick = sl_get_tick();                                \
        FLOW_UNTIL((uint32_t)(sl_get_tick() - _flow_tick) >= (ms)) \
    } while (0);

/* 事件定义 */
#define FLOW_EVENT_DEFINE(id) char flow_event_##id;
#define FLOW_EVENT_DECLARE(id) extern char flow_event_##id;

/* 发送事件 */
#define FLOW_SEND_EVENT(id) flow_event_##id = 1;

/* 等待事件（消费型） */
#define FLOW_WAIT_EVENT(id)          \
    do                               \
    {                                \
        FLOW_UNTIL(flow_event_##id); \
        flow_event_##id = 0;         \
    } while (0);

/* Flow 内部停止 */
#define FLOW_EXIT()              \
    do                           \
    {                            \
        _flow_state = FLOW_FREE; \
        return;                  \
    } while (0);

/* 业务状态机跳转 */
#define FLOW_GOTO(case_id) _flow_state = case_id;

#endif /* __sl_common_H */

/*********************************** END OF FILE ***********************************/
