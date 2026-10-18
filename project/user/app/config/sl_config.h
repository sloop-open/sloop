/**
 ******************************************************************************
 * @file    sl_config
 * @author  sloop
 * @date    2024-12-18
 * @brief   在此处配置 sloop 和启用功能模块
 * ==此文件用户可配置==
 *****************************************************************************/

#ifndef __sl_config_H
#define __sl_config_H

/* 超时任务上限 */
#define SL_TIMEOUT_LIMIT 16

/* 周期任务上限 */
#define SL_CYCLE_LIMIT 16

/* 多次任务上限 */
#define SL_MULTIPLE_LIMIT 16

/* 并发任务上限 */
#define SL_CONCURRENT_LIMIT 32

/* 单次任务上限 */
#define SL_ONCE_LIMIT 16

/* ============================================================== */

/* 启用RTT打印 */
#define SL_RTT_ENABLE 1

/* RTT 打印缓冲区大小 */
#define SL_RTT_BUFFER_SIZE 2048

#endif /* __sl_config_H */

/************************** END OF FILE **************************/
