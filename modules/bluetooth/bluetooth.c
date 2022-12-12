#include <bluetooth/bluetooth.h>
#include <stm32h7xx_hal_gpio.h>
#include <stm32h7xx_hal_rcc.h>

static uint8_t RxBuffer[19];
static uint8_t ReceivedData[19];
static UART_HandleTypeDef btHuart;
static GPIO_InitTypeDef gpio_bt_rx;
static GPIO_InitTypeDef gpio_bt_tx;
static uint8_t nb_recived = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &btHuart)
	{
			nb_recived++;
			if( nb_recived < 19)
			{
				HAL_UART_Receive_IT(&btHuart, &RxBuffer[nb_recived], 1);
			}
			else if(nb_recived == 19)
			{
				for(uint8_t i=0; i<19; i++)
				{
					ReceivedData[i] = RxBuffer[i];
					RxBuffer[i] = 0;
				}
				nb_recived = 0;
				HAL_UART_Transmit(&btHuart, ReceivedData, 19, 1000);
				HAL_UART_Receive_IT(&btHuart, &RxBuffer[0], 1);
			}

			//TEMORARY TESTING FUNCTION: TODO: DELETE
	}

}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	HAL_UARTEx_ReceiveToIdle_IT(&btHuart, RxBuffer, 19);
	HAL_UART_Transmit(&btHuart, RxBuffer, 19, 1000);
}

void UART5_IRQHandler()
{
	HAL_UART_IRQHandler(&btHuart);
}

void HAL_UARTEx_RxFifoFullCallback(UART_HandleTypeDef *huart)
{
	HAL_UART_Transmit(huart, (uint8_t*)0xAB, 2, 1000);
}

bool BT_Init()
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



	btHuart.Instance = UART5;
	btHuart.Init.BaudRate = 9600;
	btHuart.Init.WordLength = UART_WORDLENGTH_8B;
	btHuart.Init.Parity = UART_PARITY_NONE;
	btHuart.Init.StopBits = UART_STOPBITS_1;
	btHuart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	btHuart.Init.OverSampling = UART_OVERSAMPLING_8;
	btHuart.Init.Mode = UART_MODE_TX_RX;
//	btHuart.FifoMode = UART_FIFOMODE_ENABLE;
//	HAL_UARTEx_SetRxFifoThreshold(&btHuart, UART_RXFIFO_THRESHOLD_1_8);
//	HAL_UARTEx_EnableFifoMode(&btHuart);

//	btHuart.Instance->RDR;
	HAL_UART_Init(&btHuart);
	NVIC_EnableIRQ(UART5_IRQn);
	NVIC_SetPriority(UART5_IRQn,5);

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

	if(HAL_UART_Transmit(&btHuart, data, 19,1000) == HAL_OK)
		return 0;
	return 1;
}


bool BT_ReceiveData()
{
	if(HAL_UART_Receive_IT(&btHuart, &RxBuffer[0], 1)==HAL_OK)
		return 0;
	return 1;
}




