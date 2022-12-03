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
//	__HAL_RCC_GPIOD_CLK_ENABLE();
//
//	GPIO_InitTypeDef gpio;
//	gpio.Pin = GPIO_PIN_2;
//	gpio.Mode = GPIO_MODE_AF_PP;
//	gpio.Pull = GPIO_NOPULL;
//	gpio.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOG, &gpio);
//
//	__HAL_RCC_USART3_CLK_ENABLE();
//
//	huart3.Instance = USART3;
//	huart3.Init.BaudRate = 115200;
//	huart3.Init.WordLength = UART_WORDLENGTH_8B;
//	huart3.Init.Parity = UART_PARITY_NONE;
//	huart3.Init.StopBits = UART_STOPBITS_1;
//	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
//	huart3.Init.Mode = UART_MODE_TX_RX;
//
//	HAL_UART_Init(&huart3);
//	uint8_t signal = 0xAB;
	Leds_init();
	Leds_welcomeFLash();

    /* Loop forever */
	while(1)
	{
		Leds_turnOnLed(LED1);
		HAL_Delay(1000);
		Leds_turnOffLed(LED1);
		HAL_Delay(1000);
	}
}
