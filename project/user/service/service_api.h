/**
 ******************************************************************************
 * @file    service_api
 * @author  暄暄
 * @date    2025-1-27
 * @brief   服务层向应用层提供的 api
 * ==此文件用户不应变更(除了 can id)==
 *****************************************************************************/

#ifndef __service_api_H
#define __service_api_H

#include "sp_common.h"

/* ==蜂鸣器== */

/* 初始化 */
void asp_beep_init(void);

/* 单次鸣叫 */
void asp_beep(int ms);

/* 多次鸣叫 */
void asp_beep_multiple(int num, int interval, int continues);

/* ============================================================== */

/* ==运行灯== */

/* 初始化 */
void asp_run_led_init(void);

/* ============================================================== */

/* ==参数掉电保存== */

/* 自动初始化参数 */
void asp_auto_init_param(void);

/* 参数保存 */
void asp_param_save(void);

/* ============================================================== */

/* ==CAN1== */

/* CAN MCB ID */
#define MCB_ID 1
/* CAN FCB ID */
#define FCB_ID 2
/* CAN TCB ID */
#define TCB_ID 3

/* can 初始化 */
void asp_can1_init(void);

/* can 接收数据回调 */
void asp_can1_receive_callback(CanRxMsg *msg);

/* can 发送数据处理 */
void asp_can1_send(CanTxMsg *msg);

/* ============================================================== */

/* ==CAN2== */

/* can编码器 lift */
#define CAN_ENCODER_LIFT 2
/* can编码器 tube */
#define CAN_ENCODER_TUBE 3
/* can编码器 carm */
#define CAN_ENCODER_CARM 4

/* can 初始化 */
void asp_can2_init(void);

/* can 接收数据回调 */
void asp_can2_receive_callback(CanRxMsg *msg);

/* can 发送数据处理 */
void asp_can2_send(CanTxMsg *msg);

/* ============================================================== */

/* ==串口1== */

/* 串口初始化 */
void asp_uart1_init(void);

/* 串口接收数据回调 */
void asp_uart1_receive_callback(uint8_t *data, int len);

/* 串口发送数据处理 */
void asp_uart1_send(uint8_t *data, int len);

/* ============================================================== */

/* ==串口2== */

/* 串口初始化 */
void asp_uart2_init(void);

/* 串口接收数据回调 */
void asp_uart2_receive_callback(uint8_t *data, int len);

/* 串口发送数据处理 */
void asp_uart2_send(uint8_t *data, int len);

/* ============================================================== */

/* ==串口3== */

/* 串口初始化 */
void asp_uart3_init(void);

/* 串口接收数据回调 */
void asp_uart3_receive_callback(uint8_t *data, int len);

/* 串口发送数据处理 */
void asp_uart3_send(uint8_t *data, int len);

/* ============================================================== */

/* ==串口4== */

/* 串口初始化 */
void asp_uart4_init(void);

/* 串口接收数据回调 */
void asp_uart4_receive_callback(uint8_t *data, int len);

/* 串口发送数据处理 */
void asp_uart4_send(uint8_t *data, int len);

/* ============================================================== */

/* ==串口5== */

/* 串口初始化 */
void asp_uart5_init(void);

/* 串口接收数据回调 */
void asp_uart5_receive_callback(uint8_t *data, int len);

/* 串口发送数据处理 */
void asp_uart5_send(uint8_t *data, int len);

/* ============================================================== */

/* ==串口6== */

/* 串口初始化 */
void asp_uart6_init(void);

/* 串口接收数据回调 */
void asp_uart6_receive_callback(uint8_t *data, int len);

/* 串口发送数据处理 */
void asp_uart6_send(uint8_t *data, int len);

#endif /* __service_api_H */

/************************** END OF FILE **************************/
