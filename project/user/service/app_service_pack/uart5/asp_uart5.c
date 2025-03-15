/**
******************************************************************************
* @file    asp_uart5
* @author  暄暄
* @date    2025-1-16
* @brief   提供串口接收发送驱动及服务，收发都是 DMA+FIFO
==此文件用户不应变更==
*****************************************************************************/

#include "asp_uart5.h"
#include "msp_uart5.h"
#include "asp_uart5_send.h"
#include "asp_uart5_receive.h"

/* 串口初始化 */
void asp_uart5_init(void)
{
    /* 串口初始化 */
    msp_uart_init();

    /* 串口发送初始化 */
    asp_uart_send_init();

    /* 串口接收初始化 */
    asp_uart_receive_init();

    sys_prt_withFunc("init: uart5");
}

/*********************************** END OF FILE ***********************************/
