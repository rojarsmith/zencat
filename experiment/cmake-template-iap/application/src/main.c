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

#include "main.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

/* Private function prototypes -----------------------------------------------*/
static void MPU_Config(void);

//#define RCC_BASE    0x58024400
#define RCC_CFGR    (volatile uint32_t*)(RCC_BASE + 0x10)

#define HSI_FREQUENCY 64000000

#define SYSTICK_BASE       0xE000E010
#define SYSTICK_CTRL_PTR   (volatile uint32_t*)(SYSTICK_BASE + 0x00)
#define SYSTICK_LOAD_PTR   (volatile uint32_t*)(SYSTICK_BASE + 0x04)
#define SYSTICK_VAL_PTR    (volatile uint32_t*)(SYSTICK_BASE + 0x08)

#define SYSTICK_COUNTFLAG  (1 << 16)
#define SYSTICK_CLKSOURCE  (1 << 2)
#define SYSTICK_TICKINT    (1 << 1)
#define SYSTICK_ENABLE     (1 << 0)

uint32_t sysClockFreq = 0;

void SysTick_Delay_us(uint32_t us) {
	uint32_t tmp = 0;

	uint32_t dddd = *SYSTICK_CTRL_PTR;
	*SYSTICK_CTRL_PTR |= SYSTICK_CLKSOURCE; // Internal
	uint32_t dddd2 = *SYSTICK_CTRL_PTR;
	*SYSTICK_LOAD_PTR = sysClockFreq / 1000 * us;
	uint32_t dddd3 = *SYSTICK_LOAD_PTR;
	*SYSTICK_VAL_PTR = 0;
	*SYSTICK_CTRL_PTR |= SYSTICK_ENABLE;

	do {
		tmp = *SYSTICK_CTRL_PTR;
	} while (!(tmp & SYSTICK_COUNTFLAG));

	*SYSTICK_CTRL_PTR &= ~SYSTICK_ENABLE;

	if (tmp || dddd || dddd2 || dddd3) {
	}
}

int main(void) {
	MPU_Config();

	uint32_t sysClockSource = (*RCC_CFGR >> 3) & 0x3;

	switch (sysClockSource) {
	case 0: // HSI
		sysClockFreq = HSI_FREQUENCY;
		break;
	}

	if (sysClockSource || sysClockFreq) {
	}

	// 10 seconds
	for (int i = 1; i <= 100; i++) {
		SysTick_Delay_us(100);
	}

	volatile uint32_t *value1 = (volatile uint32_t*) SYSTICK_BASE;
	if (value1) {

	}

	/* Loop forever */
	for (;;)
		;
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
