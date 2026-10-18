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

#include "stm32g0xx_hal.h"
#include "SEGGER_RTT.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "sl_config.h"

#define sl_weak __attribute__((weak))

#define sl_packed __attribute__((packed))

#define sl_unused __attribute__((unused))

#define sl_used __attribute__((used))

/* 简化函数指针定义 */
typedef void (*pfunc)(void);

/* 弱定义func */
#define weak_define(func) \
    sl_weak void func(void) {}

/* 循环自增，举例：add(i, 6) i = [0,6] 循环自增 */
#define sl_add(i, upper) i = (i == upper) ? 0 : i + 1
/* 循环自减，举例：sub(i, 6) i = [6,0] 循环自减 */
#define sl_sub(i, upper) i = (i == 0) ? upper : i - 1

/* ============================================================== */
/* 打印 API */

uint32_t sl_get_tick(void);

#if !SL_RTT_ENABLE

void print_null(const char *sFormat, ...);

#define SEGGER_RTT_printf(sFormat, ...) print_null(sFormat, ##__VA_ARGS__)

#endif

/* 系统打印（带时间戳），RTT 简化版本 */
#define sl_prt_noFunc(sFormat, ...) SEGGER_RTT_printf(0, RTT_CTRL_TEXT_GREEN "\n[%02d %02d:%02d:%02d.%03d] " RTT_CTRL_TEXT_YELLOW sFormat "\n" RTT_CTRL_RESET, \
                                                      sl_get_tick() / 1000 / 60 / 60 / 24,                                                                     \
                                                      sl_get_tick() / 1000 / 60 / 60 % 24,                                                                     \
                                                      sl_get_tick() / 1000 / 60 % 60,                                                                          \
                                                      sl_get_tick() / 1000 % 60,                                                                               \
                                                      sl_get_tick() % 1000,                                                                                    \
                                                      ##__VA_ARGS__)

/* 带函数名的打印 */
#define sl_printf(sFormat, ...) sl_prt_noFunc(sFormat RTT_CTRL_TEXT_GREEN " <func: %s>" RTT_CTRL_RESET, ##__VA_ARGS__, __func__)

/* 错误日志 */
#define sl_error(sFormat, ...) sl_printf(RTT_CTRL_TEXT_BRIGHT_RED "[error] " sFormat RTT_CTRL_RESET, ##__VA_ARGS__)

/* 白色高亮日志，带 === ==== */
#define sl_focus(sFormat, ...) sl_prt_noFunc(RTT_CTRL_TEXT_BRIGHT_WHITE "=== " sFormat " ===" RTT_CTRL_RESET, ##__VA_ARGS__)

/* 白色高亮日志 */
#define sl_prt_brWhite(sFormat, ...) sl_prt_noFunc(RTT_CTRL_TEXT_BRIGHT_WHITE sFormat RTT_CTRL_RESET, ##__VA_ARGS__)

/* 黄色高亮日志 */
#define sl_prt_brYellow(sFormat, ...) sl_prt_noFunc(RTT_CTRL_TEXT_BRIGHT_YELLOW sFormat RTT_CTRL_RESET, ##__VA_ARGS__)

/* 打印变量 */
#define sl_prt_var(var) sl_printf(#var " = %d", (int)var)

/* 打印hex */
#define sl_prt_hex(hex) sl_printf(#hex " = 0x%x", (int)hex)

/* 打印字符串 */
#define sl_prt_str(str) sl_printf("%s", str)

/* 打印浮点数 */
#define sl_prt_float(var) sl_printf(#var " = %d.%02d", (int)var, abs((int)(var * 100) % 100))

/* 连续打印（末尾不带换行），用于不换行连续输出 */
#define sl_prt_noNewLine(sFormat, ...) SEGGER_RTT_printf(0, RTT_CTRL_TEXT_YELLOW sFormat RTT_CTRL_RESET, ##__VA_ARGS__, __func__)

/* ============================================================== */
/* 协程非阻塞等待 */

#define SL_BEGIN                            \
    static uint32_t _sl_state = 0;          \
    sl_unused static uint32_t _sl_tick = 0; \
    switch (_sl_state)                      \
    {                                       \
    case 0:

#define SL_END \
    }

/* 条件等待 */
#define sl_wait_until(cond)   \
    do                        \
    {                         \
        _sl_state = __LINE__; \
    case __LINE__:            \
        if (!(cond))          \
            return;           \
    } while (0);

/* 时间等待 */
#define sl_wait_ms(ms)                                              \
    do                                                              \
    {                                                               \
        _sl_tick = sl_get_tick();                                   \
        sl_wait_until((uint32_t)(sl_get_tick() - _sl_tick) >= (ms)) \
    } while (0);

#endif /* __sl_common_H */

/*********************************** END OF FILE ***********************************/
