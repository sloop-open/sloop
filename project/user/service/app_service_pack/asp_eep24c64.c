/**
 ******************************************************************************
 * @file    asp_eep24c64
 * @author  暄暄
 * @date    2020-2-20
 * @brief   提供eep24c64驱动，eep24c64是一款i2c 的EEPROM，8KB。
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "asp_i2c.h"
#include "asp_eep24c64.h"

/* eep24c64设备地址 */
#define DEV 0xA0

#define ADDR_IS_16b 0

static uint32_t tick_start;

/* eep24c64初始化 */
void asp_eep24c64_init(void)
{
    /* i2c初始化 */
    asp_i2c_init();

    if (asp_eep24c64_check() != 0)
    {
        sys_error("eep err");
    }

    /* 满足写入 cycle 要求 */
    tick_start = sys_get_tick();

    sys_wait(5);
}

/* 校验与eep24c64通讯是否正常，返回0表示设备正常 */
uint8_t asp_eep24c64_check(void)
{
    return asp_i2c_check(DEV);
}

/* 写入数据 */
void asp_eep24c64_write_bytes(uint16_t addr, uint8_t *buf, uint16_t len)
{
    /* 检查长度 */
    if (len > 32)
    {
        sys_error("eep write len too long (> 32)");

        return;
    }

    /* 检查写入周期，芯片写入周期是5ms,写入太频繁抛出错误 */
    if ((sys_get_tick() - tick_start) <= 5)
    {
        sys_error("eep write cycle < 5ms");

        return;
    }

    /* 主节点写数据 */
    if (asp_i2c_write_bytes(DEV, addr, ADDR_IS_16b, buf, len))
    {
        sys_error("eep write err");

        return;
    }

    tick_start = sys_get_tick();
}

/* 读取数据 */
void asp_eep24c64_read_bytes(uint16_t addr, uint8_t *buf, uint16_t len)
{
    /* 检查写入周期，写入周期5ms之后可以读 */
    if ((sys_get_tick() - tick_start) <= 5)
    {
        sys_error("eep read after write < 5ms");

        return;
    }

    /* 主节点读数据 */
    if (asp_i2c_read_bytes(DEV, addr, ADDR_IS_16b, buf, len))
    {
        sys_error("eep read err");

        return;
    }
}

/* 擦除一页 */
void asp_eep24c64_erase_page(uint16_t addr)
{
    /* 检查写入周期，芯片写入周期是5ms,写入太频繁抛出错误 */
    if ((sys_get_tick() - tick_start) <= 5)
    {
        sys_error("eep write cycle < 5ms");

        return;
    }

    uint8_t buf[32] = {0};

    /* 主节点写数据 */
    if (asp_i2c_write_bytes(DEV, addr, ADDR_IS_16b, buf, 32))
    {
        sys_error("eep write err");

        return;
    }

    tick_start = sys_get_tick();
}

/*********************************** END OF FILE ***********************************/
