/**
 ******************************************************************************
 * @file    msp_can
 * @author  暄暄
 * @date    2024-10-21
 * @brief   can 总线驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "msp_can2.h"

#define MSP_CAN2

#include "com_config.h"

/* 接收处理 */
static void can_rx_process(void);

static CanTxMsg TxMessage;
static CanRxMsg RxMessage;

void msp_can_init(void)
{
    CAN_InitTypeDef CAN_InitStructure = {0};
    CAN_FilterInitTypeDef CAN_FilterInitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_AHB1PeriphClockCmd(CANx_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(CANx_CLK, ENABLE);

    GPIO_PinAFConfig(CANx_GPIO_PORT, CANx_RX_SOURCE, CANx_AF_PORT);
    GPIO_PinAFConfig(CANx_GPIO_PORT, CANx_TX_SOURCE, CANx_AF_PORT);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = CANx_RX_PIN | CANx_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CANx_GPIO_PORT, &GPIO_InitStructure);

    CAN_StructInit(&CAN_InitStructure);
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CANx_Mode;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
    CAN_InitStructure.CAN_Prescaler = 4;
    CAN_Init(CANx, &CAN_InitStructure);

    CAN_FilterInitStructure.CAN_FilterNumber = CANx_CAN_FilterNumber;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = RX_ID1 << 5;
    CAN_FilterInitStructure.CAN_FilterIdLow = RX_ID2 << 5;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = RX_ID3 << 5;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = RX_ID4 << 5;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CANx_FilterFIFOAssignment;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CANx, CANx_IT, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = CANx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = PRIO_DEFAULT;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void CANx_IRQHandler(void)
{
    CAN_Receive(CANx, CANx_FIFONumber, &RxMessage);

    sys_task_once(can_rx_process);
}

/* 接收回调 */
__weak void msp_can_rx_callback(CanRxMsg *msg) {}

/* 接收处理 */
void can_rx_process(void)
{
#if RX_ECHO

    static int count;

    char buff[9] = {0};

    int len = RxMessage.DLC;

    uint32_t id = RxMessage.StdId;

    count++;

    sys_prt_var(count);

    memcpy(buff, RxMessage.Data, len);

    sys_prt_noNewLine(RTT_CTRL_TEXT_BRIGHT_YELLOW "\ncan rx len: %d, rx id: %d, data: ", len, id);

    for (int i = 0; i < len; i++)
        sys_prt_noNewLine(RTT_CTRL_TEXT_BRIGHT_YELLOW "%02x ", buff[i]);

    sys_prt_noNewLine("\n");

    sys_prt_withFunc(RTT_CTRL_TEXT_BRIGHT_YELLOW "can rx: %s", buff);

#endif

    msp_can_rx_callback(&RxMessage);
}

/* 发送数据 */
void msp_can_tx(CanTxMsg *msg)
{
    TxMessage = *msg;

#if TX_ECHO

    static int count;

    char buff[9] = {0};

    int len = TxMessage.DLC;

    int id = TxMessage.StdId;

    count++;

    sys_prt_var(count);

    memcpy(buff, TxMessage.Data, len);

    sys_prt_noNewLine(RTT_CTRL_TEXT_BRIGHT_WHITE "\ncan tx len: %d, tx id: %d, data: ", len, id);

    for (int i = 0; i < len; i++)
        sys_prt_noNewLine(RTT_CTRL_TEXT_BRIGHT_WHITE "%02x ", buff[i]);

    sys_prt_noNewLine("\n");

    sys_prt_withFunc(RTT_CTRL_TEXT_BRIGHT_WHITE "can tx: %s", buff);

#endif

    CAN_Transmit(CANx, &TxMessage);
}

/************************** END OF FILE **************************/
