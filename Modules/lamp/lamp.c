#include <stm32h7xx_hal.h>
#include <string.h>
#include <lamp/lamp.h>

uint8_t Counter_red = 0;
uint8_t Counter_blue = 0;
uint8_t Counter_green = 0;
uint8_t maxCounterRed ;
uint8_t maxCounterBlue ;
uint8_t maxCounterGreen ;

void  Set_Max_Values(uint8_t *data)
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
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM16)
  {
    __HAL_RCC_TIM16_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM16_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM16_IRQn);
   }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM16)
  {
    __HAL_RCC_TIM16_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(TIM16_IRQn);
  }
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
if(htim->Instance == TIM16)
{
    // Inkrementacja liczników po każdym przerwaniu timera
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
