/**
 ******************************************************************************
 * @file    stm32h7xx_it.h
 * @author  Rojar Smith
 * @brief   This file contains the headers of the interrupt handlers for
 *          Cortex-M7.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Rojar Smith.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32H7xx_IT_H
#define __STM32H7xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
//void TIM6_DAC_IRQHandler(void);
void LTDC_IRQHandler(void);
void DMA2D_IRQHandler(void);
void JPEG_IRQHandler(void);
void MDMA_IRQHandler(void);
void DSI_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32H7xx_IT_H */

