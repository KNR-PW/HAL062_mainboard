#ifndef MODULE_ETH_ETH_H
#define MODULE_ETH_ETH_H

#include <stm32h7xx_hal.h>
#include <stdbool.h>

bool Eth_Init();
bool Eth_sendData(char *ID, char *info);
bool Eth_ReceiveData();

#endif // MODULE_ETH_ETH_H
