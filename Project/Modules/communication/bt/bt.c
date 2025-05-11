#include "bt.h"
#include "communication/can/can.h"
#include <string.h>

static UART_HandleTypeDef* btUart;
static TIM_HandleTypeDef* btTim;
static DataBuffer rxBuffer = {0};
static DataBuffer txBuffer = {0};

static void Bt_receivedCallback(UART_HandleTypeDef *huart);
static void Bt_handleData(Command* command);
static void Bt_transmitCallback(UART_HandleTypeDef *huart);
static void Bt_timerCallback(TIM_HandleTypeDef *htim);


/**
 * *******************************************************************************
 * @brief				:	set pointer to bluetooth UART handler
 * *******************************************************************************
*/
void Bt_setUartPtr(UART_HandleTypeDef* huart)
{
	btUart = huart;
}

/**
 * *******************************************************************************
 * @brief				:	set pointer to timer handling bluetooth UART
 * *******************************************************************************
*/
void Bt_setTimerPtr(TIM_HandleTypeDef* hTim)
{
	btTim = hTim;
}

/**
 * *******************************************************************************
 * @brief				:	register Callbacks for bluetooth uart
 * *******************************************************************************
*/
void Bt_registerCallbacks(void)
{
	HAL_UART_RegisterCallback(btUart, HAL_UART_RX_COMPLETE_CB_ID, Bt_receivedCallback);
	HAL_UART_RegisterCallback(btUart, HAL_UART_TX_COMPLETE_CB_ID, Bt_transmitCallback);
	HAL_TIM_RegisterCallback(btTim, HAL_TIM_PERIOD_ELAPSED_CB_ID, Bt_timerCallback);

}

void Bt_startTimer(void)
{
	HAL_TIM_Base_Start_IT(btTim);
}

/**
 * *******************************************************************************
 * @brief				:	Begin listening on bluetooth UART
 * *******************************************************************************
*/
void Bt_startListening(void)
{
	HAL_UART_Receive_DMA(btUart, rxBuffer.data[rxBuffer.write_ptr], PAYLOAD_SIZE);
}

/**
 * *******************************************************************************
 * @brief				:	add to buffer to transmit on bluetooth UART
 * *******************************************************************************
*/
void Bt_addToTransmit(uint8_t* data)
{

	memcpy(txBuffer.data[txBuffer.write_ptr],data,PAYLOAD_SIZE);

//	if (((txBuffer.write_ptr+1)%BUFFER_SIZE)!= txBuffer.read_ptr)
//	{
		txBuffer.write_ptr = (txBuffer.write_ptr + 1)%BUFFER_SIZE;
//	}
//	else
//	{
//		txBuffer.write_ptr = (txBuffer.write_ptr + 1)%BUFFER_SIZE;
//		txBuffer.read_ptr = (txBuffer.read_ptr + 1)%BUFFER_SIZE;
//	}
}


/* =========================================================================================
 * ================================= STATIC FUNCTIONS ======================================
 * =========================================================================================*/


/**
 * *******************************************************************************
 * @brief				:	handle rx callback on bluetooth UART
 * *******************************************************************************
*/
static void Bt_receivedCallback(UART_HandleTypeDef *huart)
{
//	if (((rxBuffer.write_ptr+1)%BUFFER_SIZE)!= rxBuffer.read_ptr)
//	{
		rxBuffer.write_ptr = (rxBuffer.write_ptr + 1)%BUFFER_SIZE;
//	}
//	else
//	{
//		rxBuffer.write_ptr = (rxBuffer.write_ptr + 1)%BUFFER_SIZE;
//		rxBuffer.read_ptr = (rxBuffer.read_ptr + 1)%BUFFER_SIZE;
//	}
	HAL_UART_Receive_DMA(btUart, rxBuffer.data[rxBuffer.write_ptr], PAYLOAD_SIZE);
}

/**
 * *******************************************************************************
 * @brief				:	handle recevied data from bluetooth UART
 * *******************************************************************************
*/
static void Bt_handleData(Command* command)
{
	if(command->ID == 45)
	{
		// handle CAMERA
	}
	else
	{
		CAN_addCommandToTransmit(command);
	}

}

/**
 * *******************************************************************************
 * @brief				:	handle tx callback on bluetooth UART
 * *******************************************************************************
*/
static void Bt_transmitCallback(UART_HandleTypeDef *huart)
{

}

/**
 * *******************************************************************************
 * @brief				:	timer for handling data that arrived or are sent
 * *******************************************************************************
*/
static void Bt_timerCallback(TIM_HandleTypeDef *htim)
{
	//	sending data
	if( txBuffer.read_ptr != txBuffer.write_ptr)
	{
		HAL_UART_Transmit_DMA(btUart, txBuffer.data[txBuffer.read_ptr], PAYLOAD_SIZE);
		txBuffer.read_ptr = (txBuffer.read_ptr+1)%BUFFER_SIZE;
	}

	// receiving data
	static Command currentCommand = {0};
	if( rxBuffer.read_ptr != rxBuffer.write_ptr)
	{
		frameDecode(rxBuffer.data[rxBuffer.read_ptr], &currentCommand);
		rxBuffer.read_ptr = (rxBuffer.read_ptr+1)%BUFFER_SIZE;
		Bt_handleData(&currentCommand);
	}

}

