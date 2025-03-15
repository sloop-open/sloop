/**
 ******************************************************************************
 * @file    asp_i2c
 * @author  暄暄
 * @date    2020-2-20
 * @brief   提供GPIO模拟I2C驱动
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __asp_i2c_H
#define __asp_i2c_H

#include "sp_common.h"

#define ADDR_IS_8b 1
#define ADDR_IS_16b 0

/* i2c初始化 */
void asp_i2c_init(void);

/* 校验从节点 */
uint8_t asp_i2c_check(uint8_t dev);

/* 主节点写数据 */
uint8_t asp_i2c_write_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b,
                            uint8_t *buf, uint16_t len);

/* 主节点读数据 */
uint8_t asp_i2c_read_bytes(uint8_t dev, uint16_t addr, uint8_t addr_is_8b,
                           uint8_t *buf, uint16_t len);

/* 主节点写单字节数据 */
uint8_t asp_i2c_write_byte(uint8_t dev, uint16_t addr, uint8_t byte);

/* 主节点读单字节数据 */
uint8_t asp_i2c_read_byte(uint8_t dev, uint16_t addr);

#endif /* __asp_i2c_H */

/*********************************** END OF FILE ***********************************/
