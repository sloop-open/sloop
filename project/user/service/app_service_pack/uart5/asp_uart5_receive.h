/**
 ******************************************************************************
 * @file    asp_uart_receive
 * @author  暄暄
 * @date    2025-1-15
 * @brief   串口带FIFO的接收驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __asp_uart_receive_H
#define __asp_uart_receive_H

#include "sp_common.h"

/* 对模块内函数进行重命名 */
#define asp_uart_receive_init asp_uart5_receive_init
#define asp_uart_receive_callback asp_uart5_receive_callback

/* 串口接收初始化 */
void asp_uart_receive_init(void);

/* 带FIFO的串口接收回调 */
void asp_uart_receive_callback(uint8_t *data, int len);

#endif /* __asp_uart_receive_H */

/************************** END OF FILE **************************/
