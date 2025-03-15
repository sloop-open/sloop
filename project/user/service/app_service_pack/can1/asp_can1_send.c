/**
 ******************************************************************************
 * @file    asp_can_send
 * @author  暄暄
 * @date    2025-1-15
 * @brief   can 带FIFO的发送驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_can1_send.h"
#include "asp_can1_tx_fifo.h"
#include "msp_can1.h"

/* 发送帧间隔ms */
#define TX_INTERVAL 8

/* 发送完成标志 */
static uint32_t tx_cplt_tick;

/* FIFO出队 */
static void fifo_dequeue(void);
/* can FIFO出队 */
static void can_send_dequeue(void);

/* can 发送初始化 */
void asp_can_send_init(void)
{
    asp_fifo_init();

    sys_task_start(fifo_dequeue);
}

/* 带FIFO的can 发送 */
void asp_can_send(CanTxMsg *msg)
{
    asp_fifo_enqueue(msg);
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
            can_send_dequeue();

            /* 标记为正在发送 */
            isSending = 1;
        }
    }
}

/* can FIFO出队 */
static void can_send_dequeue(void)
{
    static CanTxMsg msg;

    /* 出队 */
    asp_fifo_dequeue(&msg);

    /* 启动发送 */
    msp_can_tx(&msg);

    tx_cplt_tick = sys_get_tick();

    if (asp_fifo_isEmpty())
        return;

    sys_timeout_start(TX_INTERVAL, can_send_dequeue);
}

/************************** END OF FILE **************************/
