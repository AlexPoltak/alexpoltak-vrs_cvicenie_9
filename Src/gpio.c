/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Pinout Configuration
*/
void MX_GPIO_Init(void)
{
/*configuration pins for display*/
	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	  /* GPIOA reset */
	  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4
			  |LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_11|LL_GPIO_PIN_12);

	  /* GPIOB reset */
	  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_3|LL_GPIO_PIN_4|LL_GPIO_PIN_5);

	  /* GPIOA init */
	  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4
			  |LL_GPIO_PIN_7|LL_GPIO_PIN_8|LL_GPIO_PIN_11|LL_GPIO_PIN_12;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /* GPIOB init */
	  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_4|LL_GPIO_PIN_5;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);


/*configuration button PB3*/
     /*EXTI configuration*/
	  NVIC_SetPriority(EXTI3_IRQn, 2);
	  NVIC_EnableIRQ(EXTI3_IRQn);
	 /*set EXTI source PB3*/
	  SYSCFG->EXTICR[1] &= ~(0xFU << 0U);
	  SYSCFG->EXTICR[1] |= (0x1 << 0U);
	 //Enable interrupt from EXTI line 3
	  EXTI->IMR |= EXTI_IMR_MR3;
	 //Set EXTI trigger to falling edge
	  EXTI->RTSR &= ~(EXTI_IMR_MR3);
	  EXTI->FTSR |= EXTI_IMR_MR3;
	 /*GPIO configuration button, PB3*/
	  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	  GPIOB->MODER &= ~(GPIO_MODER_MODER3);
	  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR3);
	  GPIOB->PUPDR |= GPIO_PUPDR_PUPDR3_0;
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
