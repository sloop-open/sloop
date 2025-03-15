/**
 ******************************************************************************
 * @file    asp_uart_send
 * @author  暄暄
 * @date    2025-1-15
 * @brief   串口带FIFO的发送驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __asp_uart_send_H
#define __asp_uart_send_H

#include "sp_common.h"

/* 对模块内函数进行重命名 */
#define asp_uart_send_init asp_uart5_send_init
#define asp_uart_send asp_uart5_send

/* 串口发送初始化 */
void asp_uart_send_init(void);

/* 带FIFO的串口发送 */
void asp_uart_send(uint8_t *data, int len);

#endif /* __asp_uart_send_H */

/************************** END OF FILE **************************/
