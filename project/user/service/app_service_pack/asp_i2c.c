/**
 ******************************************************************************
 * @file    asp_i2c
 * @author  暄暄
 * @date    2020-2-20
 * @brief   提供GPIO模拟I2C驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_i2c.h"
#include "gpio_config.h"

/* 写控制位 */
#define I2C_W 0
/* 读控制位 */
#define I2C_R 1

/**** 引脚控制宏 ****/
#define I2C_SCL_H() gpio_H(pin_i2cScl)
#define I2C_SCL_L() gpio_L(pin_i2cScl)

#define I2C_SDA_H() gpio_H(pin_i2cSda)
#define I2C_SDA_L() gpio_L(pin_i2cSda)

#define I2C_SDA_READ() gpio_read(pin_i2cSda)

void i2c_delay(void);
void i2c_start(void);
void i2c_stop(void);
void send_byte(uint8_t byte);
uint8_t read_byte(void);
uint8_t wait_ack(void);
void send_ack(void);
void send_nack(void);

/* i2c初始化 */
void asp_i2c_init(void)
{
    /* 释放总线 */
    i2c_stop();
}

void i2c_delay(void)
{
    /* 这里循环数填主频（单位MHz）即可 */
    for (int i = 0; i < 120; i++)
        __NOP();
}

/* 主节点发起的I2C启动信号 */
void i2c_start(void)
{
    I2C_SDA_H();
    i2c_delay();

    I2C_SCL_H();
    i2c_delay();

    I2C_SDA_L();
    i2c_delay();
}

/* 主节点发起的I2C停止信号 */
void i2c_stop(void)
{
    I2C_SDA_L();
    i2c_delay();

    I2C_SCL_H();
    i2c_delay();

    I2C_SDA_H();
    i2c_delay();
}

/* 发送一个字节 */
void send_byte(uint8_t byte)
{
    for (uint32_t i = 0; i < 8; i++)
    {
        I2C_SCL_L();

        if (byte & 0x80)
        {
            I2C_SDA_H();
        }
        else
        {
            I2C_SDA_L();
        }
        i2c_delay();

        I2C_SCL_H();
        i2c_delay();

        byte <<= 1;
    }
}

/* 读取一个字节 */
uint8_t read_byte(void)
{
    uint8_t byte = 0;

    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();

    for (uint32_t i = 0; i < 8; i++)
    {
        byte <<= 1;

        I2C_SCL_L();
        i2c_delay();

        I2C_SCL_H();
        i2c_delay();

        if (I2C_SDA_READ())
        {
            byte |= 0x01;
        }
    }

    return byte;
}

/* 等待从节点ack */
uint8_t wait_ack(void)
{
    uint8_t ack = 0;

    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();

    I2C_SCL_H();
    i2c_delay();

    ack = I2C_SDA_READ();

    I2C_SCL_L();
    i2c_delay();

    return ack;
}

/* 向从节点发送ack */
void send_ack(void)
{
    I2C_SCL_L();

    I2C_SDA_L();
    i2c_delay();

    I2C_SCL_H();
    i2c_delay();

    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();
}

/* 向从节点发送nack */
void send_nack(void)
{
    I2C_SCL_L();

    I2C_SDA_H();
    i2c_delay();

    I2C_SCL_H();
    i2c_delay();

    I2C_SCL_L();
    I2C_SDA_H();
    i2c_delay();
}

/* 校验从节点
返回0表示设备正常 */
uint8_t asp_i2c_check(uint8_t dev)
{
    uint8_t ack = 0;

    /* I2C启动 */
    i2c_start();

    /* 发送地址 */
    send_byte(dev | I2C_W);

    /* 获取ack */
    ack = wait_ack();

    /* I2C停止 */
    i2c_stop();

    return ack;
}

/* 主节点写数据 */
uint8_t asp_i2c_write_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b,
                            uint8_t *buf, uint16_t len)
{
    uint8_t err = 0;

    /* I2C启动 */
    i2c_start();

    /* 发送设备地址 */
    send_byte(dev | I2C_W);

    /* 等待ack */
    if (wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }

    /* 如果寄存器是8位地址 */
    if (addr_is_8b)
    {
        /* 发送寄存器地址 */
        send_byte(addr);
    }
    else
    /* 如果寄存器是16位地址 */
    {
        /* 先发高8位地址 */
        send_byte(addr >> 8);

        /* 等待ack */
        if (wait_ack())
        {
            err = 1;
            goto LABEL_END;
        }

        /* 再发低8位地址 */
        send_byte(addr);
    }

    /* 等待ack */
    if (wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }

    /* 发送数据 */
    for (int i = 0; i < len; i++)
    {
        /* 发送单个字节 */
        send_byte(*(buf + i));

        /* 等待ack */
        if (wait_ack())
        {
            err = 1;
            goto LABEL_END;
        }
    }

LABEL_END:
    /* I2C停止 */
    i2c_stop();

    return err;
}

/* 主节点读数据 */
uint8_t asp_i2c_read_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b,
                           uint8_t *buf, uint16_t len)
{
    uint8_t err = 0;

    /* I2C启动 */
    i2c_start();

    /* 发送设备地址 */
    send_byte(dev | I2C_W);

    /* 等待ack */
    if (wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }

    /* 如果寄存器是8位地址 */
    if (addr_is_8b)
    {
        /* 发送寄存器地址 */
        send_byte(addr);
    }
    else
    /* 如果寄存器是16位地址 */
    {
        /* 先发高8位地址 */
        send_byte(addr >> 8);

        /* 等待ack */
        if (wait_ack())
        {
            err = 1;
            goto LABEL_END;
        }

        /* 再发低8位地址 */
        send_byte(addr);
    }

    /* 等待ack */
    if (wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }

    /* I2C启动 */
    i2c_start();

    /* 切换成主节点读取模式 */
    send_byte(dev | I2C_R);

    /* 等待ack */
    if (wait_ack())
    {
        err = 1;
        goto LABEL_END;
    }

    /* 读取数据 */
    for (int i = 0; i < (len - 1); i++)
    {
        /* 读取单个字节 */
        *(buf + i) = read_byte();

        /* 发送ack */
        send_ack();
    }

    /* 读取最后一个字节 */
    *(buf + len - 1) = read_byte();
    /* 发送nack，结束读取 */
    send_nack();

LABEL_END:
    /* I2C停止 */
    i2c_stop();

    return err;
}

/* 主节点写单字节数据 */
uint8_t asp_i2c_write_byte(uint8_t dev, uint16_t addr, uint8_t byte)
{
    return asp_i2c_write_bytes(dev, addr, ADDR_IS_8b, &byte, 1);
}

/* 主节点读单字节数据 */
uint8_t asp_i2c_read_byte(uint8_t dev, uint16_t addr)
{
    uint8_t byte;

    asp_i2c_read_bytes(dev, addr, ADDR_IS_8b, &byte, 1);

    return byte;
}

/*********************************** END OF FILE ***********************************/
