/**
 ******************************************************************************
 * @file    asp_beep
 * @author  sloop
 * @date    2025-1-10
 * @brief   蜂鸣器驱动，支持多次鸣叫
 * ==此文件用户不应变更==
 *****************************************************************************/

#ifndef __asp_beep_H
#define __asp_beep_H

#include "sp_common.h"

/* 初始化 */
void asp_beep_init(void);

/* 单次鸣叫 */
void asp_beep(int ms);

/* 多次鸣叫 */
void asp_beep_multiple(int num, int interval, int continues);

#endif /* __asp_beep_H */

/************************** END OF FILE **************************/
