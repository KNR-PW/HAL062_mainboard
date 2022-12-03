/**
 ******************************************************************************
 * @file           : main.c
 * @author         : TODO: Add credits
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include "leds/leds.h"

// static UART_HandleTypeDef huart3;
void SysTick_Handler(void)
{
	HAL_IncTick();
}

int main(void)
{
	HAL_Init();

	Leds_init();
	Leds_welcomeFLash();

    /* Loop forever */
	while(1)
	{
		Leds_toggleLed(LED5);
		HAL_Delay(1000);
		Leds_toggleLed(LED5);
		HAL_Delay(1000);
	}
}
