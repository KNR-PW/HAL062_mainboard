#ifndef MODULE_BT_BT_H
#define MODULE_BT_BT_H

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include <stdbool.h>

bool BT_Init(USART_TypeDef* uart_instance);
bool BT_SendData(char* ID, char* info);

#endif	//MODULE_BT_BT_H
