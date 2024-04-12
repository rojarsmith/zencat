/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Rojar Smith
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 .
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "main.h"

//#define RCC_BASE    0x40023800 //H7 0x58024400
//#define RCC_CFGR    (volatile uint32_t*)(RCC_BASE + 0x10)
//
//#define HSI_FREQUENCY 64000000 //H747I-DISCO 64000000  F746G-DISCO 16000000
//
//#define SYSTICK_BASE       SysTick_BASE //H747I-DISCO 0xE000E010
//#define SYSTICK_CTRL_PTR   (volatile uint32_t*)(SYSTICK_BASE + 0x00) // 0x00
//#define SYSTICK_LOAD_PTR   (volatile uint32_t*)(SYSTICK_BASE + 0x04) // 0x04
//#define SYSTICK_VAL_PTR    (volatile uint32_t*)(SYSTICK_BASE + 0x08) // 0x08
//
//#define SYSTICK_COUNTFLAG  SysTick_CTRL_COUNTFLAG_Msk //H747I-DISCO (1 << 16)
//#define SYSTICK_CLKSOURCE  SysTick_CTRL_CLKSOURCE_Msk //H747I-DISCO (1 << 2)
//#define SYSTICK_TICKINT    SysTick_CTRL_TICKINT_Msk //H747I-DISCO (1 << 1)
//#define SYSTICK_ENABLE     SysTick_CTRL_ENABLE_Msk //H747I-DISCO (1 << 0)
//
//uint32_t sysClockFreq = 0;

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void Delay_MS(uint32_t ms);

int main(void) {
	/* System Init, System clock, voltage scaling and L1-Cache configuration are
	 done by CPU1 (Cortex-M7)
	 in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep)
	 */

	/* Configure the MPU attributes as Write Through */
	MPU_Config();

	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	/* STM32H7xx HAL library initialization:
	 - Systick timer is configured by default as source of time base, but user
	 can eventually implement his proper time base source (a general purpose
	 timer for example or other time source), keeping in mind that Time base
	 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
	 handled in milliseconds basis.
	 - Set NVIC Group Priority to 4
	 - Low Level Initialization
	 */
	HAL_Init();

	/* Loop forever */
	for (;;) {
		Delay_MS(5000);
		Delay_MS(5000);
	}
}

void Delay_MS(uint32_t ms) {
	uint32_t tickstart = HAL_GetTick();
	while ((HAL_GetTick() - tickstart) < ms) {
		// Empty loop or low power mode
	}
}

/**
 * @brief  Configure the MPU attributes as Write Through for SDRAM.
 * @note   The Base Address is SDRAM_DEVICE_ADDR.
 *         The Region Size is 32MB.
 * @param  None
 * @retval None
 */
static void MPU_Config(void) {
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU as Strongly ordered for not defined regions */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x00;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x87;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU attributes as WT for SDRAM */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = SDRAM_DEVICE_ADDR;
	MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Configure the MPU QSPI flash */
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = 0x90000000;
	MPU_InitStruct.Size = MPU_REGION_SIZE_128MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER2;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x0;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void) {
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}
