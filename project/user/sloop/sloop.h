/**
 ******************************************************************************
 * @file    sloop
 * @author  xuan
 * @date    2024-12-16
 * @brief   提供 sloop 系统API,比如超时/周期任务的创建、并行任务创建、互斥任务切换等
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __sloop_H
#define __sloop_H

#include "bl_common.h"

/* sloop 系统初始化 */
void sloop_init(void);
/* sloop 系统运行 */
void sloop(void);

/* 获取时间戳 */
uint32_t sys_get_tick(void);
/* 阻塞式延时 */
void sys_delay(int ms);

/* 获取 us 级时间戳 */
uint16_t sys_get_us(void);
/* 阻塞式延时 */
void sys_delay_us(int us);

/* 超时任务 */
void sys_timeout_start(int ms, pfunc task);
void sys_timeout_stop(pfunc task);

/* 周期任务 */
void sys_cycle_start(int ms, pfunc task);
void sys_cycle_stop(pfunc task);

/* 多次任务 */
void sys_multiple_start(int num, int ms, pfunc task);
void sys_multiple_stop(pfunc task);

/* 并行任务 */
void sys_task_start(pfunc task);
void sys_task_stop(pfunc task);

/* 单次任务，只执行一次，迟滞到 main loop 里执行。可用于中断中复杂逻辑下放执行 */
void sys_task_once(pfunc task);

/* 互斥任务切换 */
void sys_goto(pfunc task);

/* 非阻塞等待延时到达（只能在互斥任务中使用）0：等待完成, 1：等待被中断 */
char sys_wait(int ms);
/* 非阻塞裸等待，直到 break or continue */
char sys_wait_bare(void);

/* 中断等待，sys_wait 返回1 */
void sys_wait_break(void);
/* 忽略等待，sys_wait 返回0，会继续执行等待后的操作 */
void sys_wait_continue(void);
/* 获取等待状态 */
char sys_is_waiting(void);

/* 添加控制台命令 */
void sys_add_cmd(char *str, pfunc task);
/* 获取控制台输入字符串。可用于获取命令参数，推荐用 sscanf */
char *sys_get_cmd_str(void);
/* 按空格索引下一个字符串，便于解析命令参数 */
char *str_next(char *str);

/* 设置程序版本 */
void sys_set_version(char *str);
/* 查询程序版本 */
char *sys_get_version(void);

/* 重启 */
void sys_reboot(void);

/* 不可变更位置，否则编译报错 */
#include "behavior_log.h"

#endif /* __sloop_H */

/************************** END OF FILE **************************/
