/**
 ******************************************************************************
 * @file          	: communication.c
 * @author        	: Krystian Czechowicz, Adam Rybojad
 * @brief         	: Wireless communications functionality
 * @details			: This file contains both ethernet and bluetooth
 * 					  UART configuration. To undarstad it is required to see
 * 					  frame documentation. (You can see in communication.c)
 ******************************************************************************
 */

#include <stm32h7xx_hal.h>
#include <string.h>
#include "cube_interface.h"

#include "can.h"
#include "communication.h"
#include "camera.h"
#include "lamp.h"
#include "error_handlers.h"


static uint32_t err_counter = 0;
extern MessageTypeDef UART_MessageRecieved; // struct from can.h representing message

UART_HandleTypeDef btHuart;
UART_HandleTypeDef ethHuart;

uint8_t UART_ReceivedRaw[19]; // check frame documentation
uint8_t searching = 0u;
uint8_t magnetosearching = 0u;
uint8_t tutaj = 0u;

struct commands uartCommands;


/**
 * *******************************************************************************
 * @brief	:	Overwritten callback after receiving message
 * *******************************************************************************
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		HAL_IWDG_Refresh(&hiwdg1);
		UART_Decode(UART_ReceivedRaw);
		if (UART_MessageRecieved.ID == 45  ) 
		{
			handleCamera(UART_MessageRecieved.data);
		}
		else if (UART_MessageRecieved.ID == 10  ) 
		{
			handleLED(UART_MessageRecieved.data);
			Set_Max_Value(UART_MessageRecieved.data);
		}
		else
		{
			COM_RunUartAction(&UART_MessageRecieved);
		}
		UART_MessageRecieved.ID = 0;
		memset(&UART_MessageRecieved.data, 0x0u, 8);
		HAL_UART_Receive_DMA(&ethHuart, UART_ReceivedRaw, 19);
		return;
		
	}
	else if (huart->Instance == USART3) {
		HAL_IWDG_Refresh(&hiwdg1);
		UART_Decode(UART_ReceivedRaw);
		if (UART_MessageRecieved.ID == 45  ) 
		{
			handleCamera(UART_MessageRecieved.data);
		}
		else if (UART_MessageRecieved.ID == 10  ) 
		{
			handleLED(UART_MessageRecieved.data);
			Set_Max_Value(UART_MessageRecieved.data);
		}
		else
		{
			COM_RunUartAction(&UART_MessageRecieved);
		}
		UART_MessageRecieved.ID = 0;
		memset(&UART_MessageRecieved.data, 0x0u, 8);
		HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19);
		return;
		
	}
}


/**
 * *******************************************************************************
 * @details		: Initialization of USART3 with FIFO Queue
 * *******************************************************************************
*/
bool BT_Init() {
	btHuart = huart3;

	Cube_MX_USART3_UART_Init();

	return true;
}

/**
 * *******************************************************************************
 * @details		: Initialization of USART1 with FIFO Queue
 * *******************************************************************************
*/
bool Eth_Init() {
	ethHuart = huart1;

	Cube_MX_DMA_Init();
	Cube_MX_USART1_UART_Init();

	return true;
}

/**
 * *******************************************************************************
 * @details		:	Initialization of watchdog that will reset board
 * 					after communcication failure
 * *******************************************************************************
*/
void Watchdog_Init(void){
	Cube_MX_IWDG1_Init();
}

/**
 * *******************************************************************************
 * @details		:	Creating table of chars contains:
 * 					# - all message contains "#" at begging
 * 					ID - all data needs to have ID to identify what
 * 						 concrete data means
 * 					info - depending on ID, info contains concrete data
 * @see			:	UART frame documentation
 * *******************************************************************************
*/
bool Eth_sendData(uint8_t *ID, uint8_t *info) {

	uint8_t ethTxBuffer[19];
	ethTxBuffer[0] = '#';
	for (uint8_t i = 0; i < 2; i++)
		ethTxBuffer[i + 1] = ID[i];

	for (uint8_t i = 0; i < 16; i++)
		ethTxBuffer[i + 3] = info[i];

	if (HAL_UART_Transmit_DMA(&ethHuart, ethTxBuffer, 19) == HAL_OK) {
		return 0;
	}
	return 1;
}

/**
 * *******************************************************************************
 * @details		:	Creating table of chars contains:
 * 					# - all message contains "#" at begging
 * 					ID - all data needs to have ID to identify what
 * 						 concrete data means
 * 					info - depending on ID, info contains concrete data
 * @see			:	UART frame documentation
 * *******************************************************************************
*/
bool BT_sendData(char *ID, char *info) {

	uint8_t btTxBuffer[19];
	btTxBuffer[0] = '#';
	for (uint8_t i = 0; i < 2; i++)
		btTxBuffer[i + 1] = ID[i];

	for (uint8_t i = 0; i < 16; i++)
		btTxBuffer[i + 3] = info[i];

	if (HAL_UART_Transmit_IT(&btHuart, btTxBuffer, 19) == HAL_OK) {
		return 0;
	}
	return 1;
}

/**
 * *******************************************************************************
 * @details		:	starting listening on bluetooth UART pins
 * *******************************************************************************
*/
bool BT_ReceiveData() {
	if (HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19) == HAL_OK)
		return 0;
	return 1;
}

/**
 * *******************************************************************************
 * @details		:	starting listening on bluetooth UART pins
 * *******************************************************************************
*/
bool Eth_ReceiveData() {
	if (HAL_UART_Receive_DMA(&ethHuart, UART_ReceivedRaw, 19) == HAL_OK)
		return 0;
	return 1;
}

/**
 * *******************************************************************************
 * @details		:	all data are sent as HEX number coded in ASCII
 * 					(f.e number 140 is 8C so we send 0x38 (8 in ASCII)
 * 					and 0x43 (C in ASCII)
 * @see			:	UART frame documentation
 * *******************************************************************************
*/
void UART_Decode(uint8_t* rawMessage) {

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
		while (i < 19 && rawMessage[i]!=88)
		//x - end of transmission
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
		for(uint8_t j=index;j<8;j++){
			UART_MessageRecieved.data[j]='X';
		}
		UART_MessageRecieved.lenght = index;

	}
	
}

/**
 * *******************************************************************************
 * @brief		:	Counting UART_Error. TODO Handling errors.
 * *******************************************************************************
*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {

	err_counter++;

}



void UART_encode(uint8_t value, uint8_t *hex) {
    const uint8_t hex_digits[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    hex[0] = hex_digits[(value >> 4) & 0x0F];  // High nibble
    hex[1] = hex_digits[value & 0x0F];         // Low nibble
}
