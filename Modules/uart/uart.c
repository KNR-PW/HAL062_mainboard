#include "uart.h"
#include <stm32h7xx_hal.h>
#include "error_handlers/error_handlers.h"

UART_HandleTypeDef huart6;

void UART_Init(void)
{
	  huart6.Instance = USART6;
	  huart6.Init.BaudRate = 115200;
	  huart6.Init.WordLength = UART_WORDLENGTH_8B;
	  huart6.Init.StopBits = UART_STOPBITS_1;
	  huart6.Init.Parity = UART_PARITY_NONE;
	  huart6.Init.Mode = UART_MODE_TX_RX;
	  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
	  huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	  huart6.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	  huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	  if (HAL_UART_Init(&huart6) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_UARTEx_SetTxFifoThreshold(&huart6, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_UARTEx_SetRxFifoThreshold(&huart6, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_UARTEx_DisableFifoMode(&huart6) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
