/**
 ******************************************************************************
 * @file    mcu_base_timer
 * @author  xuan
 * @date    2024-12-16
 * @brief   提供 MCU 基础定时器
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __mcu_base_timer_H
#define __mcu_base_timer_H

#include "bl_common.h"

/* MCU 基础定时器初始化 */
void mcu_base_timer_init(void);

/* MCU tick 中断 */
void mcu_tick_irq(void);

/* 喂狗 */
void mcu_reload_wdg(void);

/* 获取 100ns 分辨率时间戳 */
uint16_t mcu_get_100nsRes(void);

#endif /* __mcu_base_timer_H */

/*********************************** END OF FILE ***********************************/
