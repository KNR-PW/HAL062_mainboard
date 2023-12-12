#ifndef MODULE_ETH_ETH_H
#define MODULE_ETH_ETH_H

#include <stm32h7xx_hal.h>
#include <stdbool.h>
#include "can/can.h"

bool Eth_Init();
bool BT_Init();

bool BT_sendData(MessageTypeDef* MessageToSend);
bool Eth_sendData(MessageTypeDef* MessageToSend);
void UART_Decode(uint8_t* rawMessage);

// TODO: Usunąć - tylko do celów testowych
void ETH_Test(void);

bool Eth_ReceiveData();
bool BT_ReceiveData() ;


#endif // MODULE_ETH_ETH_H
