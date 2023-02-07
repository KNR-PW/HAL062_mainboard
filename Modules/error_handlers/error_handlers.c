/**
 ******************************************************************************
 * @file           : main.c
 * @author         : J. Prokopczuk
 * @brief          : Main program body
 ******************************************************************************
 */

#include <stm32h7xx_hal.h>

#include "error_handlers/error_handlers.h"
#include "leds/leds.h"

void Error_Handler(void) {
	/// TODO: FInish Error handler!
	__disable_irq();

	Leds_init();
	Leds_turnOffAllLeds();
	Leds_turnOnLed(LED_4);
	while (1) {
	}
}
