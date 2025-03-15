/**
 ******************************************************************************
 * @file    SysTick/SysTick_Example/stm32f2xx_it.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    18-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
 ******************************************************************************
 * @attention
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include "common.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
 * @{
 */

/** @addtogroup SysTick_Example
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief   This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
  sys_error("A NMI_IRQ occurred.");

  while (1)
  {
  }
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
  sys_error("A HardFault_IRQ occurred.");
  sys_error("It may be that the function pointer is not initialized, resulting in NULL pointer dereference.");

  /*
  下面的这些地址是从栈中解析出的疑似出错前的函数调用关系。
  可以借助这些地址，定位到故障代码。
  1.打开mdk调试模式，在 Disassembly 窗口，右键选择 “Show Disassembly at Address...”.
  2.输入地址进行代码的定位，有些地址定位不到就忽略，直到可以看到有效的 C代码位置
  3.检查是否有未初始化的 NULL 函数指针调用。
  */

  /* 获取栈顶指针 */
  uint32_t msp = __get_MSP();

  uint32_t *stack = (uint32_t *)msp;

  sys_prt_brYellow("The following addresses are the function call relationships before suspected error parsed from the stack.");

  /* 按深度检索栈 */
  int len = 64;

  /* 打印调用关系，值和 080x xxxx 匹配，认为是函数地址 */
  for (int i = 0, count = 0; i < len; i++)
  {
    if ((stack[i] & 0xFFF00000) == FLASH_BASE)
    {
      sys_prt_brWhite("%d: 0x%08x", ++count, stack[i]);
    }
  }

  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
  sys_error("A MemManage_IRQ occurred.");

  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
  sys_error("A BusFault_IRQ occurred.");

  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
  sys_error("A UsageFault occurred.");

  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
  sys_error("A SVC_IRQ occurred.");

  while (1)
  {
  }
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
  sys_error("A DebugMon occurred.");

  while (1)
  {
  }
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
  sys_error("A PendSV_IRQ occurred.");

  while (1)
  {
  }
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
