/**
 ******************************************************************************
 * @file           : error_handlers.c
 * @author         : Jacek Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Handling all errors that is detected TODO
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------- */

#include "cube_interface.h"
#include <stm32h7xx_hal.h>
#include "error_handlers.h"
#include "leds.h"


/* Functions ------------------------------------------------------------------- */


/**
 ******************************************************************************
 * @details			:	Error handling - turn off leds and while loop
 ******************************************************************************
 */
void Cube_Error_Handler(void) {
	/// TODO: FInish Error handler!
	__disable_irq();

	Leds_init();
	Leds_turnOffAllLeds();
	Leds_turnOnLed(LED_4);
	while (1) {
	}
}
