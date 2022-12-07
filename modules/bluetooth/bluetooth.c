#include <bluetooth/bluetooth.h>
#include <stm32h7xx_hal_gpio.h>
#include <stm32h7xx_hal_rcc.h>

static uint8_t collected_data[19];
static UART_HandleTypeDef huart_bt;
static GPIO_InitTypeDef gpio_bt_tx;
static GPIO_InitTypeDef gpio_bt_rx;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart_bt) {
	//TODO: FUNTION TO INTERPRET RECEIVED DATA


	//TEMORARY TESTING FUNCTION: TODO: DELETE
	HAL_UART_Transmit(&huart_bt, collected_data, 19, 1000);


	//SETTING LISTENING:
	HAL_UART_Receive_IT(&huart_bt, collected_data, 19);
	}
}


void UART5_IRQHandler()
{
	HAL_UART_IRQHandler(&huart_bt);

}


bool BT_Init(USART_TypeDef* uart_instance)
{
	if(uart_instance == UART5)
	{

		gpio_bt_tx.Pin = GPIO_PIN_13;
		gpio_bt_tx.Mode = GPIO_MODE_AF_PP;
		gpio_bt_tx.Alternate = GPIO_AF14_UART5;
		gpio_bt_tx.Pull = GPIO_PULLUP;
		gpio_bt_tx.Speed = GPIO_SPEED_FREQ_LOW;

		gpio_bt_rx.Pin = GPIO_PIN_12;
		gpio_bt_rx.Mode = GPIO_MODE_AF_PP;
		gpio_bt_rx.Alternate = GPIO_AF14_UART5;
		gpio_bt_rx.Pull = GPIO_PULLUP;
		gpio_bt_rx.Speed = GPIO_SPEED_FREQ_LOW;

		__HAL_RCC_GPIOB_CLK_ENABLE();
		HAL_GPIO_Init(GPIOB, &gpio_bt_tx);
		HAL_GPIO_Init(GPIOB, &gpio_bt_rx);


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
//	huart_bt.FifoMode = UART_FIFOMODE_ENABLE;

	HAL_UART_Init(&huart_bt);
	NVIC_EnableIRQ(UART5_IRQn);
	NVIC_SetPriority(UART5_IRQn,12);

	return 0;
}


bool BT_SendData(char* ID, char* info)
{
	uint8_t data[19];
	data[0] = '#';
	for(uint8_t i=0;i<2;i++)
		data[i+1]=ID[i];

	for(uint8_t i=0;i<16;i++)
		data[i+3] = info[i];

	if(HAL_UART_Transmit(&huart_bt, data, 19,1000) == HAL_OK)
	{
		return 0;
	}
	return 1;
}


bool BT_ReceivingData()
{
	if(HAL_UART_Receive_IT(&huart_bt, collected_data, 19)==HAL_OK)
		return 0;
	return 1;
}



