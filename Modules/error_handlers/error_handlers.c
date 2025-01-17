/**
 ******************************************************************************
 * @file           : error_handlers.c
 * @author         : Jacek Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Handling all errors that are detected
 ******************************************************************************
 */

/* Includes -------------------------------------------------------------------*/

#include <stm32h7xx_hal.h>
#include "error_handlers/error_handlers.h"
#include "leds/leds.h"
#include "communication/communication.h"

#include "timers/timers.h"
#include "can/can.h"

/* Static variables -----------------------------------------------------------*/

static uint8_t ERROR_COUNT = 0;
static bool ERROR_ACTIVE = 0;
static Error_function ERROR_CURRENT = ErrorHandler_noErrorFunc;

/* Functions ------------------------------------------------------------------*/

static void critical_handle(Error_code error_code);

/**
 * @see  documentation in the header file (error_handlers.h)
 */
void Error_Handler(Error_function error_func, Error_code error_code) {
	__disable_irq();

	ERROR_ACTIVE = 1;

	if (ERROR_COUNT > 3) {
		critical_handle(error_code);
	}

	if (ERROR_CURRENT == error_func) {
		ERROR_COUNT++;
		return;
	}

	if (ERROR_CURRENT != ErrorHandler_noErrorFunc) {
		// we have two different errors
		critical_handle(error_code);
	}

	ERROR_CURRENT = error_func;

	if (error_func != COMErrorFunc_Bt)
		BT_sendData(MAINBOARD_ERROR_ID, (uint8_t*) &error_code, 1);

	if (error_func != COMErrorFunc_Eth)
		Eth_sendData(MAINBOARD_ERROR_ID, (uint8_t*) &error_code, 1);

	switch (error_func) {
	case ErrorHandler_noErrorFunc:
		ERROR_ACTIVE = 0;
		break;

	case TESTEErrorFunc:
		ERROR_ACTIVE = 0;
		break;

	case CAN1ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			CAN1_Init(); 
		}
		break;

	case CAN1ErrorFunc_transfer:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			CAN1_Init();
			CAN1_transfer();
		}
		break;

	case CAN1ErrorFunc_fifo:
		break;

	case CAN2ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			CAN2_Init();
		}
		break;

	case CAN2ErrorFunc_transfer:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			CAN2_Init();
			CAN2_transfer();
		}
		break;

	case CAN2ErrorFunc_fifo:
		break;

	case COMErrorFunc_watchdogInit:
		break;

	case COMErrorFunc_BtInit:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			BT_Init();
		}
		break;

	case COMErrorFunc_EthInit:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			Eth_Init();
		}
		break;

	case COMErrorFunc_Bt:
		ERROR_ACTIVE = 0;
		break;

	case COMErrorFunc_Eth:
		ERROR_ACTIVE = 0;
		break;

	case TIM4ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			TIM4_Init();
		}
		break;

	case TIM7ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			TIM7_Init();
		}
		break;

	case TIM16ErrorFunc_init:
		while (ERROR_ACTIVE) {
			ERROR_ACTIVE = 0;
			MX_TIM16_Init();
		}
		break;

	case SysClkErrorFunc_init:
		break;

	default:
		break;
	}

	if (ERROR_ACTIVE == 1) {
		critical_handle(error_code);
	}
	ERROR_COUNT = 0;
	ERROR_CURRENT = 0;

	__enable_irq();
}

/**
 ******************************************************************************
 * @brief	:	Reset or debug via leds in case of critical error  
 * 
 * @details	:	If DEBUG_LEDS is defined shows the error code whith the use of 
 * leds. Because we need more than 5 bits to display the error, we blink leds 
 * 1-3 for 100ms every second to display another 3 bits with led 1 beeng the 
 * least sugnificant. 
 *
 * If DEBUG_LEDS is not defined function resets the board
 ******************************************************************************
 */
static void critical_handle(Error_code error_code) {

#ifdef FEATURE_LED_ALIVE_DISPLAY
	extern bool alive_display_state;
	alive_display_state = 0;
#endif

#ifdef FEATURE_DEBUG_LEDS

	extern IWDG_HandleTypeDef hiwdg1;
	HAL_IWDG_Refresh(&hiwdg1);

	Leds_turnOff(LED_ALL);

	if (error_code & 0b00000001) {Leds_turnOn(LED_1);}
	if (error_code & 0b00000010) {Leds_turnOn(LED_2);}
	if (error_code & 0b00000100) {Leds_turnOn(LED_3);}
	if (error_code & 0b00001000) {Leds_turnOn(LED_4);}
	if (error_code & 0b00010000) {Leds_turnOn(LED_5);}

	uint16_t owerflow_leds = 0;
	if (error_code & 0b00100000) {owerflow_leds |= LED_1;}
	if (error_code & 0b01000000) {owerflow_leds |= LED_2;}
	if (error_code & 0b10000000) {owerflow_leds |= LED_3;}

	while (1) {
		HAL_IWDG_Refresh(&hiwdg1);
		HAL_Delay(1000);

		// blink leds for a short time to get more precision
		Leds_toggle(owerflow_leds);
		HAL_Delay(100);
		Leds_toggle(owerflow_leds);
	}

#else

	NVIC_SystemReset();

#endif
}
