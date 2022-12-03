#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include "bluetooth/bluetooth.h"


static UART_HandleTypeDef huart_bt;

bool BT_Init(USART_TypeDef* uart_instance) {

	static GPIO_InitTypeDef gpio_bt;

	if(uart_instance == USART3)
	{
		HAL_Init();
		gpio_eth.Pin = GPIO_PIN_8;
		gpio_eth.Mode = GPIO_MODE_AF_PP;
		gpio_eth.Alternate = GPIO_AF7_USART3;
		gpio_eth.Pull = GPIO_PULLDOWN;
		gpio_eth.Speed = GPIO_SPEED_FREQ_LOW;
		__HAL_RCC_GPIOD_CLK_ENABLE();

		HAL_GPIO_Init(GPIOD, &gpio_bt);

		__HAL_RCC_USART3_CLK_ENABLE();
	}

	else
	{
		return 1;
	}


	huart_eth.Instance = uart_instance;
	huart_eth.Init.BaudRate = 115200;
	huart_eth.Init.WordLength = UART_WORDLENGTH_8B;
	huart_eth.Init.Parity = UART_PARITY_NONE;
	huart_eth.Init.StopBits = UART_STOPBITS_1;
	huart_eth.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart_eth.Init.OverSampling = UART_OVERSAMPLING_16;
	huart_eth.Init.Mode = UART_MODE_TX;
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

	if(HAL_UART_Transmit(&huart_bt, &data, 19 ,1000) == HAL_OK)
	{
		return 0;
	}
	return 1;
}
}

