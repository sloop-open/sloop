/**
 ******************************************************************************
 * @file    asp_can_send
 * @author  暄暄
 * @date    2025-1-15
 * @brief   can 带FIFO的发送驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __asp_can_send_H
#define __asp_can_send_H

#include "sp_common.h"

/* 对模块内函数进行重命名 */
#define asp_can_send_init asp_can2_send_init
#define asp_can_send asp_can2_send

/* can 发送初始化 */
void asp_can_send_init(void);

/* 带FIFO的 can 发送 */
void asp_can_send(CanTxMsg *msg);

#endif /* __asp_can_send_H */

/************************** END OF FILE **************************/
