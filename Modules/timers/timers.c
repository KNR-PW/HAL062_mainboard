/**
 ******************************************************************************
 * @file           : timers.c
 * @author         : Krystian Czechowicz
 * @brief          : Timer to handle communication
 ******************************************************************************
 */

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_tim.h>
#include <string.h>
#include "error_handlers/error_handlers.h"
#include "communication/communication.h"
#include "can/can.h"

TIM_HandleTypeDef htim7;
extern struct commands uartCommands;
extern MessageTypeDef UART_MessageRecieved;

void TIM7_Init()
{

  TIM_MasterConfigTypeDef sMasterConfig = {0};


  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 704-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_Base_Start_IT(&htim7);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM7){
		if(uartCommands.numberOfCommands > 0){
			--uartCommands.numberOfCommands;
		}
		return;
	}


}

