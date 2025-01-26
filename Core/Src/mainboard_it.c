#include <stm32h7xx_hal.h>
#include <stm32h7xx_it.h>
#include <stdbool.h>

#include "cube_interface.h"

#include "leds.h"
#include "can.h"
#include "communication.h"


void Cube_SysTick_Handler(void) {
	static int work_led_cnt = 0;
	static bool work_led_state = false;

	if (work_led_state && work_led_cnt >= 100) {
		work_led_cnt = 0u;
		work_led_state = false;
		Leds_turnOffLed(LED1);
	}
	if (!work_led_state && work_led_cnt >= 400) {
		work_led_cnt = 0u;
		work_led_state = true;
		Leds_turnOnLed(LED1);
	}
	work_led_cnt++;
}
