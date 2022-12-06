#include <stm32h7xx_hal_conf.h>
#include "bluetooth/bluetooth.h"


static UART_HandleTypeDef huart_bt;

bool BT_Init(USART_TypeDef* uart_instance) {

	GPIO_InitTypeDef gpio_bt_tx;
	GPIO_InitTypeDef gpio_bt_rx;

	if(uart_instance == UART5)
	{
		HAL_Init();
		gpio_bt_tx.Pin = GPIO_PIN_13;
		gpio_bt_tx.Mode = GPIO_MODE_AF_PP;
		gpio_bt_tx.Alternate = GPIO_AF8_UART5;
		gpio_bt_tx.Pull = GPIO_PULLDOWN;
		gpio_bt_tx.Speed = GPIO_SPEED_FREQ_LOW;

		gpio_bt_rx.Pin = GPIO_PIN_12;
		gpio_bt_rx.Mode = GPIO_MODE_AF_PP;
		gpio_bt_rx.Alternate = GPIO_AF8_UART5;
		gpio_bt_rx.Pull = GPIO_PULLDOWN;
		gpio_bt_rx.Speed = GPIO_SPEED_FREQ_LOW;

		__HAL_RCC_GPIOD_CLK_ENABLE();
		HAL_GPIO_Init(GPIOD, &gpio_bt_tx);
		HAL_GPIO_Init(GPIOD, &gpio_bt_rx);


		__HAL_RCC_UART5_CLK_ENABLE();
	}

	else
	{
		return 1;
	}


	huart_bt.Instance = uart_instance;
	huart_bt.Init.BaudRate = 115200;
	huart_bt.Init.WordLength = UART_WORDLENGTH_8B;
	huart_bt.Init.Parity = UART_PARITY_NONE;
	huart_bt.Init.StopBits = UART_STOPBITS_1;
	huart_bt.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart_bt.Init.OverSampling = UART_OVERSAMPLING_16;
	huart_bt.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&huart_bt);

	return 0;
}


bool BT_SendData(char* ID, char* info){
	uint8_t data[19];
	data[0] = '#';
	for(uint8_t i=0;i<2;i++)
		data[i+1]=ID[i];

	for(uint8_t i=0;i<16;i++)
		data[i+3] = info[i];

	if(HAL_UART_Transmit(&huart_bt, &data, 19,1000) == HAL_OK)
	{
		return 0;
	}
	return 1;
}


