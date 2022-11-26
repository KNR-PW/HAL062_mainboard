#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include "ethernet/eth.h"

bool Eth_init(USART_TypeDef* uart_instance){

// Initialization of Ethernet module.
//	Works in NUCLEO H743ZI2

// Input:
//	USART_TypeDef uart_instance - needed to correctly set register and gpio


// Output: Function returns bool type:
//	0 - when initialization was done successfully
//	1 - when initialization had problems
//
//
//	!!! Global static !!!
//	For those who doesn't want to use prepared ETH functions
//	there are static global variable to UART_HandleTypeDef huart_eth and
//	GPIO_InitTypeDef gpio_eth. Change them carefully
//
//

	static UART_HandleTypeDef huart_eth;
	static GPIO_InitTypeDef gpio_eth;

	//GPIO initialization for USART3
	if(uart_instance == USART3){
	HAL_Init();
	gpio_eth.Pin = GPIO_PIN_8;
	gpio_eth.Mode = GPIO_MODE_AF_PP;
	gpio_eth.Alternate = GPIO_AF7_USART3;
	gpio_eth.Pull = GPIO_PULLDOWN;
	gpio_eth.Speed = GPIO_SPEED_FREQ_LOW;
	__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_Init(GPIOD, &gpio_eth);
	__HAL_RCC_USART3_CLK_ENABLE();
	}
	else
	{
		return 1;
	}

	//USART initialization - default data and instance
	huart_eth.Instance = uart_instance;
	huart_eth.Init.BaudRate = 115200;
	huart_eth.Init.WordLength = UART_WORDLENGTH_8B;
	huart_eth.Init.Parity = UART_PARITY_NONE;
	huart_eth.Init.StopBits = UART_STOPBITS_1;
	huart_eth.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart_eth.Init.OverSampling = UART_OVERSAMPLING_16;
	huart_eth.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&huart_eth);

	return 0;
}


bool Eth_sendData(char* ID, char* info){
//
// !!! WORKS FOR ROVER COMMUNICATION FRAME !!!
//
//	-------------------------------------------
//	Input:
//	-ID - 2 bytes (2 x uint8_t) identification of area which info is about
//	-INFO - 16 bytes (16 x uint8_t) information connected with concrete ID
//

	// Output: Function returns bool type:
	//	0 - when initialization was done successfully
	//	1 - when initialization had problems



	uint8_t data[19];
	data[0] = '#';
	for(uint8_t i=0;i<2;i++)
		data[i+1]=ID[i];

	for(uint8_t i=0;i<16;i++)
		data[i+3] = info[i];


	static UART_HandleTypeDef huart_eth;

	if(HAL_UART_Transmit(&huart_eth,data,19,1000) == HAL_OK)
	{
		return 0;
	}
	return 1;

}



