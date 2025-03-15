/**
 ******************************************************************************
 * @file    msp_flash
 * @author  暄暄
 * @date    2024-10-21
 * @brief   mcu flash 驱动，用于支撑参数脱机保存
 * ==此文件用户不应变更==
 *****************************************************************************/

#include "msp_flash.h"

/* 读数据 */
void msp_flash_read(uint32_t addr, uint8_t *buf, uint32_t len)
{
    memcpy((void *)buf, (void *)addr, len);
}

/* 写数据 */
void msp_flash_write(uint32_t addr, uint8_t *buf, uint32_t len)
{
    FLASH_Unlock();

    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    /* 擦除一页 */
    FLASH_EraseSector(FLASH_Sector_7, VoltageRange_3);

    uint8_t *_addr = (uint8_t *)addr;

    /* 写入数据 */
    for (int i = 0; i < len; i++)
    {
        FLASH_ProgramByte((uint32_t)(_addr++), *(buf++));
    }

    FLASH_Lock();
}

/************************** END OF FILE **************************/
