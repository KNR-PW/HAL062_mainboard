#include "ethernet/eth.h"

enum ETH_STATUS eth_init(UART_HandleTypeDef huart_eth, GPIO_InitTypeDef gpio_eth){
// Initialization of Ethernet module.
//	Works in NUCLEO H743ZI2

// Input:
//	UART_HandleTypeDef uart_eth - previous defined instance of uart handle
//	GPIO_InitTypeDef gpio_eth - previous defined variable of type GPIO_InitTypeDef


// Output: Function returns enum type ETH_STATUS:
//	ETH_OK - when initialization was done successfully
//	ETH_ERROR - when initialization had problems
//

	HAL_Init();
	gpio_eth.Pin = GPIO_PIN_8;
	gpio_eth.Mode = GPIO_MODE_AF_PP;
	gpio_eth.Alternate = GPIO_AF7_USART3;
	gpio_eth.Pull = GPIO_PULLDOWN;
	gpio_eth.Speed = GPIO_SPEED_FREQ_LOW;
	__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_Init(GPIOD, &gpio_eth);
	__HAL_RCC_USART3_CLK_ENABLE();


	huart_eth.Instance = USART3;
	huart_eth.Init.BaudRate = 115200;
	huart_eth.Init.WordLength = UART_WORDLENGTH_8B;
	huart_eth.Init.Parity = UART_PARITY_NONE;
	huart_eth.Init.StopBits = UART_STOPBITS_1;
	huart_eth.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart_eth.Init.OverSampling = UART_OVERSAMPLING_16;
	huart_eth.Init.Mode = UART_MODE_TX_RX;

	HAL_UART_Init(&huart_eth);

	return ETH_OK;
}
