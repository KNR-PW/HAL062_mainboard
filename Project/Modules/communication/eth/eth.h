#ifndef PROJECT_MODULES_COMMUNICATION_ETH_H
#define PROJECT_MODULES_COMMUNICATION_ETH_H


#include "stm32h7xx_hal.h"
#include "communication/data.h"

void Eth_setUartPtr(UART_HandleTypeDef* huart);
void Eth_setTimerPtr(TIM_HandleTypeDef* hTim);
void Eth_registerCallbacks(void);
void Eth_startTimer(void);

void Eth_startListening(void);
void Eth_addToTransmit(uint8_t* data);


#endif //PROJECT_MODULES_COMMUNICATION_ETH_H
