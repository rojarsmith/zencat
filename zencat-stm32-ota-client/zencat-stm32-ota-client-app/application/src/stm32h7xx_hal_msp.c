/**
 ******************************************************************************
 * @file         stm32f7xx_hal_msp.c
 * @brief        This file provides code for the MSP Initialization
 *               and de-Initialization codes.
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

/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void) {
	/* USER CODE BEGIN MspInit 0 */

	/* USER CODE END MspInit 0 */

	__HAL_RCC_SYSCFG_CLK_ENABLE();

	/* System interrupt init*/

	/* USER CODE BEGIN MspInit 1 */

	/* USER CODE END MspInit 1 */
}

/**
 * @brief CRC MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hcrc: CRC handle pointer
 * @retval None
 */
void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc) {
	if (hcrc->Instance == CRC) {
		/* USER CODE BEGIN CRC_MspInit 0 */

		/* USER CODE END CRC_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_CRC_CLK_ENABLE();
		/* USER CODE BEGIN CRC_MspInit 1 */

		/* USER CODE END CRC_MspInit 1 */
	}

}

/**
 * @brief UART MSP Initialization
 * This function configures the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	if (huart->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspInit 0 */

		/* USER CODE END USART1_MspInit 0 */

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
		PeriphClkInitStruct.Usart16ClockSelection =
				RCC_USART16CLKSOURCE_D2PCLK2;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
			Error_Handler();
		}

		/* Peripheral clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART1 GPIO Configuration
		 PB7     ------> USART1_RX
		 PA9     ------> USART1_TX
		 */
		GPIO_InitStruct.Pin = VCP_RX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(VCP_RX_GPIO_Port, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = VCP_TX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(VCP_TX_GPIO_Port, &GPIO_InitStruct);

		/* USER CODE BEGIN USART1_MspInit 1 */

		/* USER CODE END USART1_MspInit 1 */
	}

}

/**
 * @brief UART MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		/* USER CODE BEGIN USART1_MspDeInit 0 */

		/* USER CODE END USART1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		 PB7     ------> USART1_RX
		 PA9     ------> USART1_TX
		 */
		HAL_GPIO_DeInit(VCP_RX_GPIO_Port, VCP_RX_Pin);

		HAL_GPIO_DeInit(VCP_TX_GPIO_Port, VCP_TX_Pin);

		/* USER CODE BEGIN USART1_MspDeInit 1 */

		/* USER CODE END USART1_MspDeInit 1 */
	}

}

/**
 * @brief DSI MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hdsi: DSI handle pointer
 * @retval None
 */
void HAL_DSI_MspInit(DSI_HandleTypeDef *hdsi) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	if (hdsi->Instance == DSI) {
		/* USER CODE BEGIN DSI_MspInit 0 */

		/* USER CODE END DSI_MspInit 0 */

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_DSI;
		PeriphClkInitStruct.DsiClockSelection = RCC_DSICLKSOURCE_PHY;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
			Error_Handler();
		}

		/* Peripheral clock enable */
		__HAL_RCC_DSI_CLK_ENABLE();

		__HAL_RCC_GPIOJ_CLK_ENABLE();
		/**DSIHOST GPIO Configuration
		 DSI_D1P     ------> DSIHOST_D1P
		 DSI_D1N     ------> DSIHOST_D1N
		 DSI_CKP     ------> DSIHOST_CKP
		 DSI_CKN     ------> DSIHOST_CKN
		 DSI_D0P     ------> DSIHOST_D0P
		 DSI_D0N     ------> DSIHOST_D0N
		 PJ2     ------> DSIHOST_TE
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF13_DSI;
		HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

		/* DSI interrupt Init */
		HAL_NVIC_SetPriority(DSI_IRQn, 7, 0);
		HAL_NVIC_EnableIRQ(DSI_IRQn);
		/* USER CODE BEGIN DSI_MspInit 1 */

		/* USER CODE END DSI_MspInit 1 */
	}

}

/**
 * @brief DSI MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hdsi: DSI handle pointer
 * @retval None
 */
void HAL_DSI_MspDeInit(DSI_HandleTypeDef *hdsi) {
	if (hdsi->Instance == DSI) {
		/* USER CODE BEGIN DSI_MspDeInit 0 */

		/* USER CODE END DSI_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_DSI_CLK_DISABLE();

		/**DSIHOST GPIO Configuration
		 DSI_D1P     ------> DSIHOST_D1P
		 DSI_D1N     ------> DSIHOST_D1N
		 DSI_CKP     ------> DSIHOST_CKP
		 DSI_CKN     ------> DSIHOST_CKN
		 DSI_D0P     ------> DSIHOST_D0P
		 DSI_D0N     ------> DSIHOST_D0N
		 PJ2     ------> DSIHOST_TE
		 */
		HAL_GPIO_DeInit(GPIOJ, GPIO_PIN_2);

		/* DSI interrupt DeInit */
		HAL_NVIC_DisableIRQ(DSI_IRQn);
		/* USER CODE BEGIN DSI_MspDeInit 1 */

		/* USER CODE END DSI_MspDeInit 1 */
	}

}
