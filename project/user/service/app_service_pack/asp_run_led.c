/**
 ******************************************************************************
 * @file    asp_run_led
 * @author  暄暄
 * @date    2025-1-10
 * @brief   运行指示灯驱动，每秒闪一下，可用于判断是否死机
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_run_led.h"
#include "gpio_config.h"

/* 指示灯闪烁任务 */
void run_led_task(void);

/* 初始化 */
void asp_run_led_init(void)
{
    sys_cycle_start(500, run_led_task);

    sys_prt_withFunc("init: run led 1Hz");
}

/* 指示灯闪烁任务 */
void run_led_task(void)
{
    gpio_toggle(pin_runLed);
}

/************************** END OF FILE **************************/
