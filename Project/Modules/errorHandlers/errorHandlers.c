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

// usage - warn(__FILE__, __LINE__, 0);
// tip: use " #line __LINE__ "Modules/MODULE/MODULE.c" " to avoid exposing the whole path
void warn(char *filename, uint32_t line, uint32_t info) {
	LED_TURN_ON(LED_2);
}


// usage - error(__FILE__, __LINE__, 0);
// tip: use " #line __LINE__ "Modules/MODULE/MODULE.c" " to avoid exposing the whole path
void error(char *filename, uint32_t line, uint32_t info) {
	__disable_irq();

	LED_TURN_OFF(LED_ALL);
	LED_TURN_ON(LED_1);
	while (1) {
	}
}
