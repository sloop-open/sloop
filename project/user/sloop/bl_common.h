/**
 ******************************************************************************
 * @file    bl_common
 * @author  xuan
 * @date    2024-12-24
 * @brief   杂项包含
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __bl_common_H
#define __bl_common_H

#include "stm32f2xx.h"
#include "SEGGER_RTT.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "bl_config.h"

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

/* 循环自增，举例：add(i, 6) i = 0->6 循环自增 */
#define add(i, upper) i = (i == upper) ? 0 : i + 1
/* 循环自减，举例：sub(i, 6) i = 6->0 循环自减 */
#define sub(i, upper) i = (i == 0) ? upper : i - 1

/* ============================================================== */
/* 打印 API */

uint32_t sys_get_tick(void);

#if !SYS_RTT_ENABLE

void print_null(const char *sFormat, ...);

#define SEGGER_RTT_printf(sFormat, ...) print_null(sFormat, ##__VA_ARGS__)

#endif

/* 系统打印（带时间戳），RTT 简化版本 */
#define sys_printf(sFormat, ...) SEGGER_RTT_printf(0, RTT_CTRL_TEXT_GREEN "\n[%02d %02d:%02d:%02d.%03d] " RTT_CTRL_TEXT_YELLOW sFormat "\n" RTT_CTRL_RESET, \
                                                   sys_get_tick() / 1000 / 60 / 60 / 24,                                                                    \
                                                   sys_get_tick() / 1000 / 60 / 60 % 24,                                                                    \
                                                   sys_get_tick() / 1000 / 60 % 60,                                                                         \
                                                   sys_get_tick() / 1000 % 60,                                                                              \
                                                   sys_get_tick() % 1000,                                                                                   \
                                                   ##__VA_ARGS__, __func__)

/* 带函数名的打印 */
#define sys_prt_withFunc(sFormat, ...) sys_printf(sFormat RTT_CTRL_TEXT_GREEN " <func: %s>" RTT_CTRL_RESET, ##__VA_ARGS__, __func__)

/* 错误日志 */
#define sys_error(sFormat, ...) sys_prt_withFunc(RTT_CTRL_TEXT_BRIGHT_RED "[error] " sFormat RTT_CTRL_RESET, ##__VA_ARGS__)

/* 白色高亮日志，带 === ==== */
#define sys_focus(sFormat, ...) sys_printf(RTT_CTRL_TEXT_BRIGHT_WHITE "=== " sFormat " ===" RTT_CTRL_RESET, ##__VA_ARGS__)

/* 白色高亮日志 */
#define sys_prt_brWhite(sFormat, ...) sys_printf(RTT_CTRL_TEXT_BRIGHT_WHITE sFormat RTT_CTRL_RESET, ##__VA_ARGS__)

/* 黄色高亮日志 */
#define sys_prt_brYellow(sFormat, ...) sys_printf(RTT_CTRL_TEXT_BRIGHT_YELLOW sFormat RTT_CTRL_RESET, ##__VA_ARGS__)

/* 打印变量 */
#define sys_prt_var(var) sys_prt_withFunc(#var " = %d", (int)var)

/* 打印字符串 */
#define sys_prt_str(str) sys_prt_withFunc("%s", str)

/* 打印浮点数 */
#define sys_prt_float(var) sys_prt_withFunc(#var " = %d.%2d", (int)var, (int)(var * 1000) % 1000)

/* 连续打印（末尾不带换行），用于不换行连续输出 */
#define sys_prt_noNewLine(sFormat, ...) SEGGER_RTT_printf(0, RTT_CTRL_TEXT_YELLOW sFormat RTT_CTRL_RESET, ##__VA_ARGS__, __func__)

/* ============================================================== */
/* 互斥任务相关服务 */

extern char _init;
extern char _free;

/* 加载新任务 */
void sys_load_new_task(void);

/* 任务初始化宏 */
#define _INIT                            \
    if (_init == 1)                      \
    {                                    \
        sys_focus("enter %s", __func__); \
        _init = 0;

/* 任务释放宏 */
#define _FREE       \
    }               \
    if (_free == 1) \
    {

#define _RUN                        \
    sys_focus("exit %s", __func__); \
    sys_load_new_task();            \
    _init = 1;                      \
    _free = 0;                      \
    return;                         \
    }

#endif /* __bl_common_H */

/*********************************** END OF FILE ***********************************/
