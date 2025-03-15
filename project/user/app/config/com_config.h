/**
 ******************************************************************************
 * @file    com_config
 * @author  暄暄
 * @date    2025-1-27
 * @brief   串口和can 配置
 * ==此文件用户可配置==
 *****************************************************************************/

#ifndef __com_config_H
#define __com_config_H

#if defined MSP_CAN1

#include "service_api.h"

/* CAN1 配置 */

/* 发送回显 */
#define TX_ECHO 1
/* 接收回显 */
#define RX_ECHO 1

#define CANx_GPIO_PORT GPIOB
#define CANx_GPIO_CLK RCC_AHB1Periph_GPIOB
#define CANx_RX_SOURCE GPIO_PinSource8
#define CANx_RX_PIN GPIO_Pin_8
#define CANx_TX_SOURCE GPIO_PinSource9
#define CANx_TX_PIN GPIO_Pin_9

#define CANx CAN1
#define CANx_AF_PORT GPIO_AF_CAN1
#define CANx_CLK RCC_APB1Periph_CAN1
#define CANx_IRQn CAN1_RX0_IRQn
#define CANx_IRQHandler CAN1_RX0_IRQHandler
#define CANx_IT CAN_IT_FMP0
#define CANx_CAN_FilterNumber 0
#define CANx_FilterFIFOAssignment 0
#define CANx_FIFONumber 0
#define CANx_Mode CAN_Mode_Normal

/* 接收 ID，用于过滤接收数据 */
#define RX_ID1 MCB_ID
#define RX_ID2 0
#define RX_ID3 0
#define RX_ID4 0

/* ============================================================== */

#elif defined MSP_CAN2

#include "service_api.h"

/* CAN2 配置 */

/* 发送回显 */
#define TX_ECHO 1
/* 接收回显 */
#define RX_ECHO 0

#define CANx_GPIO_PORT GPIOB
#define CANx_GPIO_CLK RCC_AHB1Periph_GPIOB
#define CANx_RX_SOURCE GPIO_PinSource5
#define CANx_RX_PIN GPIO_Pin_5
#define CANx_TX_SOURCE GPIO_PinSource6
#define CANx_TX_PIN GPIO_Pin_6

#define CANx CAN2
#define CANx_AF_PORT GPIO_AF_CAN2
#define CANx_CLK RCC_APB1Periph_CAN2
#define CANx_IRQn CAN2_RX1_IRQn
#define CANx_IRQHandler CAN2_RX1_IRQHandler
#define CANx_IT CAN_IT_FMP1
#define CANx_CAN_FilterNumber 14
#define CANx_FilterFIFOAssignment 1
#define CANx_FIFONumber 1
#define CANx_Mode CAN_Mode_Normal

/* 接收 ID，用于过滤接收数据 */
#define RX_ID1 CAN_ENCODER_LIFT
#define RX_ID2 CAN_ENCODER_TUBE
#define RX_ID3 CAN_ENCODER_CARM
#define RX_ID4 0

/* ============================================================== */

#elif defined MSP_UART1

/* 串口1 配置 */

/* 发送回显 */
#define TX_ECHO 1
/* 接收回显 */
#define RX_ECHO 1

/* 接收超时时间 ms */
#define TIMEOUT_RX 3

#define USARTx_TX_GPIO_PORT GPIOA
#define USARTx_TX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define USARTx_TX_SOURCE GPIO_PinSource9
#define USARTx_TX_PIN GPIO_Pin_9

#define USARTx_RX_GPIO_PORT GPIOA
#define USARTx_RX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define USARTx_RX_SOURCE GPIO_PinSource10
#define USARTx_RX_PIN GPIO_Pin_10

#define USARTx USART1
#define USARTx_TX_AF GPIO_AF_USART1
#define USARTx_RX_AF GPIO_AF_USART1
#define USARTx_CLK RCC_APB2Periph_USART1
#define USARTx_CLK_CMD RCC_APB2PeriphClockCmd
#define USARTx_IRQn USART1_IRQn
#define USARTx_IRQHandler USART1_IRQHandler
#define BAUD_RATE 19200

#define DMAx DMA2
#define DMAx_CLK RCC_AHB1Periph_DMA2
#define DMAX_CLK_CMD RCC_AHB1PeriphClockCmd
#define USARTx_TX_DMA_CHANNEL DMA_Channel_4
#define USARTx_TX_DMA_STREAM DMA2_Stream7
#define USARTx_RX_DMA_CHANNEL DMA_Channel_4
#define USARTx_RX_DMA_STREAM DMA2_Stream5
#define USARTx_DR_ADDRESS ((uint32_t) & USARTx->DR)

