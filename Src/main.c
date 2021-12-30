/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "tim.h"
#include "display.h"

#include "lis3mdltr.h"
#include "lsm6ds0.h"
#include "lps25hb.h"
#include "hts221.h"
#include <string.h>
#include <math.h>

void SystemClock_Config(void);
uint8_t length(uint8_t *);
extern uint64_t disp_time;
uint64_t saved_time;
double num_to_display = 10;
float pressure,humidity,temperature,temperature1,altitude;
uint8_t buttonState=0;
char temp_s[4],hum_s[2],press_s[6],alt_s[5];

int main(void)
{

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();

  setSegments();
  setDigits();

  LL_mDelay(2000);

  resetDigits();
  resetSegments();

  MX_TIM3_Init();

  lsm6ds0_init();
  lps25hb_init();
  hts221_init();

  uint8_t message[] = "TEMP";
  uint8_t lengthOfMessage = length(message);
  while (1)
  {
	  lps25hb_get_pressure(&pressure);
	  hts221_get_humidity(&humidity);
	  hts221_get_temperature(&temperature);
	  lps25hb_get_temperature(&temperature1);
	  lps25hb_get_altitude(&altitude);

	  if(buttonState == 0){

	  		  char temp[] = "TEMP_";

	  		  sprintf(temp_s, "%.1f", temperature);
	  		  strcat(temp, temp_s);
	  		  memcpy (message, temp, sizeof(temp));
	  		  lengthOfMessage=length(message);
	  	  }
	  else if(buttonState == 1){
	  		  char hum[] = "HUM_";

	  		  sprintf(hum_s, "%.0f", humidity);
	  		  strcat(hum, hum_s);
	  		  memcpy (message, hum, sizeof(hum));
	  		  lengthOfMessage=length(message);
	  	  }
	  else if(buttonState == 2){
	  		  char bar[] = "BAR_";

	  		  sprintf(press_s, "%.2f", pressure);
	  		  strcat(bar, press_s);
	  		  memcpy (message, bar, sizeof(bar));
	  		  lengthOfMessage=length(message);
	  	  }
	  else if(buttonState == 3){
	  		  char alt[] = "ALT_";

	  		  sprintf(alt_s, "%.1f", altitude);
	  		  strcat(alt, alt_s);
	  		  memcpy (message, alt, sizeof(alt));
	  		  lengthOfMessage=length(message);
	  	  }


	  if(disp_time > (saved_time + 1000))
	  {
	  	  saved_time = disp_time;
  	  	  fillBufferForDisplay(message, lengthOfMessage);

	  }
  }

}
uint8_t length(uint8_t *str) {
	uint8_t i = 0;

	while (str[i] != '\0') {
		i++;
	}
	return i;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SYSTICK_EnableIT();
  LL_SetSystemCoreClock(8000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
