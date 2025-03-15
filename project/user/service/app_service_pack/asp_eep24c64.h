/**
 ******************************************************************************
 * @file    asp_eep24c64
 * @author  暄暄
 * @date    2020-2-20
 * @brief   提供eep24c64驱动，eep24c64是一款i2c 的EEPROM，8KB。
 *****************************************************************************/

#ifndef __asp_eep24c64_H
#define __asp_eep24c64_H

#include "sp_common.h"

/* ============================================================== */
/* ==用户配置区 开始== */

/* 起始地址 */
#define eep24c64_BASE 0x0000
/* 存储器容量 */
#define eep24c64_SIZE 0x2000

/* ==用户配置区 结束== */
/* ============================================================== */

/* eep24c64初始化 */
void asp_eep24c64_init(void);

/* 校验与eep24c64通讯是否正常，返回0表示设备正常 */
uint8_t asp_eep24c64_check(void);

/* 写入数据，不支持跨页写入，
即写入数据需在1paga以内（32Bytes） */
void asp_eep24c64_write_bytes(uint16_t addr, uint8_t *buf, uint16_t len);

/* 读取数据 */
void asp_eep24c64_read_bytes(uint16_t addr, uint8_t *buf, uint16_t len);

/* 擦除一页 */
void asp_eep24c64_erase_page(uint16_t addr);

#endif /* __asp_eep24c64_H */

/*********************************** END OF FILE ***********************************/
