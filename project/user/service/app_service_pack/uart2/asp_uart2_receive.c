/**
 ******************************************************************************
 * @file    asp_uart_receive
 * @author  暄暄
 * @date    2025-1-15
 * @brief   串口带FIFO的接收驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_uart2_receive.h"
#include "asp_uart2_rx_fifo.h"
#include "msp_uart2.h"

/* FIFO出队 */
static void fifo_dequeue(void);

/* 串口接收初始化 */
void asp_uart_receive_init(void)
{
    asp_fifo_init();

    sys_task_start(fifo_dequeue);
}

/* FIFO出队 */
static void fifo_dequeue(void)
{
    if (asp_fifo_isEmpty())
        return;

    static int len;
    static uint8_t data[FRAME_DATA_MAX_SIZE];

    /* 出队 */
    asp_fifo_dequeue(data, &len);

    asp_uart_receive_callback(data, len);
}

/* 串口接收数据处理 */
void msp_uart_rx_callback(uint8_t *data, int len)
{
    if (len == 0)
    {
        sys_error("Receive length is 0");

        return;
    }

    asp_fifo_enqueue(data, len);
}

/* 带FIFO的串口接收回调 */
__weak void asp_uart_receive_callback(uint8_t *data, int len)
{
    if (len < FRAME_DATA_MAX_SIZE)
        data[len] = 0;
    else
        data[len - 1] = 0;

    sys_printf("uart fifo rx, len: %d, data: ", len);

    for (int i = 0; i < len; i++)
        sys_prt_noNewLine("%02x ", data[i]);

    sys_prt_noNewLine("\n");

    sys_prt_withFunc("uart fifo rx: %s", data);
}

/************************** END OF FILE **************************/
