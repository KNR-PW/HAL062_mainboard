#include <stm32h7xx_hal_gpio.h>
#include <stm32h7xx_hal_rcc.h>
#include <stm32h7xx_hal_rcc_ex.h>
#include <stm32h7xx_hal_cortex.h>

#include "ethernet/eth.h"

static UART_HandleTypeDef ethHuart;
static GPIO_InitTypeDef ethGpio;
static uint8_t ethRxBuffer[19];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_UART_Receive_IT(&ethHuart, ethRxBuffer, 19);
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

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	HAL_UART_Init(&ethHuart);
	HAL_UARTEx_SetRxFifoThreshold(&ethHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetTxFifoThreshold(&ethHuart, UART_TXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_EnableFifoMode(&ethHuart);

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
	if (HAL_UART_Receive_IT(&ethHuart, ethRxBuffer, 19) == HAL_OK)
		return 0;
	return 1;
}

