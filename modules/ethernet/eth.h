#ifndef MODULE_ETH_ETH_H
#define MODULE_ETH_ETH_H

#include <stm32h7xx_hal_conf.h>
#include <stm32h7xx_hal.h>
#include <stdbool.h>

bool Eth_init(USART_TypeDef* uart_instance);
bool Eth_sendData(char* ID, char* info);

#endif // MODULE_ETH_ETH_H
