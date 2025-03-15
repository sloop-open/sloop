/**
 ******************************************************************************
 * @file    asp_can_receive
 * @author  暄暄
 * @date    2025-1-15
 * @brief   can 带FIFO的接收驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_can1_receive.h"
#include "asp_can1_rx_fifo.h"
#include "msp_can1.h"

/* FIFO出队 */
static void fifo_dequeue(void);

/* can 接收初始化 */
void asp_can_receive_init(void)
{
    asp_fifo_init();

    sys_task_start(fifo_dequeue);
}

/* FIFO出队 */
static void fifo_dequeue(void)
{
    if (asp_fifo_isEmpty())
        return;

    static CanRxMsg msg;

    /* 出队 */
    asp_fifo_dequeue(&msg);

    asp_can_receive_callback(&msg);
}

/* can 接收数据处理 */
void msp_can_rx_callback(CanRxMsg *msg)
{
    asp_fifo_enqueue(msg);
}

/* 带FIFO的can 接收回调 */
__weak void asp_can_receive_callback(CanRxMsg *msg)
{
    char buff[9] = {0};

    int len = msg->DLC;

    int id = msg->StdId;

    memcpy(buff, msg->Data, len);

    sys_printf("can fifo rx, len: %d, id: %d, data: ", len, id);

    for (int i = 0; i < len; i++)
        sys_prt_noNewLine("%02x ", buff[i]);

    sys_prt_noNewLine("\n");

    sys_prt_withFunc("can fifo rx: %s", buff);
}

/************************** END OF FILE **************************/
