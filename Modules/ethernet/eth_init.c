#include <stm32h7xx_hal.h>

#include "ethernet/eth_init.h"

UART_HandleTypeDef ethHuart;
UART_HandleTypeDef btHuart;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

void BT_Init() {

	/* Peripheral clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* NWM CZY MOZE BYC ZAKOMENTOWANE */
//	__HAL_RCC_GPIOD_CLK_ENABLE();
//	__HAL_RCC_USART3_CLK_ENABLE();


	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	btHuart.Instance = USART3;
	btHuart.Init.BaudRate = 115200;
	btHuart.Init.WordLength = UART_WORDLENGTH_8B;
	btHuart.Init.Parity = UART_PARITY_NONE;
	btHuart.Init.StopBits = UART_STOPBITS_1;
	btHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	btHuart.Init.OverSampling = UART_OVERSAMPLING_16;
	btHuart.Init.Mode = UART_MODE_TX_RX;
	btHuart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	btHuart.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	btHuart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&btHuart) != HAL_OK) {
			Error_Handler();
		}


	if (HAL_UARTEx_SetTxFifoThreshold(&btHuart, UART_TXFIFO_THRESHOLD_1_8)
				!= HAL_OK) {
			Error_Handler();
		}
		if (HAL_UARTEx_SetRxFifoThreshold(&btHuart, UART_RXFIFO_THRESHOLD_1_8)
				!= HAL_OK) {
			Error_Handler();
		}
		if (HAL_UARTEx_EnableFifoMode(&btHuart) != HAL_OK) {
			Error_Handler();
		}
}

void Eth_Init() {

	__HAL_RCC_USART1_CLK_ENABLE();
//	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

	HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

	ethHuart.Instance = USART1;
	ethHuart.Init.BaudRate = 115200;
	ethHuart.Init.WordLength = UART_WORDLENGTH_8B;
	ethHuart.Init.Parity = UART_PARITY_NONE;
	ethHuart.Init.StopBits = UART_STOPBITS_1;
	ethHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	ethHuart.Init.OverSampling = UART_OVERSAMPLING_16;
	ethHuart.Init.Mode = UART_MODE_TX_RX;

	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	if (HAL_UART_Init(&ethHuart) != HAL_OK) {
				Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&ethHuart, UART_TXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&ethHuart, UART_RXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_EnableFifoMode(&ethHuart) != HAL_OK) {
		Error_Handler();
	}

}