/* ============================================================== */

#elif defined MSP_UART2

/* 串口1 配置 */

/* 发送回显 */
#define TX_ECHO 1
/* 接收回显 */
#define RX_ECHO 1

/* 接收超时时间 ms */
#define TIMEOUT_RX 3

#define USARTx_TX_GPIO_PORT GPIOA
#define USARTx_TX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define USARTx_TX_SOURCE GPIO_PinSource2
#define USARTx_TX_PIN GPIO_Pin_2

#define USARTx_RX_GPIO_PORT GPIOA
#define USARTx_RX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define USARTx_RX_SOURCE GPIO_PinSource3
#define USARTx_RX_PIN GPIO_Pin_3

#define USARTx USART2
#define USARTx_TX_AF GPIO_AF_USART2
#define USARTx_RX_AF GPIO_AF_USART2
#define USARTx_CLK RCC_APB1Periph_USART2
#define USARTx_CLK_CMD RCC_APB1PeriphClockCmd
#define USARTx_IRQn USART2_IRQn
#define USARTx_IRQHandler USART2_IRQHandler
#define BAUD_RATE 115200

#define DMAx DMA1
#define DMAx_CLK RCC_AHB1Periph_DMA1
#define DMAX_CLK_CMD RCC_AHB1PeriphClockCmd
#define USARTx_TX_DMA_CHANNEL DMA_Channel_4
#define USARTx_TX_DMA_STREAM DMA1_Stream6
#define USARTx_RX_DMA_CHANNEL DMA_Channel_4
#define USARTx_RX_DMA_STREAM DMA1_Stream5
#define USARTx_DR_ADDRESS ((uint32_t) & USARTx->DR)

/* ============================================================== */

#elif defined MSP_UART3

/* 串口1 配置 */

/* 发送回显 */
#define TX_ECHO 1
/* 接收回显 */
#define RX_ECHO 1

/* 接收超时时间 ms */
#define TIMEOUT_RX 3

#define USARTx_TX_GPIO_PORT GPIOC
#define USARTx_TX_GPIO_CLK RCC_AHB1Periph_GPIOC
#define USARTx_TX_SOURCE GPIO_PinSource10
#define USARTx_TX_PIN GPIO_Pin_10

#define USARTx_RX_GPIO_PORT GPIOC
#define USARTx_RX_GPIO_CLK RCC_AHB1Periph_GPIOC
#define USARTx_RX_SOURCE GPIO_PinSource11
#define USARTx_RX_PIN GPIO_Pin_11

#define USARTx USART3
#define USARTx_TX_AF GPIO_AF_USART3
#define USARTx_RX_AF GPIO_AF_USART3
#define USARTx_CLK RCC_APB1Periph_USART3
#define USARTx_CLK_CMD RCC_APB1PeriphClockCmd
#define USARTx_IRQn USART3_IRQn
#define USARTx_IRQHandler USART3_IRQHandler
#define BAUD_RATE 19200

#define DMAx DMA1
#define DMAx_CLK RCC_AHB1Periph_DMA1
#define DMAX_CLK_CMD RCC_AHB1PeriphClockCmd
#define USARTx_TX_DMA_CHANNEL DMA_Channel_4
#define USARTx_TX_DMA_STREAM DMA1_Stream3
#define USARTx_RX_DMA_CHANNEL DMA_Channel_4
#define USARTx_RX_DMA_STREAM DMA1_Stream1
#define USARTx_DR_ADDRESS ((uint32_t) & USARTx->DR)

/* ============================================================== */

#elif defined MSP_UART4

/* 串口4 配置 */

/* 发送回显 */
#define TX_ECHO 1
/* 接收回显 */
#define RX_ECHO 1

/* 接收超时时间 ms */
#define TIMEOUT_RX 3

#define USARTx_TX_GPIO_PORT GPIOA
#define USARTx_TX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define USARTx_TX_SOURCE GPIO_PinSource0
#define USARTx_TX_PIN GPIO_Pin_0

#define USARTx_RX_GPIO_PORT GPIOA
#define USARTx_RX_GPIO_CLK RCC_AHB1Periph_GPIOA
#define USARTx_RX_SOURCE GPIO_PinSource1
#define USARTx_RX_PIN GPIO_Pin_1

