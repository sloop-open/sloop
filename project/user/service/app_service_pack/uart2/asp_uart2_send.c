/**
 ******************************************************************************
 * @file    asp_uart_send
 * @author  暄暄
 * @date    2025-1-15
 * @brief   串口带FIFO的发送驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_uart2_send.h"
#include "asp_uart2_tx_fifo.h"
#include "msp_uart2.h"

/* 发送帧间隔ms */
#define TX_INTERVAL 8

/* 发送完成标志 */
static uint32_t tx_cplt_tick;

/* FIFO出队 */
static void fifo_dequeue(void);
/* 串口FIFO出队 */
static void uart_send_dequeue(void);

/* 串口发送初始化 */
void asp_uart_send_init(void)
{
    asp_fifo_init();

    sys_task_start(fifo_dequeue);
}

/* 带FIFO的串口发送 */
void asp_uart_send(uint8_t *data, int len)
{
    if (len == 0)
    {
        sys_error("Send length is 0");

        return;
    }

    asp_fifo_enqueue(data, len);
}

/* FIFO出队 */
static void fifo_dequeue(void)
{
    static char isSending;

    /* 发送中 */
    if (isSending)
    {
        /* fifo 发空了 */
        if (asp_fifo_isEmpty())
        {
            /* 标记为空闲状态 */
            isSending = 0;
        }

        return;
    }

    /* 发送空闲，检查是否有数据可发送 */
    if (!asp_fifo_isEmpty())
    {
        /* 与上次发送间隔时间 */
        if ((sys_get_tick() - tx_cplt_tick) > TX_INTERVAL)
        {
            /* 启动发送链 */
            uart_send_dequeue();

            /* 标记为正在发送 */
            isSending = 1;
        }
    }
}

/* 串口FIFO出队 */
static void uart_send_dequeue(void)
{
    static int len;
    static uint8_t data[FRAME_DATA_MAX_SIZE];

    /* 出队 */
    asp_fifo_dequeue(data, &len);

    /* 启动发送 */
    msp_uart_tx(data, len);
}

/* 发送完成回调 */
void msp_uart_txCplt_callback(void)
{
    tx_cplt_tick = sys_get_tick();

    if (asp_fifo_isEmpty())
        return;

    sys_timeout_start(TX_INTERVAL, uart_send_dequeue);
}

/************************** END OF FILE **************************/
