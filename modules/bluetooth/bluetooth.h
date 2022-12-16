#ifndef MODULE_BT_BT_H
#define MODULE_BT_BT_H

#include <stm32h7xx_hal_uart.h>
#include <stdbool.h>

bool BT_Init();
bool BT_SendData(char *ID, char *info);
bool BT_ReceiveData();

#endif	//MODULE_BT_BT_H
