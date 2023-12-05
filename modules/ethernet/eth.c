//#include <stm32h7xx_hal_gpio.h>
//#include <stm32h7xx_hal_rcc.h>
//#include <stm32h7xx_hal_rcc_ex.h>
//#include <stm32h7xx_hal_cortex.h>
//#include <stm32h7xx_hal_uart.h>
#include <stm32h7xx_hal.h>
//#include <stm32h7xx_hal_def.h>

#include "ethernet/eth.h"
#include "can/can.h"

static UART_HandleTypeDef ethHuart;
static UART_HandleTypeDef btHuart;

static GPIO_InitTypeDef ethGpio;
static GPIO_InitTypeDef btGpio;

uint8_t UART_ReceivedRaw[19];
uint8_t searching = 0u;
uint8_t magnetosearching = 0u;
//volatile uint8_t nowa[19];
uint8_t tutaj = 0u;
static uint32_t a = 0;

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart5_rx;


volatile static MessageTypeDef UART_MessageRecieved;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//	HAL_UART_IRQHandler(&ethHuart);
	if (huart->Instance == USART1) {
		UART_Decode(&UART_ReceivedRaw);
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
		UART_Decode(&UART_ReceivedRaw);
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

//		COM_RunUartAction(UART_ReceivedRaw);
//		HAL_UART_Receive_IT(&btHuart, UART_ReceivedRaw, 19);
	}
}

void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&ethHuart);

//	UART_Decode();
//	COM_RunUartAction(&UART_MessageRecieved);
//
//	// Second init
//	HAL_UART_ReceiverTimeout_Config(&ethHuart, 30u);
//	HAL_UART_EnableReceiverTimeout(&ethHuart);
//
//	HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 19);
}

void USART3_IRQHandler() {
	HAL_UART_IRQHandler(&btHuart);

//	UART_Decode();
//	COM_RunUartAction(&UART_MessageRecieved);

//	HAL_UART_ReceiverTimeout_Config(&btHuart, 30u);
//	HAL_UART_EnableReceiverTimeout(&btHuart);

}


bool BT_Init() {
//	__HAL_RCC_DMA1_CLK_ENABLE();
//	HAL_NVIC_SetPriority(DMA1_Stream2_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(DMA1_Stream2_IRQn);
//
//	hdma_usart5_rx.Instance = DMA1_Stream2;
//	hdma_usart5_rx.Init.Request = DMA_REQUEST_USART3_RX;
//	hdma_usart5_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//	hdma_usart5_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//	hdma_usart5_rx.Init.MemInc = DMA_MINC_ENABLE;
//	hdma_usart5_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//	hdma_usart5_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//	hdma_usart5_rx.Init.Mode = DMA_CIRCULAR;
//	hdma_usart5_rx.Init.Priority = DMA_PRIORITY_LOW;
//	hdma_usart5_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//	if (HAL_DMA_Init(&hdma_usart5_rx) != HAL_OK) {
//		Error_Handler();
//	}

//	__HAL_LINKDMA(&btHuart, hdmarx, hdma_usart5_rx);

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

//
//	HAL_UART_ReceiverTimeout_Config(&btHuart, 30u);
//	HAL_UART_EnableReceiverTimeout(&btHuart);

	return 0;
}

bool Eth_Init() {
//	__HAL_RCC_DMA1_CLK_ENABLE();
//	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
//
//	hdma_usart1_rx.Instance = DMA1_Stream0;
//	hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
//	hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//	hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
//	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//	hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//	hdma_usart1_rx.Init.Mode = DMA_NORMAL;
//	hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
//	hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
//	if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK) {
//		Error_Handler();
//	}
//
//	__HAL_LINKDMA(&ethHuart, hdmarx, hdma_usart1_rx);

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
//	// HAL_UARTEx_DisableFifoMode(&ethHuart);
//
//	HAL_UART_ReceiverTimeout_Config(&ethHuart, 30);
//	HAL_UART_EnableReceiverTimeout(&ethHuart);

	return 0;
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

void DMA_STR0_IRQHandler(void) {

	HAL_DMA_IRQHandler(&hdma_usart1_rx);

	__HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx,
			__HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart1_rx));
}

void DMA_STR2_IRQHandler(void) {

	HAL_DMA_IRQHandler(&hdma_usart5_rx);

	__HAL_DMA_CLEAR_FLAG(&hdma_usart5_rx,
			__HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart5_rx));
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	a++;
}
