#include <stm32h7xx_hal.h>
#include <string.h>
#include <lamp/lamp.h>

void GPIO_Init() {


    // Inicjalizacja pinów diod
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED1_PIN | LED2_PIN | LED3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);


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
