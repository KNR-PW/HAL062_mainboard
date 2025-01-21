/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Krystian Czechowicz, Adam Rybojad
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdbool.h>

#include "main.h"
#include "init_interface.h"

#include "leds.h"
#include "can.h"
#include "communication.h"
#include "error_handlers.h"
#include "timers.h"


/**
 * *******************************************************************************
 * @brief	:	Main function contains all required initializations
 * *******************************************************************************
*/
int main(void) {

	__HAL_RCC_GPIOB_CLK_ENABLE(); //< TODO check if it is required

	HAL_Init(); //< HAL library initialization
	Init_SystemClock_Config(); //< Configuration of system clock (AUTOGEN)

//	Leds_init(); //< Initialization of LEDs pins
	Init_MX_GPIO_Init();
	Leds_welcomeFLash(); //< All LEDs flash (test LEDs performance)

	FDCAN1_Init(); //< Initialization of CAN1 - rail (motorboards, sensorboards)
	FDCAN2_Init(); //< Initialization of CAN 2 - manip/labo

	Eth_Init(); //< Initialization of UART ethernet
	Eth_ReceiveData(); //< Starting listening on ethernet uart pins

	BT_Init(); //< Initialization of UART bluetooth
	BT_ReceiveData(); //< Starting listening on bluetooth uart pins

	Watchdog_Init(); //< Starting watchdog to prevent communication lost

	Camera_Init();
	TIM4_Init();
	/* Loop forever */
	while (1) {
	}
}
