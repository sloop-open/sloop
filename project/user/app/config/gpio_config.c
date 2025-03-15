/**
 ******************************************************************************
 * @file    gpio_config
 * @author  暄暄
 * @date    2025-2-27
 * @brief   GPIO 配置
 * ==用户可在此文件上集中配置GPIO==
 *****************************************************************************/

#include "gpio_config.h"

#define _gpio_read(name) \
    case pin_##name:     \
        return GPIO_ReadInputDataBit(name##_port, name##_pin);

#define _gpio_write(name, bit)                                  \
    case pin_##name:                                            \
        GPIO_WriteBit(name##_port, name##_pin, (BitAction)bit); \
        break;

#define _gpio_H(name)                          \
    case pin_##name:                           \
        GPIO_SetBits(name##_port, name##_pin); \
        break;

#define _gpio_L(name)                            \
    case pin_##name:                             \
        GPIO_ResetBits(name##_port, name##_pin); \
        break;

#define _gpio_toggle(name)                        \
    case pin_##name:                              \
        GPIO_ToggleBits(name##_port, name##_pin); \
        break;

/* 读引脚电平 */
uint8_t gpio_read(char pin)
{
    switch (pin)
    {
        _gpio_read(sw1);
        _gpio_read(sw2);
        _gpio_read(sw3);
        _gpio_read(sw4);
        _gpio_read(beep);
        _gpio_read(i2cScl);
        _gpio_read(i2cSda);
        _gpio_read(runLed);

    default:
        return 0;
    }
}

/* 设置引脚电平 */
void gpio_write(char pin, char bit)
{
    switch (pin)
    {
        _gpio_write(sw1, bit);
        _gpio_write(sw2, bit);
        _gpio_write(sw3, bit);
        _gpio_write(sw4, bit);
        _gpio_write(beep, bit);
        _gpio_write(i2cScl, bit);
        _gpio_write(i2cSda, bit);
        _gpio_write(runLed, bit);

    default:
        break;
    }
}

/* 拉高引脚电平 */
void gpio_H(char pin)
{
    switch (pin)
    {
        _gpio_H(sw1);
        _gpio_H(sw2);
        _gpio_H(sw3);
        _gpio_H(sw4);
        _gpio_H(beep);
        _gpio_H(i2cScl);
        _gpio_H(i2cSda);
        _gpio_H(runLed);

    default:
        break;
    }
}

/* 拉低引脚电平 */
void gpio_L(char pin)
{
    switch (pin)
    {
        _gpio_L(sw1);
        _gpio_L(sw2);
        _gpio_L(sw3);
        _gpio_L(sw4);
        _gpio_L(beep);
        _gpio_L(i2cScl);
        _gpio_L(i2cSda);
        _gpio_L(runLed);

    default:
        break;
    }
}

/* 翻转引脚电平 */
void gpio_toggle(char pin)
{
    switch (pin)
    {
        _gpio_toggle(sw1);
        _gpio_toggle(sw2);
        _gpio_toggle(sw3);
        _gpio_toggle(sw4);
        _gpio_toggle(beep);
        _gpio_toggle(i2cScl);
        _gpio_toggle(i2cSda);
        _gpio_toggle(runLed);

    default:
        break;
    }
}

/* ============================================================== */

/* GPIO输出配置宏 */
#define gpio_out_cfg(name, OD_PP, MHz, PULL, bit)               \
    do                                                          \
    {                                                           \
        GPIO_StructInit(&GPIO_InitStructure);                   \
        GPIO_InitStructure.GPIO_Pin = name##_pin;               \
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;           \
        GPIO_InitStructure.GPIO_OType = OD_PP;                  \
        GPIO_InitStructure.GPIO_Speed = MHz;                    \
        GPIO_InitStructure.GPIO_PuPd = PULL;                    \
        GPIO_Init(name##_port, &GPIO_InitStructure);            \
        GPIO_WriteBit(name##_port, name##_pin, (BitAction)bit); \
    } while (0)

/* GPIO输入配置宏 */
#define gpio_in_cfg(name, PULL)                      \
    do                                               \
    {                                                \
        GPIO_StructInit(&GPIO_InitStructure);        \
        GPIO_InitStructure.GPIO_Pin = name##_pin;    \
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; \
        GPIO_InitStructure.GPIO_PuPd = PULL;         \
        GPIO_Init(name##_port, &GPIO_InitStructure); \
    } while (0)

/* GPIO模拟输入配置宏 */
#define gpio_an_cfg(name)                                \
    do                                                   \
    {                                                    \
        GPIO_StructInit(&GPIO_InitStructure);            \
        GPIO_InitStructure.GPIO_Pin = name##_pin;        \
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     \
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; \
        GPIO_Init(name##_port, &GPIO_InitStructure);     \
    } while (0)

/* ============================================================== */

/* GPIO 配置方法
第一步：在 gpio_config.h 定义 GPIO 别名
第二步：在下面配置 GPIO */

/* gpio 配置 */
void gpio_config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

    /* 蜂鸣器 */
    gpio_out_cfg(beep, GPIO_OType_PP, GPIO_Speed_2MHz, GPIO_PuPd_NOPULL, L);

    /* 运行指示灯 */
    gpio_out_cfg(runLed, GPIO_OType_OD, GPIO_Speed_2MHz, GPIO_PuPd_NOPULL, L);

    sys_prt_withFunc("gpio config ok");
}

#define _gpio_map(name) {"pin_" #name, pin_##name}

gpio_map_typ gpio_map[] = {
    _gpio_map(sw1),
    _gpio_map(sw2),
    _gpio_map(sw3),
    _gpio_map(sw4),
    _gpio_map(beep),
    _gpio_map(i2cScl),
    _gpio_map(i2cSda),
    _gpio_map(runLed),
};

#define msk(name) 0x01 << pin_##name

/* 输入回显屏蔽掩码 */
#define MSK (     \
    msk(beep) |   \
    msk(i2cScl) | \
    msk(i2cSda) | \
    msk(runLed))

#define _input(name) gpio_read(pin_##name) << pin_##name

/* GPIO 输入回显 */
void gpio_input_echo(void)
{
    static int input;

    int new =
        _input(sw1) |
        _input(sw2) |
        _input(sw3) |
        _input(sw4) |
        _input(beep) |
        _input(i2cScl) |
        _input(i2cSda) |
        _input(runLed);

    int trig = (input ^ new) & (~MSK);

    input = new;

    for (int i = 0; i < 32; i++)
    {
        if ((trig >> i) & 0x01)
        {
            sys_prt_withFunc("input trigger: %s, level: %c", gpio_map[i].str, (input >> i) & 0x01 ? 'H' : 'L');
        }
    }
}

/************************** END OF FILE **************************/
