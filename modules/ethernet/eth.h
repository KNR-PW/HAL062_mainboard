#ifndef MODULE_ETH_ETH_H
#define MODULE_ETH_ETH_H

#include <stm32h7xx_hal.h>
#include <stdbool.h>

typedef struct
{
	uint8_t ID;
	uint8_t lenght;
	uint8_t data[8];
}UART_MessageTypeDef;

bool Eth_Init();

bool Eth_sendData(char *ID, char *info);

bool Eth_ReceiveData();

#endif // MODULE_ETH_ETH_H
