/**
 ******************************************************************************
 * @file    msp_can
 * @author  暄暄
 * @date    2024-10-21
 * @brief   can 总线驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __msp_can_H
#define __msp_can_H

#include "sp_common.h"

/* 对模块内函数进行重命名 */
#define msp_can_init msp_can2_init
#define msp_can_rx_callback msp_can2_rx_callback
#define msp_can_tx msp_can2_tx
#define can_rx_process can2_rx_process

void msp_can_init(void);

/* 接收回调 */
void msp_can_rx_callback(CanRxMsg *msg);

/* 发送数据 */
void msp_can_tx(CanTxMsg *msg);

#endif /* __msp_can_H */

/************************** END OF FILE **************************/
