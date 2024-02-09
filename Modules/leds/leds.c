/**
 ******************************************************************************
 * @file           : leds.c
 * @author         : J. Prokopczuk
 * @brief          : Leds driver
 ******************************************************************************
 */

#include <stm32h7xx_hal.h> // Simple HAL delay
#include <stm32h7xx_hal_rcc.h>
#include <stm32h7xx_hal_gpio.h>
#include <stm32h7xx_hal.h>

#include "leds/leds.h"

void Leds_init(void) {
	GPIO_InitTypeDef gpio;
	__HAL_RCC_GPIOG_CLK_ENABLE();
	gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_PULLDOWN;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &gpio);
}

void Leds_turnOnLed(Leds_ID ledId) {
	switch (ledId) {
	case LED1:
		HAL_GPIO_WritePin(LEDS_PORT, GPIO_PIN_2, 0x01u);
		break;
	case LED2:
		HAL_GPIO_WritePin(LEDS_PORT, GPIO_PIN_3, 0x01u);
		break;
	case LED3:
		HAL_GPIO_WritePin(LEDS_PORT, GPIO_PIN_4, 0x01u);
		break;
	case LED4:
		HAL_GPIO_WritePin(LEDS_PORT, GPIO_PIN_5, 0x01u);
		break;
	case LED5:
		HAL_GPIO_WritePin(LEDS_PORT, GPIO_PIN_6, 0x01u);
		break;
	}
}

void Leds_turnOffLed(Leds_ID ledId) {
	switch (ledId) {
	case LED1:
		HAL_GPIO_WritePin(LEDS_PORT, GPIO_PIN_2, 0u);
		break;
	case LED2:
		HAL_GPIO_WritePin(LEDS_PORT, GPIO_PIN_3, 0u);
		break;
	case LED3:
		HAL_GPIO_WritePin(LEDS_PORT, GPIO_PIN_4, 0u);
		break;
	case LED4:
		HAL_GPIO_WritePin(LEDS_PORT, GPIO_PIN_5, 0u);
		break;
	case LED5:
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, 0u);
		break;
	}
}

void Leds_turnOffAllLeds(void) {
	uint32_t reg = LED_1 | LED_2 | LED_3 | LED_4 | LED_5;
	HAL_GPIO_WritePin(LEDS_PORT, reg, 0u);
}

void Leds_toggleLed(Leds_ID ledId) {
	switch (ledId) {
	case LED1:
		HAL_GPIO_TogglePin(LEDS_PORT, GPIO_PIN_2);
		break;
	case LED2:
		HAL_GPIO_TogglePin(LEDS_PORT, GPIO_PIN_3);
		break;
	case LED3:
		HAL_GPIO_TogglePin(LEDS_PORT, GPIO_PIN_4);
		break;
	case LED4:
		HAL_GPIO_TogglePin(LEDS_PORT, GPIO_PIN_5);
		break;
	case LED5:
		HAL_GPIO_TogglePin(LEDS_PORT, GPIO_PIN_6);
		break;
	}
}

void Leds_welcomeFLash(void) {
	Leds_turnOnLed(LED1);
	Leds_turnOnLed(LED2);
	Leds_turnOnLed(LED2);
	Leds_turnOnLed(LED3);
	Leds_turnOnLed(LED4);
	Leds_turnOnLed(LED5);
	HAL_Delay(1000);
	Leds_turnOffLed(LED1);
	Leds_turnOffLed(LED2);
	Leds_turnOffLed(LED2);
	Leds_turnOffLed(LED3);
	Leds_turnOffLed(LED4);
	Leds_turnOffLed(LED5);
	HAL_Delay(1000);
}
