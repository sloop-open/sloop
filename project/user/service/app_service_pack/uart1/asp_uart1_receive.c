/**
 ******************************************************************************
 * @file    asp_uart_receive
 * @author  sloop
 * @date    2025-1-15
 * @brief   串口带FIFO的接收驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_uart1_receive.h"
#include "asp_uart1_rx_fifo.h"
#include "msp_uart1.h"

/* FIFO出队 */
static void fifo_dequeue(void);

/* 串口接收初始化 */
void asp_uart_receive_init(void)
{
    asp_fifo_init();

    sl_task_start(fifo_dequeue);
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
        sl_error("Receive length is 0");

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

    sl_printf("uart fifo rx, len: %d, data: ", len);

    for (int i = 0; i < len; i++)
        sl_prt_noNewLine("%02x ", data[i]);

    sl_prt_noNewLine("\n");

    sl_prt_withFunc("uart fifo rx: %s", data);
}

/************************** END OF FILE **************************/
