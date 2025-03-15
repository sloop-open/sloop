/**
 ******************************************************************************
 * @file    gpio_config
 * @author  暄暄
 * @date    2025-2-27
 * @brief   GPIO 配置
 * ==此文件用户可配置 gpio 别名==
 *****************************************************************************/

#ifndef __gpio_config_H
#define __gpio_config_H

#include "sp_common.h"

/* gpio 名称字符串最大长度 */
#define GPIO_STR_MAX 32

typedef struct
{
    /* 字符串 */
    char str[GPIO_STR_MAX];

    /* 枚举值 */
    char _enum;

} gpio_map_typ;

enum
{
    L = 0,
    H
};

extern gpio_map_typ gpio_map[];

/* gpio 配置 */
void gpio_config(void);

/* 读引脚电平 */
uint8_t gpio_read(char pin);

/* 设置引脚电平，例如 gpio_write(pin_beep, H); */
void gpio_write(char pin, char bit);

/* 拉高引脚电平 */
void gpio_H(char pin);

/* 拉低引脚电平 */
void gpio_L(char pin);

/* 翻转引脚电平 */
void gpio_toggle(char pin);

/* GPIO 输入回显 */
void gpio_input_echo(void);

/* ============================================================== */

/* GPIO 配置方法
第一步：在下面定义 GPIO 别名
第二步：在 gpio_config.c 配置 GPIO */

/* 以下是 GPIO 别名 */

enum
{

/* 拨码sw1 */
#define sw1_port GPIOE
#define sw1_pin GPIO_Pin_0
    pin_sw1,

#define sw2_port GPIOE
#define sw2_pin GPIO_Pin_1
    pin_sw2,

#define sw3_port GPIOE
#define sw3_pin GPIO_Pin_2
    pin_sw3,

#define sw4_port GPIOE
#define sw4_pin GPIO_Pin_3
    pin_sw4,

/* 蜂鸣器 */
#define beep_port GPIOE
#define beep_pin GPIO_Pin_14
    pin_beep,

/* I2C 引脚配置 */
#define i2cScl_port GPIOA
#define i2cScl_pin GPIO_Pin_11
    pin_i2cScl,

#define i2cSda_port GPIOA
#define i2cSda_pin GPIO_Pin_12
    pin_i2cSda,

/* 运行指示灯 */
#define runLed_port GPIOB
#define runLed_pin GPIO_Pin_4
    pin_runLed,

    PIN_NUM,

};

#endif /* __gpio_config_H */

/************************** END OF FILE **************************/
