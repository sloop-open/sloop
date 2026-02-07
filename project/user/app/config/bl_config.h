/**
 ******************************************************************************
 * @file    bl_config
 * @author  sloop
 * @date    2024-12-18
 * @brief   在此处配置 sloop 和启用功能模块
 * ==此文件用户可配置==
 *****************************************************************************/

#ifndef __bl_config_H
#define __bl_config_H

/* 超时任务上限 */
#define TIMEOUT_LIMIT 16

/* 周期任务上限 */
#define CYCLE_LIMIT 16

/* 多次任务上限 */
#define MULTIPLE_LIMIT 16

/* 并行任务上限 */
#define PARALLEL_TASK_LIMIT 32

/* 单次任务上限 */
#define ONCE_TASK_LIMIT 16

/* 控制台命令上限 */
#define SL_CMD_LIMIT 16

/* 控制台命令字符上限 */
#define SL_CMD_SIZE 64

/* 任务数据存储上限 */
#define TASK_DATA_LIMIT 64

/* ============================================================== */

/* 启用看门狗 */
#define SL_WDG_ENABLE 0

/* 启用RTT打印 */
#define SL_RTT_ENABLE 1

/* 启用行为日志 */
#define BHV_LOG_ENABLE 1

/* 启用控制台 */
#define SL_CMD_ENABLE 1

#endif /* __bl_config_H */

/************************** END OF FILE **************************/
