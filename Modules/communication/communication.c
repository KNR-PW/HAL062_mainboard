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
#include "communication/communication.h"


extern UART_HandleTypeDef btHuart;
extern UART_HandleTypeDef ethHuart;

static GPIO_InitTypeDef ethGpio;
static GPIO_InitTypeDef btGpio;

static uint32_t err_counter = 0;

volatile extern MessageTypeDef UART_MessageRecieved; // struct from can.h representing message


uint8_t UART_ReceivedRaw[19]; // check frame documentation

struct commands uartCommands;

/**
 * *******************************************************************************
 * @details		: Initialization of USART3 with FIFO Queue
 * *******************************************************************************
*/
bool BT_Init() {

	btHuart.Instance = USART3;
	btHuart.Init.BaudRate = 115200;
	btHuart.Init.WordLength = UART_WORDLENGTH_8B;
	btHuart.Init.Parity = UART_PARITY_NONE;
	btHuart.Init.StopBits = UART_STOPBITS_1;
	btHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	btHuart.Init.OverSampling = UART_OVERSAMPLING_16;
	btHuart.Init.Mode = UART_MODE_TX_RX;

	/* Peripheral clock enable */
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	btGpio.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	btGpio.Mode = GPIO_MODE_AF_PP;
	btGpio.Pull = GPIO_NOPULL;
	btGpio.Speed = GPIO_SPEED_FREQ_LOW;
	btGpio.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOD, &btGpio);

	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	HAL_UART_Init(&btHuart);

	HAL_UARTEx_SetRxFifoThreshold(&btHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetTxFifoThreshold(&btHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_EnableFifoMode(&btHuart);

	return 0;
}

/**
 * *******************************************************************************
 * @details		: Initialization of USART1 with FIFO Queue
 * *******************************************************************************
*/
bool Eth_Init() {

	ethHuart.Instance = USART1;
	ethHuart.Init.BaudRate = 115200;
	ethHuart.Init.WordLength = UART_WORDLENGTH_8B;
	ethHuart.Init.Parity = UART_PARITY_NONE;
	ethHuart.Init.StopBits = UART_STOPBITS_1;
	ethHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	ethHuart.Init.OverSampling = UART_OVERSAMPLING_16;
	ethHuart.Init.Mode = UART_MODE_TX_RX;

	//	Peripheral clock enable
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	ethGpio.Pin = GPIO_PIN_9 | GPIO_PIN_10;
	ethGpio.Mode = GPIO_MODE_AF_PP;
	ethGpio.Alternate = GPIO_AF7_USART1;
	ethGpio.Pull = GPIO_PULLDOWN;
	ethGpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &ethGpio);

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	HAL_UART_Init(&ethHuart);
	HAL_UARTEx_SetRxFifoThreshold(&ethHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetTxFifoThreshold(&ethHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_EnableFifoMode(&ethHuart);

	return 0;
}

/**
 * *******************************************************************************
 * @details		:	TODO
 * *******************************************************************************
*/
bool Eth_sendData(MessageTypeDef* MessageToSend) {

	static uint8_t ethTxBuffer[19];
	ethTxBuffer[0] = '#';

	ethTxBuffer[1] = (MessageToSend->ID & 0xF0) >> 4;
	ethTxBuffer[2] = MessageToSend->ID & 0x0F;

	// Wsadzanie danych do bufora
	for(uint8_t i = 0; i < 8; i++) {
		if(i >= MessageToSend->lenght) {
			ethTxBuffer[2*i + 3] = 'X';
			ethTxBuffer[2*i + 4] = 'X';
		}
		else {
			ethTxBuffer[2*i + 3] = (MessageToSend->data[i] & 0xF0) >> 4;
			ethTxBuffer[2*i + 4] = MessageToSend->data[i] & 0x0F;
		}
	}

	// Konwertowanie tego na ascii
	for(uint8_t i = 1; i < 19; i++)
	{
		if(ethTxBuffer[i] >= 0 && ethTxBuffer[i] <= 9)
			ethTxBuffer[i] += '0';
		else if(ethTxBuffer[i] >= 0x0A && ethTxBuffer[i] <= 0x0F)
			ethTxBuffer[i] = ethTxBuffer[i] - 0x0A + 'A';
	}

	if (HAL_UART_Transmit_IT(&ethHuart, ethTxBuffer, 19) == HAL_OK) {
		return 0;
	}
	return 1;
}

/**
 * *******************************************************************************
 * @details		:	TODO
 * *******************************************************************************
*/
bool BT_sendData(char *ID, char *info) {


	static uint8_t btTxBuffer[19];
	btTxBuffer[0] = '#';

	btTxBuffer[1] = (MessageToSend->ID & 0xF0) >> 4;
	btTxBuffer[2] = MessageToSend->ID & 0x0F;

	// Wsadzanie danych do bufora
	for(uint8_t i = 0; i < 8; i++) {
		if(i >= MessageToSend->lenght) {
			btTxBuffer[2*i + 3] = 'X';
			btTxBuffer[2*i + 4] = 'X';
		}
		else {
			btTxBuffer[2*i + 3] = (MessageToSend->data[i] & 0xF0) >> 4;
			btTxBuffer[2*i + 4] = MessageToSend->data[i] & 0x0F;
		}
	}

	// Konwertowanie tego na ascii
	for(uint8_t i = 1; i < 19; i++) {
		if(btTxBuffer[i] >= 0 && btTxBuffer[i] <= 9)
			btTxBuffer[i] += '0';
		else if(btTxBuffer[i] >= 0x0A && btTxBuffer[i] <= 0x0F)
			btTxBuffer[i] = btTxBuffer[i] - 0x0A + 'A';
	}

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
	if (HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 19) == HAL_OK)
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
 * @brief	:	Overwritten callback after receiving message
 * *******************************************************************************
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		UART_Decode(UART_ReceivedRaw);

		COM_RunUartAction();
		HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 19);
	}
	else if (huart->Instance == USART3) {
		UART_Decode(UART_ReceivedRaw);

		COM_RunUartAction();
		HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19);
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
