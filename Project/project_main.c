#include "can/can.h"
#include "project_main.h"
#include "leds/leds.h"
#include "uart/uart.h"
#include "main.h"
#include "camera/camera.h"

extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim4;
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

void project_main(void)
{

	CAM_setCamerasTimerPtr(&htim4);
	CAM_registerCallbacks();
	CAM_start();
	
	CAN_init();
	UART_init();

	UART_startRecive();
	CAN_startRecive();

	while(1)
	{

	}

}
