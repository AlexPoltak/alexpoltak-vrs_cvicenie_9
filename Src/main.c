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
#include "gpio.h"
#include "tim.h"
#include "display.h"
#include <string.h>

#include "i2c.h"
#include "../vlhkomer/hts221.h"
#include "../accelerometer/lsm6ds0.h"
#include <math.h>
#include "../tlakomer/lps25hb.h"

void SystemClock_Config(void);

uint8_t checkButtonState(GPIO_TypeDef* PORT, uint8_t PIN);
uint8_t length(uint8_t *);
extern uint64_t disp_time;
uint64_t saved_time;
double num_to_display = 10;
float pressure,humidity,temperature,temperature1,altitude;
static uint8_t button_state=0;
char temp_s[10],hum_s[10],press_s[10],alt_s[10];

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

	  MX_TIM2_Init();

	  lps25hb_init();
	  hts221_init();

	  uint8_t message[] = "tEMP";
	  uint8_t lengthOfMessage = length(message);


	  while (1)
	  {
		  lps25hb_get_pressure(&pressure);
		  hts221_get_humidity(&humidity);
		  lps25hb_get_temperature(&temperature1);
		  lps25hb_get_altitude(&altitude);
		  if(temperature1 < -99.9){
			  temperature1 = -99.9;
		  	  }
		  else if(temperature1 > 99.9){
			  temperature1 = 99.9;
		  	  }
		  if(humidity < 0){
			  humidity = 0;
		  	  }
		  else if(humidity > 99){
			  humidity = 99;
		  	  }
		  if(pressure < -9999.99){
			  pressure = -9999.99;
		  	  }
		  else if(pressure > 9999.99){
			  pressure = 9999.99;
		  	  }
		  if(altitude < -9999.9){
			  altitude = -9999.9;
		  	  }
		  else if(altitude > 9999.9){
			  altitude = 9999.9;
		  	  }

		  if(button_state == 0){

		  		  char temp[] = "tMP_";
		  		  memset(message,0,10);

		  		  sprintf(temp_s, "%.1f", temperature1);
		  		  strcat(temp, temp_s);

		  		  memcpy (message, temp, strlen(temp));
		  		  lengthOfMessage=length(message);
		  	  }
		  if(button_state == 1){
		  		  char hum[] = "HUM_";
		  		  memset(message,0,10);

		  		  sprintf(hum_s, "%.0f", humidity);
		  		  strcat(hum, hum_s);

		  		  memcpy (message, hum, strlen(hum));
		  		  lengthOfMessage=length(message);
		  	  }
		  if(button_state == 2){
		  		  char bar[] = "bAr_";
		  		  memset(message,0,10);

		  		  sprintf(press_s, "%.2f", pressure);
		  		  strcat(bar, press_s);

		  		  memcpy (message, bar, strlen(bar));
		  		  lengthOfMessage=length(message);
		  	  }
		  if(button_state == 3){
		  		  char alt[] = "ALt_";
		  		memset(message,0,10);

		  		  sprintf(alt_s, "%.1f", altitude);
		  		  strcat(alt, alt_s);

		  		  memcpy (message, alt, strlen(alt));
		  		  lengthOfMessage=length(message);
		  	  }


		  if(disp_time > (saved_time + 1000))
		  {
		  	  saved_time = disp_time;
	  	  	  fillBufferForDisplay(message, lengthOfMessage);

		  }
	  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
uint8_t length(uint8_t *str) {
	uint8_t i = 0;

	while (str[i] != '\0') {
		i++;
	}
	return i;
}

uint8_t checkButtonState(GPIO_TypeDef* PORT, uint8_t PIN)
{
	  //type your code for "checkButtonState" implementation here:

		button_state++;

		if(button_state > 3){
			button_state = 0;
		}
		return 1;

}

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

