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
#include <stdbool.h>
#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include "ethernet/eth.h"



void SysTick_Handler(void)
{
	HAL_IncTick();
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	huart->gState = HAL_UART_STATE_READY;
}

int main(void)
{
	//Ethernet module initialization
	Eth_init(USART3);


	//Infinite loop

	while(1)
	{
		char id[2] = "71";
		char msg[16] = "UPxxxxxxxxxxxxxx";
		Eth_sendData(id, msg);
		HAL_Delay(1000);
	}
}
