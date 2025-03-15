/**
 ******************************************************************************
 * @file    msp_flash
 * @author  暄暄
 * @date    2024-10-21
 * @brief   mcu flash 驱动，用于支撑参数脱机保存
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __msp_flash_H
#define __msp_flash_H

#include "sp_common.h"

/* 参数起始位置 384k */
#define FLASH_PARAM_BASE (FLASH_BASE + 0x60000)

/* 读数据 */
void msp_flash_read(uint32_t addr, uint8_t *buf, uint32_t len);

/* 写数据 */
void msp_flash_write(uint32_t addr, uint8_t *buf, uint32_t len);

#endif /* __msp_flash_H */

/************************** END OF FILE **************************/
