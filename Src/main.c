/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include "ethernet/eth.h"


void SysTick_Handler(void)
{
	HAL_IncTick();
}

int main(void)
{
	//Ethernet module initialization
	static UART_HandleTypeDef huart_eth;
	static GPIO_InitTypeDef gpio_eth;
	eth_init(huart_eth, gpio_eth);


	//test for working initialized uart
	uint8_t signal = 0xAB;

	//Infinite loop
	while(1)
	{
		HAL_UART_Transmit(&huart_eth, &signal, 1, 1000);
		HAL_Delay(1000);
	}
}
