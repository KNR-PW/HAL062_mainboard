#include <stm32h7xx_hal.h>
#include <string.h>
#include <lamp/lamp.h>
#include "error_handlers/error_handlers.h"

uint8_t Counter_red = 0;
uint8_t Counter_blue = 0;
uint8_t Counter_green = 0;
uint8_t maxCounterRed = 0;
uint8_t maxCounterBlue = 0;
uint8_t maxCounterGreen = 0;

void  Set_Max_Value(uint8_t *data)
{ maxCounterRed = data[3];
  maxCounterBlue = data[4];
  maxCounterGreen = data[5];
}

void GPIO_Init() {


    // Inicjalizacja pinów diod
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN | LED3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);


}
void MX_TIM16_Init(void)
{

  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 7199;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 99;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_Base_Start_IT(&htim16);
}



void handleLED(uint8_t *data) {
    // Sprawdzenie czy dane spełniają warunki i włączenie odpowiedniej diody
    if (data[0] == 1) {
        HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_SET);
    }
    else
    {
    	HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_RESET);
    }
    if (data[1] == 1) {
        HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_SET);
    }
    else
        {
        	HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_RESET);
        }
    if (data[2] == 1) {


        HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_SET);
    }
    else
            {
            	HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_RESET);
            }
}


