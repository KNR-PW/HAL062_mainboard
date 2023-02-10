#include <stm32h7xx_hal_gpio.h>
#include <stm32h7xx_hal_rcc.h>
#include <stm32h7xx_hal_rcc_ex.h>
#include <stm32h7xx_hal_cortex.h>
#include <stm32h7xx_hal.h>
//#include <stm32h7xx_hal_def.h>

#include "ethernet/eth.h"

static UART_HandleTypeDef ethHuart;
static GPIO_InitTypeDef ethGpio;
static uint8_t UART_ReceivedRaw[22];
uint8_t searching=0;
uint8_t magnetosearching=0;

uint8_t tutaj=0;

DMA_HandleTypeDef hdma_usart1_rx;

volatile UART_MessageTypeDef UART_MessageRecieved;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	// HAL_UART_Receive_IT(&ethHuart, ethRxBuffer, 19);

	if (huart->Instance==USART1)
	{
		UART_Decode();
		if (searching==0)
		{
		// COM_RunUartAction();
		HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 19);
		return;
		}
		if (searching==1)
		{
			HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 1);
			return;
		}
		if (searching==2)
		{
			HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 18);
			return;
		}
	}

}

void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&ethHuart);
}

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



	hdma_usart1_rx.Instance = DMA1_Stream0;
	hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
	hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_rx.Init.Mode = DMA_NORMAL;
	hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
	{
	  Error_Handler();
	}

	__HAL_LINKDMA(&ethHuart,hdmarx,hdma_usart1_rx);
//
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	HAL_UART_Init(&ethHuart);
	HAL_UARTEx_SetRxFifoThreshold(&ethHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetTxFifoThreshold(&ethHuart, UART_TXFIFO_THRESHOLD_1_8);
	// HAL_UARTEx_EnableFifoMode(&ethHuart);
	HAL_UARTEx_DisableFifoMode(&ethHuart);


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

bool Eth_ReceiveData() {
	if(HAL_UART_Receive_DMA(&ethHuart, UART_ReceivedRaw, 19) == HAL_OK)
		return 0;
	return 1;
//	if (HAL_UART_Receive_IT(&ethHuart, UART_ReceivedRaw, 19) == HAL_OK)
//		return 0;
//	return 1;
}

void UART_Decode()
{
	/*Test czy pierwszy jest # by sie przydal*/
	if (UART_ReceivedRaw[0]!='#'&& searching !=2)
	{
		searching = 1;
		return;
	}
	if (UART_ReceivedRaw[0]=='#' && searching == 1)
	{
		searching = 2;
		tutaj=1;
		return;
	}
	if (searching == 2)
	{
		//nie ma '#' w tym wiec trzeba do gory jedno przeniesc
		for (int p=17;p>=0;p--)
		{
			UART_ReceivedRaw[p+1]=UART_ReceivedRaw[p];
		}
		searching =0;
	}
	UART_MessageRecieved.ID = 0;

	/*Zamiana hex w ACSII na liczbe*/

	if (UART_ReceivedRaw[1]>=65)
		UART_MessageRecieved.ID += (UART_ReceivedRaw[1]-55)*0x10;
	else
		UART_MessageRecieved.ID += (UART_ReceivedRaw[1]-48)*0x10;

	if (UART_ReceivedRaw[2]>=65)
		UART_MessageRecieved.ID += (UART_ReceivedRaw[2]-55);
	else
		UART_MessageRecieved.ID += (UART_ReceivedRaw[2]-48);

	uint8_t i=3;
	uint8_t index=0;
	while (i<19 && UART_ReceivedRaw[i]!=88)			//x - end of transmission
	{
		UART_MessageRecieved.data[index]=0;
		if (UART_ReceivedRaw[i]>=65)
			UART_MessageRecieved.data[index] += (UART_ReceivedRaw[i]-55)*0x10;
		else
			UART_MessageRecieved.data[index] += (UART_ReceivedRaw[i]-48)*0x10;
		i++;
		if (UART_ReceivedRaw[i]>=65)
			UART_MessageRecieved.data[index] += (UART_ReceivedRaw[i]-55);
		else
			UART_MessageRecieved.data[index] += (UART_ReceivedRaw[i]-48);
		i++;
		index++;
	}
	UART_MessageRecieved.lenght=index;
}

void DMA_STR0_IRQHandler(void){

	HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

