/**
 ******************************************************************************
 * @file    msp_uart
 * @author  暄暄
 * @date    2025-1-10
 * @brief   提供串口接收发送驱动及服务，收发都是DMA
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __msp_uart_H
#define __msp_uart_H

#include "sp_common.h"

/* 对模块内函数进行重命名 */
#define msp_uart_init msp_uart6_init
#define msp_uart_start msp_uart6_start
#define msp_uart_rx_callback msp_uart6_rx_callback
#define msp_uart_tx msp_uart6_tx
#define msp_uart_txCplt_callback msp_uart6_txCplt_callback
#define uart_rx_timeout_process uart6_rx_timeout_process

/* 串口初始化 */
void msp_uart_init(void);

/* 串口接收数据回调 */
void msp_uart_rx_callback(uint8_t *data, int len);

/* 串口发送数据处理 */
void msp_uart_tx(uint8_t *data, int len);

/* 发送完成回调 */
void msp_uart_txCplt_callback(void);

#endif /* __msp_uart_H */

/*********************************** END OF FILE ***********************************/
