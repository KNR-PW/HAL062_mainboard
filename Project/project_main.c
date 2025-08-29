#include "can/can.h"
#include "project_main.h"
#include "leds/leds.h"
#include "uart/uart.h"
#include "main.h"
#include "camera/camera.h"
#include "watchdog/watchdog.h"

extern TIM_HandleTypeDef htim4;
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

void project_main(void)
{
	WATCHDOG_init();
	CAM_setCamerasTimerPtr(&htim4);
	CAM_registerCallbacks();
	CAM_start();
	
	CAN_init();
	UART_init();

	UART_startRecive();
	CAN_startRecive();

	LED_TURN_ON(LED_ALL);
	HAL_Delay(1000);
	LED_TURN_OFF(LED_ALL);

	while(1)
	{

	}
}
