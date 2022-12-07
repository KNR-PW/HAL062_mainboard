#ifndef MODULE_BT_BT_H
#define MODULE_BT_BT_H

#include <stm32h7xx_hal_uart.h>
#include <stdbool.h>

bool BT_Init(USART_TypeDef* uart_instance);
bool BT_SendData(char* ID, char* info);
bool BT_ReceivingData();

#endif	//MODULE_BT_BT_H
