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
#include "timers.h"
#include "lamp/lamp.h"
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim4;
extern struct commands uartCommands;
extern MessageTypeDef UART_MessageRecieved;
int cam_bridge[3] = {0,0,0};
extern uint8_t Counter_red;
extern uint8_t Counter_blue;
extern uint8_t Counter_green;
extern uint8_t maxCounterRed ;
extern uint8_t maxCounterBlue ;
extern uint8_t maxCounterGreen ;

void Camera_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CAM_STER3_Pin|CAM_STER2_Pin|CAM_STER1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = CAM_STER3_Pin|CAM_STER2_Pin|CAM_STER1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

void TIM4_Init()
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};


  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 80;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 20000;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
	Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
	Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim4) != HAL_OK)
  {
	Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
	Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 1500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
	Error_Handler();
  }
  sConfigOC.Pulse = 1000;
  if (HAL_TIM_OC_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
	Error_Handler();
  }
  sConfigOC.Pulse = 2000;
  if (HAL_TIM_OC_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
	Error_Handler();
  }
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_2);
  HAL_TIM_OC_Start_IT(&htim4, TIM_CHANNEL_3);

}

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


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM7){
		if(uartCommands.numberOfCommands > 0){
			--uartCommands.numberOfCommands;
		}
		return;
	}
	else if(htim->Instance == TIM4)
	{
    HAL_GPIO_WritePin(CAM_STER1_GPIO_Port, CAM_STER1_Pin, 1);
    HAL_GPIO_WritePin(CAM_STER2_GPIO_Port, CAM_STER2_Pin, 1);
    HAL_GPIO_WritePin(CAM_STER3_GPIO_Port, CAM_STER3_Pin, 1);
	}
	else if(htim->Instance == TIM16)
	{
		Counter_red++;
		    Counter_blue++;
		    Counter_green++;

		    // Sprawdzenie, czy osiągnięto wartości maksymalne
		    if (Counter_red == maxCounterRed) {
		        // Zmiana stanu diody czerwonej
		        HAL_GPIO_TogglePin(LED_PORT, LED1_PIN);
		        // Resetowanie licznika
		        Counter_red = 0;
		    }
		    if (Counter_blue == maxCounterBlue) {
		        // Zmiana stanu diody niebieskiej
		        HAL_GPIO_TogglePin(LED_PORT, LED2_PIN);
		        // Resetowanie licznika
		        Counter_blue = 0;
		    }
		    if (Counter_green == maxCounterGreen) {
		        // Zmiana stanu diody zielonej
		        HAL_GPIO_TogglePin(LED_PORT, LED3_PIN);
		        // Resetowanie licznika
		        Counter_green = 0;
		    }
	}
 }


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim4) {
    switch (HAL_TIM_GetActiveChannel(&htim4)) {
    case HAL_TIM_ACTIVE_CHANNEL_1:
    	if (cam_bridge[0] == 0) {
    		HAL_GPIO_WritePin(CAM_STER1_GPIO_Port, CAM_STER1_Pin, 0);
    	}
    	if (cam_bridge[1] == 0) {
    		HAL_GPIO_WritePin(CAM_STER2_GPIO_Port, CAM_STER2_Pin, 0);
    	}
    	if (cam_bridge[2] == 0) {
    		HAL_GPIO_WritePin(CAM_STER3_GPIO_Port, CAM_STER3_Pin, 0);
    	}
      break;
    case HAL_TIM_ACTIVE_CHANNEL_2:
    	if (cam_bridge[0] == 1) {
			HAL_GPIO_WritePin(CAM_STER1_GPIO_Port, CAM_STER1_Pin, 0);
		}
		if (cam_bridge[1] == 1) {
			HAL_GPIO_WritePin(CAM_STER2_GPIO_Port, CAM_STER2_Pin, 0);
		}
		if (cam_bridge[2] == 1) {
			HAL_GPIO_WritePin(CAM_STER3_GPIO_Port, CAM_STER3_Pin, 0);
		}
      break;
    case HAL_TIM_ACTIVE_CHANNEL_3:
    	if (cam_bridge[0] == 2) {
			HAL_GPIO_WritePin(CAM_STER1_GPIO_Port, CAM_STER1_Pin, 0);
		}
		if (cam_bridge[1] == 2) {
			HAL_GPIO_WritePin(CAM_STER2_GPIO_Port, CAM_STER2_Pin, 0);
		}
		if (cam_bridge[2] == 2) {
			HAL_GPIO_WritePin(CAM_STER3_GPIO_Port, CAM_STER3_Pin, 0);
		}
      break;
    default:
      break;
    }

  }
}

