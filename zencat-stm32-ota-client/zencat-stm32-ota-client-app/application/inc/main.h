/**
 ******************************************************************************
 * @file    main.h
 * @author  Rojar Smith
 * @brief   Header for main.c module for Cortex-M7.
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
#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_bus.h"
#include "stm32h747i_discovery_qspi.h"
#include "stm32h747i_discovery_sdram.h"
#include "stm32h747i_discovery_errno.h"
#include "otm8009a.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "app_touchgfx.h"

/* Exported functions ------------------------------------------------------- */
void Error_Handler(void);

extern int32_t DSI_IO_Write(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData,
		uint16_t Size);
extern int32_t DSI_IO_Read(uint16_t ChannelNbr, uint16_t Reg, uint8_t *pData,
		uint16_t Size);

/* Private defines -----------------------------------------------------------*/
#define VCP_TX_Pin GPIO_PIN_9
#define VCP_TX_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_10
#define VCP_RX_GPIO_Port GPIOA

#define LCD_BL_Pin GPIO_PIN_12
#define LCD_BL_GPIO_Port GPIOJ
#define LCD_RESET_Pin GPIO_PIN_3
#define LCD_RESET_GPIO_Port GPIOG
#define MCU_ACTIVE_Pin GPIO_PIN_8
#define MCU_ACTIVE_GPIO_Port GPIOF
#define FRAME_RATE_Pin GPIO_PIN_9
#define FRAME_RATE_GPIO_Port GPIOJ
#define RENDER_TIME_Pin GPIO_PIN_8
#define RENDER_TIME_GPIO_Port GPIOJ
#define VSYNC_FREQ_Pin GPIO_PIN_3
#define VSYNC_FREQ_GPIO_Port GPIOJ

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
