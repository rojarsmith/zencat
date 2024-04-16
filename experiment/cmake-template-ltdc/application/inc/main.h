/**
 ******************************************************************************
 * @file    BSP/CM7/Inc/main.h
 * @author  MCD Application Team
 * @brief   Header for main.c module for Cortex-M7.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_lcd.h"
//#include "stm32h747xx.h"
//#include "stm32h7xx_hal_cortex.h"
#include "stm32h747i_discovery_sdram.h"

/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define VCP_TX_Pin GPIO_PIN_9
#define VCP_TX_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_7
#define VCP_RX_GPIO_Port GPIOB

#endif /* __MAIN_H */
