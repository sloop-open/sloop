/**
 ******************************************************************************
 * @file    asp_can_receive
 * @author  暄暄
 * @date    2025-1-15
 * @brief   can 带FIFO的接收驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __asp_can_receive_H
#define __asp_can_receive_H

#include "sp_common.h"

/* 对模块内函数进行重命名 */
#define asp_can_receive_init asp_can2_receive_init
#define asp_can_receive_callback asp_can2_receive_callback

/* can 接收初始化 */
void asp_can_receive_init(void);

/* 带FIFO的 can 接收回调 */
void asp_can_receive_callback(CanRxMsg *msg);

#endif /* __asp_can_receive_H */

/************************** END OF FILE **************************/
