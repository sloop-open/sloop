/**
 ******************************************************************************
 * @file    sloop
 * @author  sloop
 * @date    2024-12-16
 * @brief   提供 sloop 系统API,比如超时/周期任务的创建、并发任务创建等
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __sloop_H
#define __sloop_H

#include "sl_common.h"

/* sloop 系统初始化 */
void sloop_init(void);
/* sloop 系统运行 */
void sloop_run(void);

/* MCU tick 中断 */
void sl_tick_irq(void);

/* 获取时间戳 */
uint32_t sl_get_tick(void);
/* 阻塞式延时 */
void sl_delay_ms(int ms);

/* 超时任务 */
void sl_timeout_start(int ms, pfunc task);
void sl_timeout_stop(pfunc task);

/* 周期任务 */
void sl_cycle_start(int ms, pfunc task);
void sl_cycle_stop(pfunc task);

/* 多次任务 */
void sl_multiple_start(int num, int ms, pfunc task);
void sl_multiple_stop(pfunc task);

/* 并发任务 */
void sl_task_start(pfunc task);
void sl_task_stop(pfunc task);

/* 异步单次任务 */
void sl_task_once(pfunc task);

#endif /* __sloop_H */

/************************** END OF FILE **************************/
