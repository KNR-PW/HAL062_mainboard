/**
 ***************************************************************************************
 * @file           : eth.c
 * @author         : Jacek Prokopczuk, Krystian Czechowicz
 * @brief          : File for handling UART messages from ethernet (WiFi or Bluetooth)
 ***************************************************************************************
 */

#include <stm32h7xx_hal.h>

#include "ethernet/eth.h"
#include "can/can.h"
#include <string.h>

extern UART_HandleTypeDef ethHuart;
extern UART_HandleTypeDef btHuart;

uint8_t UART_ReceivedRaw[19];
uint8_t searching = 0u;
uint8_t magnetosearching = 0u;
//volatile uint8_t nowa[19];
uint8_t tutaj = 0u;
static uint32_t a = 0;



volatile static MessageTypeDef UART_MessageRecieved;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//	HAL_UART_IRQHandler(&ethHuart);
	if (huart->Instance == USART1) {

		UART_Decode(UART_ReceivedRaw);
		if (searching == 0) {
			COM_RunUartAction(&UART_MessageRecieved);
			UART_MessageRecieved.ID = 0;
			memset((void *)&UART_MessageRecieved.data, 0x0u, 8);
			HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 19);
			return;
		}
		if (searching == 1) {
			HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 1);
			return;
		}
		if (searching == 2) {
			HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 18);
			return;
		}
	}
	else if(huart->Instance == USART3)
	{
		UART_Decode(UART_ReceivedRaw);
		if (searching == 0) {
			COM_RunUartAction(&UART_MessageRecieved);
			UART_MessageRecieved.ID = 0;
			memset((void *)&UART_MessageRecieved.data, 0x0u, 8);
			HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19);
			return;
		}
		if (searching == 1) {
			HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 1);
			return;
		}
		if (searching == 2) {
			HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 18);
			return;
		}

//		COM_RunUartAction(UART_ReceivedRaw);
//		HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19);
	}
}

bool Eth_sendData(char *ID, char *info) {

	uint8_t ethTxBuffer[19];
	ethTxBuffer[0] = '#';
	for (uint8_t i = 0; i < 2; i++)
		ethTxBuffer[i + 1] = ID[i];

	for (uint8_t i = 0; i < 16; i++)
		ethTxBuffer[i + 3] = info[i];

	if (HAL_UART_Transmit(&ethHuart, ethTxBuffer, 19, 1000) == HAL_OK) {
		return 0;
	}
	return 1;
}

bool BT_sendData(char *ID, char *info) {

	uint8_t btTxBuffer[19];
	btTxBuffer[0] = '#';
	for (uint8_t i = 0; i < 2; i++)
		btTxBuffer[i + 1] = ID[i];

	for (uint8_t i = 0; i < 16; i++)
		btTxBuffer[i + 3] = info[i];

	if (HAL_UART_Transmit(&btHuart, btTxBuffer, 19, 1000) == HAL_OK) {
		return 0;
	}
	return 1;
}

bool BT_ReceiveData() {
	if (HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19) == HAL_OK)
		return 0;
	return 1;
}

bool Eth_ReceiveData() {
	if (HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 19) == HAL_OK)
		return 0;
	return 1;
}

void UART_Decode(uint8_t* rawMessage) {
	/*Test czy pierwszy jest # by sie przydal*/
	if (rawMessage[0] != '#' && searching != 2) {
		searching = 1;
		return;
	}
	if (rawMessage[0] != '#' && searching == 1) {
		searching = 2;
		tutaj = 1;
		return;
	}
	if (searching == 2) {
		//nie ma '#' w tym wiec trzeba do gory jedno przeniesc
		for (int p = 17; p >= 0; p--) {
			rawMessage[p + 1] = rawMessage[p];
		}
		searching = 0;
	}
//	UART_MessageRecieved.ID = 0x01u;//rawMessage[1];

	/*Zamiana hex w ACSII na liczbe*/
	if(rawMessage[0] == '#')
	{
		if (rawMessage[1] >= 65)
			UART_MessageRecieved.ID += (rawMessage[1] - 55) * 0x10;
		else
			UART_MessageRecieved.ID += (rawMessage[1] - 48) * 0x10;

		if (rawMessage[2] >= 65)
			UART_MessageRecieved.ID += (rawMessage[2] - 55);
		else
			UART_MessageRecieved.ID += (rawMessage[2] - 48);

		uint8_t i = 3;
		uint8_t index = 0;
		while (i < 19 && rawMessage[i] != 88)		//x - end of transmission
		{
			UART_MessageRecieved.data[index] = 0;
			if (rawMessage[i] >= 65)
				UART_MessageRecieved.data[index] += (rawMessage[i] - 55)
						* 0x10;
			else
				UART_MessageRecieved.data[index] += (rawMessage[i] - 48)
						* 0x10;
			i++;
			if (rawMessage[i] >= 65)
				UART_MessageRecieved.data[index] += (rawMessage[i] - 55);
			else
				UART_MessageRecieved.data[index] += (rawMessage[i] - 48);
			i++;
			index++;
		}
		UART_MessageRecieved.lenght = index;

//		memset(&UART_MessageRecieved.data, 0x0u, 8);
//		UART_MessageRecieved.lenght = 8;
//		UART_MessageRecieved.data[0] = UART_ReceivedRaw[1];
//		UART_MessageRecieved.data[1] = UART_ReceivedRaw[2];
//		UART_MessageRecieved.data[2] = UART_ReceivedRaw[3];

//		memset(&UART_ReceivedRaw, 0x0u, 22);
	}
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	a++;
}
