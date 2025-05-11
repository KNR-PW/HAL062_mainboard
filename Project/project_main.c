#include "communication/can/can.h"
#include "project_main.h"
#include "leds/leds.h"
#include "communication/eth/eth.h"
#include "communication/bt/bt.h"
#include "main.h"
#include "camera/camera.h"

extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

void project_main(void)
{

	Leds_welcomeFLash();

	CAM_setCamerasTimerPtr(&htim4);
	CAM_registerCallbacks();
	CAM_start();

	Eth_setTimerPtr(&htim17);
	Eth_setUartPtr(&huart1);
	Eth_registerCallbacks();
	Eth_startTimer();

	Bt_setTimerPtr(&htim16);
	Bt_setUartPtr(&huart3);
	Bt_registerCallbacks();
	Bt_startTimer();

	CAN_setTimerPtr(&htim14);
	CAN_setExtCanPtr(&hfdcan1);
	CAN_setRailCanPtr(&hfdcan2);
	CAN_registerCallbacks();
	CAN_extCanInit();
	CAN_railCanInit();

	Eth_startListening();
	Bt_startListening();
	CAN_railCanStartListening();
	CAN_extCanStartListening();
	while(1)
	{

	}

}
