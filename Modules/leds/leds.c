/**
 ******************************************************************************
 * @file           : leds.c
 * @author         : J. Prokopczuk
 * @brief          : Leds driver
 ******************************************************************************
 */

#include <stm32h7xx_hal_rcc.h>
#include <stm32h7xx_hal_gpio.h>

#include "leds.h"

void Leds_init(void)
{
	GPIO_InitTypeDef gpio;
	__HAL_RCC_GPIOG_CLK_ENABLE();
	gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_PULLDOWN;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &gpio);
}

void Leds_turnOnLed(Leds_ID ledId){
	switch(ledId){
	case LED1:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, 0x01u);
		break;
	case LED2:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, 0x01u);
		break;
	case LED3:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, 0x01u);
		break;
	case LED4:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, 0x01u);
		break;
	case LED5:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, 0x01u);
		break;
	}
}

void Leds_turnOffLed(Leds_ID ledId){
	switch(ledId){
	case LED1:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, 0u);
		break;
	case LED2:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, 0u);
		break;
	case LED3:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, 0u);
		break;
	case LED4:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, 0u);
		break;
	case LED5:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, 0u);
		break;
	}
}
