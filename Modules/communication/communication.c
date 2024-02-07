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
#include "can/can.h"
#include "communication/communication.h"


static GPIO_InitTypeDef ethGpio;
static GPIO_InitTypeDef btGpio;

static uint32_t err_counter = 0;
static MessageTypeDef UART_MessageRecieved; // struct from can.h representing message

UART_HandleTypeDef btHuart;
UART_HandleTypeDef ethHuart;

uint8_t UART_ReceivedRaw[19]; // check frame documentation
uint8_t searching = 0u;
uint8_t magnetosearching = 0u;
uint8_t tutaj = 0u;


DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart5_rx;


/**
 * *******************************************************************************
 * @brief	:	Overwritten callback after receiving message
 * *******************************************************************************
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		UART_Decode(UART_ReceivedRaw);
		if (searching == 0) {
			COM_RunUartAction(&UART_MessageRecieved);
			UART_MessageRecieved.ID = 0;
			memset(&UART_MessageRecieved.data, 0x0u, 8);
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
			memset(&UART_MessageRecieved.data, 0x0u, 8);
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
	}
}

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
	btHuart.Init.Mode = UART_MODE_RX;

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
	ethHuart.Init.Mode = UART_MODE_RX;

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
 * @details		:	Creating table of chars contains:
 * 					# - all message contains "#" at begging
 * 					ID - all data needs to have ID to identify what
 * 						 concrete data means
 * 					info - depending on ID, info contains concrete data
 * @see			:	UART frame documentation
 * *******************************************************************************
*/
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

	if (HAL_UART_Transmit(&btHuart, btTxBuffer, 19, 1000) == HAL_OK) {
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
		for(int j=index;j<8;j++){
			UART_MessageRecieved.data[j]='x';
		}
		UART_MessageRecieved.lenght = index;

	}
}


/**
 * *******************************************************************************
 * @brief		:	TODO required after changing to DMC
 * *******************************************************************************
*/
void DMA_STR0_IRQHandler(void) {

	HAL_DMA_IRQHandler(&hdma_usart1_rx);

	__HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx,
			__HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart1_rx));
}

/**
 * *******************************************************************************
 * @brief		:	TODO required after changing to DMC
 * *******************************************************************************
*/
void DMA_STR2_IRQHandler(void) {

	HAL_DMA_IRQHandler(&hdma_usart5_rx);

	__HAL_DMA_CLEAR_FLAG(&hdma_usart5_rx,
			__HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart5_rx));
}


/**
 * *******************************************************************************
 * @brief		:	Counting UART_Error. TODO Handling errors.
 * *******************************************************************************
*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	err_counter++;
}
