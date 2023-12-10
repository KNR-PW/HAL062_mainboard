#include <stm32h7xx_hal.h>

#include "ethernet/eth.h"
#include "can/can.h"

static UART_HandleTypeDef ethHuart;
static UART_HandleTypeDef btHuart;

static GPIO_InitTypeDef ethGpio;
static GPIO_InitTypeDef btGpio;

uint8_t UART_ReceivedRaw[19];
uint8_t magnetosearching = 0u;

static uint32_t a = 0;

volatile static MessageTypeDef UART_MessageRecieved;

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

bool Eth_Init() {
	ethHuart.Instance = USART1;
	ethHuart.Init.BaudRate = 9600;
	ethHuart.Init.WordLength = UART_WORDLENGTH_8B;
	ethHuart.Init.Parity = UART_PARITY_NONE;
	ethHuart.Init.StopBits = UART_STOPBITS_1;
	ethHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	ethHuart.Init.OverSampling = UART_OVERSAMPLING_16;
	ethHuart.Init.Mode = UART_MODE_TX_RX;

	// TODO: Zmienić na konfigurację startową

	//	Peripheral clock enable
	__HAL_RCC_USART1_CLK_ENABLE();
	//__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	//ethGpio.Pin = GPIO_PIN_9 | GPIO_PIN_10;
	ethGpio.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	ethGpio.Mode = GPIO_MODE_AF_PP;
	ethGpio.Alternate = GPIO_AF7_USART1;
	ethGpio.Pull = GPIO_PULLDOWN;
	ethGpio.Speed = GPIO_SPEED_FREQ_LOW;
	//HAL_GPIO_Init(GPIOA, &ethGpio);
	HAL_GPIO_Init(GPIOB, &ethGpio);

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	HAL_UART_Init(&ethHuart);
	HAL_UARTEx_SetRxFifoThreshold(&ethHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetTxFifoThreshold(&ethHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_EnableFifoMode(&ethHuart);

	return 0;
}

bool Eth_sendData(MessageTypeDef* MessageToSend)
{
	static uint8_t ethTxBuffer[19];
	ethTxBuffer[0] = '#';

	ethTxBuffer[1] = (MessageToSend->ID & 0xF0) >> 4;
	ethTxBuffer[2] = MessageToSend->ID & 0x0F;

	for(uint8_t i = 0; i < 8; i++)
	{
		if(i >= MessageToSend->lenght)
		{
			ethTxBuffer[2*i + 3] = 'x';
			ethTxBuffer[2*i + 4] = 'x';
		}
		else
		{
			ethTxBuffer[2*i + 3] = (MessageToSend->data[i] & 0xF0) >> 4;
			ethTxBuffer[2*i + 4] = MessageToSend->data[i] & 0x0F;
		}
	}

	for(uint8_t i = 1; i < 19; i++)
	{
		if(ethTxBuffer[i] >= 0 && ethTxBuffer[i] <= 9)
		{
			ethTxBuffer[i] += '0';
		}
		else if(ethTxBuffer[i] >= 0x0A && ethTxBuffer[i] <= 0x0F)
		{
			ethTxBuffer[i] = ethTxBuffer[i] - 0x0A + 'A';
		}
	}

	if (HAL_UART_Transmit(&ethHuart, ethTxBuffer, 19, HAL_MAX_DELAY) == HAL_OK) {
		return 0;
	}
	return 1;
}

bool BT_sendData(char *ID, char *info) {

	static uint8_t btTxBuffer[19];
	btTxBuffer[0] = '#';

	btTxBuffer[1] = (MessageToSend->ID & 0xF0) >> 4;
	btTxBuffer[2] = MessageToSend->ID & 0x0F;

	for(uint8_t i = 0; i < 8; i++)
	{
		if(i >= MessageToSend->lenght)
		{
			btTxBuffer[2*i + 3] = 'x';
			btTxBuffer[2*i + 4] = 'x';
		}
		else
		{
			btTxBuffer[2*i + 3] = (MessageToSend->data[i] & 0xF0) >> 4;
			btTxBuffer[2*i + 4] = MessageToSend->data[i] & 0x0F;
		}
	}

	for(uint8_t i = 1; i < 19; i++)
	{
		if(btTxBuffer[i] >= 0 && btTxBuffer[i] <= 9)
		{
			btTxBuffer[i] += '0';
		}
		else if(btTxBuffer[i] >= 0x0A && btTxBuffer[i] <= 0x0F)
		{
			btTxBuffer[i] = btTxBuffer[i] - 0x0A + 'A';
		}
	}

	if (HAL_UART_Transmit(&ethHuart, btTxBuffer, 19, HAL_MAX_DELAY) == HAL_OK) {
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

void ETH_Test(void)
{
	char *msg = "Gajo Jajo";
	uint8_t *raw = "#55123456789ABCDEFA";
	HAL_StatusTypeDef ret;

	MessageTypeDef exampl_mess = {0xFA, 8, {0xA1, 0x2B, 0xC3, 0x4D, 0xE5, 0x6F, 7, 8}};
	Eth_sendData(&exampl_mess);

	Eth_ReceiveData();
	while(1)
	{
		HAL_Delay(500);
	}

}

void UART_Decode(uint8_t* rawMessage) {

	// TODO: Dodać sprawdzanie czy użyto wielkie czy małe litery
	if(rawMessage[0] != '#')
		return;

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
			UART_MessageRecieved.data[index] += (rawMessage[i] - 55) * 0x10;
		else
			UART_MessageRecieved.data[index] += (rawMessage[i] - 48) * 0x10;
		i++;
		if (rawMessage[i] >= 65)
			UART_MessageRecieved.data[index] += (rawMessage[i] - 55);
		else
			UART_MessageRecieved.data[index] += (rawMessage[i] - 48);
		i++;
		index++;
	}
	UART_MessageRecieved.lenght = index;
}

void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&ethHuart);
}

void USART3_IRQHandler() {
	HAL_UART_IRQHandler(&btHuart);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart->Instance == USART1)
	{
		UART_Decode(&UART_ReceivedRaw);
		if(Eth_ReceiveData() != 0)
		{
			HAL_UART_ErrorCallback(&huart);
		}
	}
	else if(huart->Instance == USART3)
	{
		UART_Decode(&UART_ReceivedRaw);
		if(Bt_ReceiveData() != 0)
		{
			HAL_UART_ErrorCallback(&huart);
		}
	}

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	a++;
}


