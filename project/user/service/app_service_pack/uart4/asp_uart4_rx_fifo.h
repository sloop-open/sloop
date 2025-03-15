/**
 ******************************************************************************
 * @file    asp_fifo
 * @author  暄暄
 * @date    2025-1-15
 * @brief   fifo
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __asp_fifo_H
#define __asp_fifo_H

#include "sp_common.h"

/* 对模块内函数进行重命名 */
#define asp_fifo_init asp_uart4_rx_fifo_init
#define asp_fifo_isEmpty asp_uart4_rx_fifo_isEmpty
#define asp_fifo_isFull asp_uart4_rx_fifo_isFull
#define asp_fifo_enqueue asp_uart4_rx_fifo_enqueue
#define asp_fifo_dequeue asp_uart4_rx_fifo_dequeue
#define asp_fifo_print asp_uart4_rx_fifo_print

/* 每帧数据的最大长度 */
#define FRAME_DATA_MAX_SIZE 128

/* 初始化 */
void asp_fifo_init(void);

/* 查空 */
char asp_fifo_isEmpty(void);

/* 查满 */
char asp_fifo_isFull(void);

/* 入队 */
void asp_fifo_enqueue(uint8_t *data, int len);

/* 出队 */
void asp_fifo_dequeue(uint8_t *data, int *len);

/* 打印FIFO */
void asp_fifo_print(void);

#endif /* __asp_fifo_H */

/************************** END OF FILE **************************/
