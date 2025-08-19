/**
 ******************************************************************************
 * @file           : error_handlers.c
 * @author         : Jacek Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Handling all errors that is detected TODO
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------- */

#include <stm32h7xx_hal.h>
#include "errorHandlers.h"
#include "leds/leds.h"


/* Functions ------------------------------------------------------------------- */


/**
 ******************************************************************************
 * @details			:	Error handling - turn off leds and while loop
 ******************************************************************************
 */
void errorHandler(void) {
	/// TODO: FInish Error handler!
	__disable_irq();

	LED_TURN_OFF(LED_ALL);
	LED_TURN_ON(LED_4);
	while (1) {
	}
}
