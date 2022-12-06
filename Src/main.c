/**
 ******************************************************************************
 * @file           : main.c
 * @author         : TODO: Add credits
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdbool.h>
#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include "ethernet/eth.h"
#include "leds/leds.h"


void SysTick_Handler(void)
{
	HAL_IncTick();
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	huart->gState = HAL_UART_STATE_READY;
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
