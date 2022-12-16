#include <bluetooth/bluetooth.h>
#include <stm32h7xx_hal_gpio.h>
#include <stm32h7xx_hal_rcc.h>
#include <stm32h7xx_hal_rcc_ex.h>
#include <stm32h7xx_hal_cortex.h>

static uint8_t btRxBuffer[19];
static UART_HandleTypeDef btHuart;
static GPIO_InitTypeDef btGpio;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	HAL_UART_Receive_IT(&btHuart, btRxBuffer, 19);
}

void UART5_IRQHandler() {
	HAL_UART_IRQHandler(&btHuart);
}

bool BT_Init() {

	btHuart.Instance = UART5;
	btHuart.Init.BaudRate = 115200;
	btHuart.Init.WordLength = UART_WORDLENGTH_8B;
	btHuart.Init.Parity = UART_PARITY_NONE;
	btHuart.Init.StopBits = UART_STOPBITS_1;
	btHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	btHuart.Init.OverSampling = UART_OVERSAMPLING_16;
	btHuart.Init.Mode = UART_MODE_TX_RX;

	/* Peripheral clock enable */
	__HAL_RCC_UART5_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	btGpio.Pin = GPIO_PIN_12 | GPIO_PIN_13;
	btGpio.Mode = GPIO_MODE_AF_PP;
	btGpio.Pull = GPIO_NOPULL;
	btGpio.Speed = GPIO_SPEED_FREQ_LOW;
	btGpio.Alternate = GPIO_AF14_UART5;
	HAL_GPIO_Init(GPIOB, &btGpio);

	HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(UART5_IRQn);

	HAL_UART_Init(&btHuart);
	HAL_UARTEx_SetRxFifoThreshold(&btHuart, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetTxFifoThreshold(&btHuart, UART_TXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_EnableFifoMode(&btHuart);

	return 0;
}

bool BT_SendData(char *ID, char *info) {
	uint8_t btTxBuffer[19];
	btTxBuffer[0] = '#';
	for (uint8_t i = 0; i < 2; i++)
		btTxBuffer[i + 1] = ID[i];

	for (uint8_t i = 0; i < 16; i++)
		btTxBuffer[i + 3] = info[i];

	if (HAL_UART_Transmit(&btHuart, btTxBuffer, 19, 1000) == HAL_OK)
		return 0;
	return 1;
}

bool BT_ReceiveData() {
	if (HAL_UART_Receive_IT(&btHuart, btRxBuffer, 19) == HAL_OK)
		return 0;
	return 1;
}

