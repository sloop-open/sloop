/**
 ******************************************************************************
 * @file    asp_beep
 * @author  sloop
 * @date    2025-1-10
 * @brief   蜂鸣器驱动，支持多次鸣叫
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_beep.h"
#include "gpio_config.h"

void beep_on(void);
void beep_off(void);
void beep_once(void);

/* 初始化 */
void asp_beep_init(void)
{
    gpio_L(pin_beep);

    sl_prt_withFunc("init: beep");
}

/* 单次鸣叫 */
void asp_beep(int ms)
{
    beep_on();

    sl_timeout_start(ms, beep_off);
}

static int _continues;

/* 多次鸣叫 */
void asp_beep_multiple(int num, int interval, int continues)
{
    /* 断言 interval > continues */
    if (interval <= continues)
    {
        sl_error("The interval time should be greater than the continues. interval: %d, continues: %d", interval, continues);

        return;
    }

    _continues = continues;

    sl_multiple_start(num, interval, beep_once);
}

void beep_on(void)
{
    gpio_H(pin_beep);
}

void beep_off(void)
{
    gpio_L(pin_beep);
}

void beep_once(void)
{
    asp_beep(_continues);
}

/************************** END OF FILE **************************/
