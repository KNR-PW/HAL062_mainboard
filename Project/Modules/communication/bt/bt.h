#ifndef PROJECT_MODULES_COMMUNICATION_BT_H
#define PROJECT_MODULES_COMMUNICATION_BT_H


#include "stm32h7xx_hal.h"
#include "communication/data.h"

void Bt_setUartPtr(UART_HandleTypeDef* huart);
void Bt_setTimerPtr(TIM_HandleTypeDef* hTim);
void Bt_registerCallbacks(void);
void Bt_startTimer(void);

void Bt_startListening(void);
void Bt_addToTransmit(uint8_t* data);


#endif //PROJECT_MODULES_COMMUNICATION_ETH_H
