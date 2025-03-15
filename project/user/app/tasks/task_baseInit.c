/**
 ******************************************************************************
 * @file    task_baseInit
 * @author  暄暄
 * @date    2025-1-13
 * @brief   基础初始化
 *****************************************************************************/

#include "common.h"

/* 基础驱动初始化 */
void task_baseInit(void)
{
    _INIT; /* 初次进入任务时，执行一次 */

    /* 设置程序版本 */
    sys_set_version("sloop Demo V1.0.0");

    /* gpio 配置 */
    gpio_config();

    /* 运行灯初始化 */
    asp_run_led_init();

    /* 蜂鸣器初始化 */
    asp_beep_init();

    /* can1 初始化 */
    asp_can1_init();

    /* can2 初始化 */
    asp_can2_init();

    /* 串口1 初始化 */
    asp_uart1_init();

    /* 串口2 初始化 */
    asp_uart2_init();

    /* 串口3 初始化 */
    asp_uart3_init();

    /* 串口4 初始化 */
    asp_uart4_init();

    /* 串口5 初始化 */
    asp_uart5_init();

    /* 串口6 初始化 */
    asp_uart6_init();

    asp_beep(100);

    sys_wait(100);

    sys_goto(task_idle);

    _FREE; /* 任务结束，不再执行时，释放资源 */

    _RUN; /* 下方开始进入任务运行逻辑 */
}

/************************** END OF FILE **************************/
