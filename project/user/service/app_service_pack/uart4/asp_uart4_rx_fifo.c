/**
 ******************************************************************************
 * @file    asp_fifo
 * @author  暄暄
 * @date    2025-1-15
 * @brief   fifo
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_uart4_rx_fifo.h"

/* FIFO 的最大容量 */
#define FIFO_MAX_SIZE 8

typedef struct
{
    /* 帧长度 */
    int len;

    uint8_t data[FRAME_DATA_MAX_SIZE + 1];

} Frame;

typedef struct
{
    /* 存储帧的数组 */
    Frame buffer[FIFO_MAX_SIZE];

    /* 指向队列头部 */
    int head;

    /* 指向队列尾部 */
    int tail;

    /* 当前队列中的元素数量 */
    int size;

} FIFO;

static FIFO fifo;

void asp_fifo_init(void)
{
    fifo.head = 0;
    fifo.tail = 0;
    fifo.size = 0;
}

/* 查空 */
char asp_fifo_isEmpty(void)
{
    return fifo.size == 0;
}

/* 查满 */
char asp_fifo_isFull(void)
{
    return fifo.size == FIFO_MAX_SIZE;
}

/* 入队 */
void asp_fifo_enqueue(uint8_t *data, int len)
{
    if (asp_fifo_isFull())
    {
        sys_error("FIFO is full! Cannot enqueue.");

        return;
    }
    
    if (len > FRAME_DATA_MAX_SIZE)
    {
        sys_error("Frame data len too long (> %d)", FRAME_DATA_MAX_SIZE);

        return;
    }

    /* 添加帧到队列尾部 */
    fifo.buffer[fifo.tail].len = len;
    memcpy(fifo.buffer[fifo.tail].data, data, len);

    fifo.tail = (fifo.tail + 1) % FIFO_MAX_SIZE;
    fifo.size++;
}

/* 出队 */
void asp_fifo_dequeue(uint8_t *data, int *len)
{
    if (asp_fifo_isEmpty())
    {
        sys_error("FIFO is empty! Cannot dequeue.");

        return;
    }

    /* 从队列头部移除帧 */
    *len = fifo.buffer[fifo.head].len;
    memcpy(data, fifo.buffer[fifo.head].data, *len);

    fifo.head = (fifo.head + 1) % FIFO_MAX_SIZE;
    fifo.size--;
}

/* 打印FIFO */
void asp_fifo_print(void)
{
    sys_printf("FIFO contents:");

    int len = 0;

    int index = fifo.head;

    for (int i = 0; i < fifo.size; i++)
    {
        len = fifo.buffer[index].len;

        sys_printf("Frame %d: len = %d, Data = ", i + 1, len);

        fifo.buffer[index].data[len] = 0;

        sys_printf("%s", fifo.buffer[index].data);

        index = (index + 1) % FIFO_MAX_SIZE;
    }
}

/************************** END OF FILE **************************/
