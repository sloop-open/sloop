/**
******************************************************************************
* @file    asp_can
* @author  暄暄
* @date    2025-1-16
* @brief   提供 can 接收发送驱动及服务，收发都是 DMA+FIFO
==此文件用户不应变更==
*****************************************************************************/

#ifndef __asp_can_H
#define __asp_can_H

#include "sp_common.h"

/* can 初始化 */
void asp_can2_init(void);

/* can 接收数据回调 */
void asp_can2_receive_callback(CanRxMsg *msg);

/* can 发送数据处理 */
void asp_can2_send(CanTxMsg *msg);

#endif /* __asp_can_H */

/*********************************** END OF FILE ***********************************/
