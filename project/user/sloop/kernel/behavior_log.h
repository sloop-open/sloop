/**
 ******************************************************************************
 * @file    behavior_log
 * @author  sloop
 * @date    2024-12-18
 * @brief   在 sl_config.h 中开启行为日志功能。
 * 开启行为日志后，会对系统 API 进行打印增强。即在调用系统 API 时会有相应日志输出。
 * 这对分析系统工作流程会有帮助。
 *
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __behavior_log_H
#define __behavior_log_H

#include "sl_config.h"

#if !SL_RTT_ENABLE

#undef BHV_LOG_ENABLE

#define BHV_LOG_ENABLE 0

#endif

/* 任务名加入注册表 */
void add_task_name(pfunc task, char *str);

#if BHV_LOG_ENABLE

#define sl_delay(ms)                         \
    do                                        \
    {                                         \
        sl_prt_withFunc("delay: %d ms", ms); \
        sl_delay(ms);                        \
    } while (0)

/* ============================================================== */

#define sl_timeout_start(ms, task)                                   \
    do                                                                \
    {                                                                 \
        sl_prt_withFunc("timeout task start: %d ms, %s", ms, #task); \
        sl_timeout_start(ms, task);                                  \
    } while (0)

#define sl_timeout_stop(task)                             \
    do                                                     \
    {                                                      \
        sl_prt_withFunc("timeout task stop:  %s", #task); \
        sl_timeout_stop(task);                            \
    } while (0)

/* ============================================================== */

#define sl_cycle_start(ms, task)                                   \
    do                                                              \
    {                                                               \
        add_task_name(task, #task);                                 \
        sl_prt_withFunc("cycle task start: %d ms, %s", ms, #task); \
        sl_cycle_start(ms, task);                                  \
    } while (0)

#define sl_cycle_stop(task)                             \
    do                                                   \
    {                                                    \
        sl_prt_withFunc("cycle task stop:  %s", #task); \
        sl_cycle_stop(task);                            \
    } while (0)

/* ============================================================== */

#define sl_task_start(task)                                \
    do                                                      \
    {                                                       \
        add_task_name(task, #task);                         \
        sl_prt_withFunc("parallel task start: %s", #task); \
        sl_task_start(task);                               \
    } while (0)

#define sl_task_stop(task)                                 \
    do                                                      \
    {                                                       \
        sl_prt_withFunc("parallel task stop:  %s", #task); \
        sl_task_stop(task);                                \
    } while (0)

/* ============================================================== */

#define sl_goto(task)                      \
    do                                      \
    {                                       \
        add_task_name(task, #task);         \
        sl_prt_withFunc("goto %s", #task); \
        sl_goto(task);                     \
    } while (0)

#endif /* BHV_LOG_ENABLE */

#endif /* __behavior_log_H */

/************************** END OF FILE **************************/
