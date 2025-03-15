/**
******************************************************************************
* @file    asp_uart6
* @author  暄暄
* @date    2025-1-16
* @brief   提供串口接收发送驱动及服务，收发都是 DMA+FIFO
==此文件用户不应变更==
*****************************************************************************/

#ifndef __asp_uart6_H
#define __asp_uart6_H

#include "sp_common.h"

/* 串口初始化 */
void asp_uart6_init(void);

/* 串口接收数据回调 */
void asp_uart6_receive_callback(uint8_t *data, int len);

/* 串口发送数据处理 */
void asp_uart6_send(uint8_t *data, int len);

#endif /* __asp_uart6_H */

/*********************************** END OF FILE ***********************************/
