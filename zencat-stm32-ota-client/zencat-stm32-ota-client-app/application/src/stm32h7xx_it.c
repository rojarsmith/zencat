/**
 ******************************************************************************
 * @file    stm32h7xx_it.c
 * @author  Rojar Smith
 * @brief   Main Interrupt Service Routines for Cortex-M7.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"

/** @addtogroup STM32H7xx_HAL_Examples
 * @{
 */

/** @addtogroup DMAMUX_SYNC
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern DMA2D_HandleTypeDef hdma2d;
extern DSI_HandleTypeDef hdsi;
extern MDMA_HandleTypeDef hmdma_jpeg_infifo_th;
extern MDMA_HandleTypeDef hmdma_jpeg_outfifo_th;
extern JPEG_HandleTypeDef hjpeg;
extern LTDC_HandleTypeDef hltdc;
extern TIM_HandleTypeDef htim6;

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void) {
	/* USER CODE BEGIN NonMaskableInt_IRQn 0 */

	/* USER CODE END NonMaskableInt_IRQn 0 */
	/* USER CODE BEGIN NonMaskableInt_IRQn 1 */
	while (1) {
	}
	/* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void) {
	/* USER CODE BEGIN HardFault_IRQn 0 */

	/* USER CODE END HardFault_IRQn 0 */
	while (1) {
		/* USER CODE BEGIN W1_HardFault_IRQn 0 */
		/* USER CODE END W1_HardFault_IRQn 0 */
	}
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void) {
	/* USER CODE BEGIN MemoryManagement_IRQn 0 */

	/* USER CODE END MemoryManagement_IRQn 0 */
	while (1) {
		/* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
		/* USER CODE END W1_MemoryManagement_IRQn 0 */
	}
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void) {
	/* USER CODE BEGIN BusFault_IRQn 0 */

	/* USER CODE END BusFault_IRQn 0 */
	while (1) {
		/* USER CODE BEGIN W1_BusFault_IRQn 0 */
		/* USER CODE END W1_BusFault_IRQn 0 */
	}
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void) {
	/* USER CODE BEGIN UsageFault_IRQn 0 */

	/* USER CODE END UsageFault_IRQn 0 */
	while (1) {
		/* USER CODE BEGIN W1_UsageFault_IRQn 0 */
		/* USER CODE END W1_UsageFault_IRQn 0 */
	}
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void) {
	/* USER CODE BEGIN DebugMonitor_IRQn 0 */

	/* USER CODE END DebugMonitor_IRQn 0 */
	/* USER CODE BEGIN DebugMonitor_IRQn 1 */

	/* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles TIM6 global interrupt, DAC1_CH1 and DAC1_CH2 underrun error interrupts.
 */
//void TIM6_DAC_IRQHandler(void) {
//	/* USER CODE BEGIN TIM6_DAC_IRQn 0 */
//
//	/* USER CODE END TIM6_DAC_IRQn 0 */
//	HAL_TIM_IRQHandler(&htim6);
//	/* USER CODE BEGIN TIM6_DAC_IRQn 1 */
//
//	/* USER CODE END TIM6_DAC_IRQn 1 */
//}

/**
 * @brief This function handles LTDC global interrupt.
 */
void LTDC_IRQHandler(void) {
	/* USER CODE BEGIN LTDC_IRQn 0 */

	/* USER CODE END LTDC_IRQn 0 */
	HAL_LTDC_IRQHandler(&hltdc);
	/* USER CODE BEGIN LTDC_IRQn 1 */

	/* USER CODE END LTDC_IRQn 1 */
}

/**
 * @brief This function handles DMA2D global interrupt.
 */
void DMA2D_IRQHandler(void) {
	/* USER CODE BEGIN DMA2D_IRQn 0 */

	/* USER CODE END DMA2D_IRQn 0 */
	HAL_DMA2D_IRQHandler(&hdma2d);
	/* USER CODE BEGIN DMA2D_IRQn 1 */

	/* USER CODE END DMA2D_IRQn 1 */
}

/**
 * @brief This function handles JPEG global interrupt.
 */
void JPEG_IRQHandler(void) {
	/* USER CODE BEGIN JPEG_IRQn 0 */

	/* USER CODE END JPEG_IRQn 0 */
	HAL_JPEG_IRQHandler(&hjpeg);
	/* USER CODE BEGIN JPEG_IRQn 1 */

	/* USER CODE END JPEG_IRQn 1 */
}

/**
 * @brief This function handles MDMA global interrupt.
 */
void MDMA_IRQHandler(void) {
	/* USER CODE BEGIN MDMA_IRQn 0 */

	/* USER CODE END MDMA_IRQn 0 */
	HAL_MDMA_IRQHandler(&hmdma_jpeg_outfifo_th);
	HAL_MDMA_IRQHandler(&hmdma_jpeg_infifo_th);
	/* USER CODE BEGIN MDMA_IRQn 1 */

	/* USER CODE END MDMA_IRQn 1 */
}

/**
 * @brief This function handles DSI global Interrupt.
 */
void DSI_IRQHandler(void) {
	/* USER CODE BEGIN DSI_IRQn 0 */

	/* USER CODE END DSI_IRQn 0 */
	HAL_DSI_IRQHandler(&hdsi);
	/* USER CODE BEGIN DSI_IRQn 1 */

	/* USER CODE END DSI_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
 * @}
 */

/**
 * @}
 */