#define USARTx UART4
#define USARTx_TX_AF GPIO_AF_UART4
#define USARTx_RX_AF GPIO_AF_UART4
#define USARTx_CLK RCC_APB1Periph_UART4
#define USARTx_CLK_CMD RCC_APB1PeriphClockCmd
#define USARTx_IRQn UART4_IRQn
#define USARTx_IRQHandler UART4_IRQHandler
#define BAUD_RATE 115200

#define DMAx DMA1
#define DMAx_CLK RCC_AHB1Periph_DMA1
#define DMAX_CLK_CMD RCC_AHB1PeriphClockCmd
#define USARTx_TX_DMA_CHANNEL DMA_Channel_4
#define USARTx_TX_DMA_STREAM DMA1_Stream4
#define USARTx_RX_DMA_CHANNEL DMA_Channel_4
#define USARTx_RX_DMA_STREAM DMA1_Stream2
#define USARTx_DR_ADDRESS ((uint32_t) & USARTx->DR)

/* ============================================================== */

#elif defined MSP_UART5

/* 串口4 配置 */

/* 发送回显 */
#define TX_ECHO 1
/* 接收回显 */
#define RX_ECHO 1

/* 接收超时时间 ms */
#define TIMEOUT_RX 3

#define USARTx_TX_GPIO_PORT GPIOC
#define USARTx_TX_GPIO_CLK RCC_AHB1Periph_GPIOC
#define USARTx_TX_SOURCE GPIO_PinSource12
#define USARTx_TX_PIN GPIO_Pin_12

#define USARTx_RX_GPIO_PORT GPIOD
#define USARTx_RX_GPIO_CLK RCC_AHB1Periph_GPIOD
#define USARTx_RX_SOURCE GPIO_PinSource2
#define USARTx_RX_PIN GPIO_Pin_2

#define USARTx UART5
#define USARTx_TX_AF GPIO_AF_UART5
#define USARTx_RX_AF GPIO_AF_UART5
#define USARTx_CLK RCC_APB1Periph_UART5
#define USARTx_CLK_CMD RCC_APB1PeriphClockCmd
#define USARTx_IRQn UART5_IRQn
#define USARTx_IRQHandler UART5_IRQHandler
#define BAUD_RATE 19200

#define DMAx DMA1
#define DMAx_CLK RCC_AHB1Periph_DMA1
#define DMAX_CLK_CMD RCC_AHB1PeriphClockCmd
#define USARTx_TX_DMA_CHANNEL DMA_Channel_4
#define USARTx_TX_DMA_STREAM DMA1_Stream7
#define USARTx_RX_DMA_CHANNEL DMA_Channel_4
#define USARTx_RX_DMA_STREAM DMA1_Stream0
#define USARTx_DR_ADDRESS ((uint32_t) & USARTx->DR)

/* ============================================================== */

#elif defined MSP_UART6

/* 串口6 配置 */

/* 发送回显 */
#define TX_ECHO 1
/* 接收回显 */
#define RX_ECHO 1

/* 接收超时时间 ms */
#define TIMEOUT_RX 3

#define USARTx_TX_GPIO_PORT GPIOC
#define USARTx_TX_GPIO_CLK RCC_AHB1Periph_GPIOC
#define USARTx_TX_SOURCE GPIO_PinSource6
#define USARTx_TX_PIN GPIO_Pin_6

#define USARTx_RX_GPIO_PORT GPIOC
#define USARTx_RX_GPIO_CLK RCC_AHB1Periph_GPIOC
#define USARTx_RX_SOURCE GPIO_PinSource7
#define USARTx_RX_PIN GPIO_Pin_7

#define USARTx USART6
#define USARTx_TX_AF GPIO_AF_USART6
#define USARTx_RX_AF GPIO_AF_USART6
#define USARTx_CLK RCC_APB2Periph_USART6
#define USARTx_CLK_CMD RCC_APB2PeriphClockCmd
#define USARTx_IRQn USART6_IRQn
#define USARTx_IRQHandler USART6_IRQHandler
#define BAUD_RATE 19200

#define DMAx DMA2
#define DMAx_CLK RCC_AHB1Periph_DMA2
#define DMAX_CLK_CMD RCC_AHB1PeriphClockCmd
#define USARTx_TX_DMA_CHANNEL DMA_Channel_5
#define USARTx_TX_DMA_STREAM DMA2_Stream6
#define USARTx_RX_DMA_CHANNEL DMA_Channel_5
#define USARTx_RX_DMA_STREAM DMA2_Stream2
#define USARTx_DR_ADDRESS ((uint32_t) & USARTx->DR)

#endif

#endif /* __com_config_H */

/************************** END OF FILE **************************/
