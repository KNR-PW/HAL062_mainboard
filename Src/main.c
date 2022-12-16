/**
 ******************************************************************************
 * @file           : main.c
 * @author         : TODO: Add credits
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdbool.h>
#include "ethernet/eth.h"
#include "leds/leds.h"

void SysTick_Handler(void) {
	HAL_IncTick();
}

int main(void) {
	HAL_Init();

	Leds_init();
	Leds_welcomeFLash();

	Eth_Init();
	Eth_ReceiveData();

	/* Loop forever */
	while (1) {
		Leds_toggleLed(LED5);
		HAL_Delay(1000);
		Leds_toggleLed(LED5);
		HAL_Delay(1000);
	}
}
