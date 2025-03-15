/**
 ******************************************************************************
 * @file    mcu_base_timer
 * @author  xuan
 * @date    2024-12-16
 * @brief   提供 MCU 基础定时器
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "mcu_base_timer.h"

void IWDG_init(void);
weak_define(mcu_tick_irq);
void tim100ns_init(void);

/* MCU 基础定时器初始化 */
void mcu_base_timer_init(void)
{
    /* HSE 时钟配置 */
    RCC_DeInit();

    SystemInit();

    SystemCoreClockUpdate();

    sys_prt_withFunc("SYSCLK: %d MHz", SystemCoreClock / 1000000);

#if SYS_WDG_ENABLE

    /* 配置看门狗 超时时间 1s */
    IWDG_init();

    sys_printf("watch dog start");

#endif

    /* NVIC分组,16级响应优先级，不支持抢占，有助于提升系统性能 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    SysTick_Config(SystemCoreClock / 1000);

    /* 优先级最高，保证 tick 实时更新 */
    NVIC_SetPriority(SysTick_IRQn, PRIO_HIGHEST);

    tim100ns_init();
}

void SysTick_Handler(void)
{
    mcu_tick_irq();
}

#if SYS_WDG_ENABLE

void IWDG_init(void)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    IWDG_SetPrescaler(IWDG_Prescaler_32);

    IWDG_SetReload(1000 - 1);

    IWDG_ReloadCounter();

    IWDG_Enable();
}

/* 喂狗 */
void mcu_reload_wdg(void)
{
    IWDG_ReloadCounter();
}

#endif

void tim100ns_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = (6 - 1);
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    TIM_Cmd(TIM7, ENABLE);
}

/* 获取 100ns 分辨率时间戳 */
uint16_t mcu_get_100nsRes(void)
{
    return TIM_GetCounter(TIM7);
}

/*********************************** END OF FILE ***********************************/
