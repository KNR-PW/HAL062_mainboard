#include "eth.h"
#include "communication/can/can.h"
#include <string.h>
#include "camera/camera.h"

static UART_HandleTypeDef* ethUart;
static TIM_HandleTypeDef* ethTim;
static DataBuffer rxBuffer = {0};
static DataBuffer txBuffer = {0};
static uint8_t rxData[19] = {0};
static uint8_t transmitDone = 0;

static void Eth_receivedCallback(UART_HandleTypeDef *huart);
static void Eth_handleData(Command* command);
static void Eth_transmitCallback(UART_HandleTypeDef *huart);
static void Eth_timerCallback(TIM_HandleTypeDef *htim);
static void Eth_errorCallback(UART_HandleTypeDef *huart);

/**
 * *******************************************************************************
 * @brief				:	set pointer to ethernet UART handler
 * *******************************************************************************
*/
void Eth_setUartPtr(UART_HandleTypeDef* huart)
{
	ethUart = huart;
}

/**
 * *******************************************************************************
 * @brief				:	set pointer to timer handling ethernet UART
 * *******************************************************************************
*/
void Eth_setTimerPtr(TIM_HandleTypeDef* hTim)
{
	ethTim = hTim;
}

/**
 * *******************************************************************************
 * @brief				:	register Callbacks for ethernet uart
 * *******************************************************************************
*/
void Eth_registerCallbacks()
{
	HAL_UART_RegisterCallback(ethUart, HAL_UART_RX_COMPLETE_CB_ID, Eth_receivedCallback);
	HAL_UART_RegisterCallback(ethUart, HAL_UART_TX_COMPLETE_CB_ID, Eth_transmitCallback);
	HAL_UART_RegisterCallback(ethUart, HAL_UART_ERROR_CB_ID, Eth_errorCallback);
	HAL_TIM_RegisterCallback(ethTim, HAL_TIM_PERIOD_ELAPSED_CB_ID, Eth_timerCallback);

}

void Eth_startTimer(void)
{
	HAL_TIM_Base_Start_IT(ethTim);
}


/**
 * *******************************************************************************
 * @brief				:	Begin listening on ethernet UART
 * *******************************************************************************
*/
void Eth_startListening(void)
{
	HAL_UART_Receive_DMA(ethUart, rxData, PAYLOAD_SIZE);
}


/**
 * *******************************************************************************
 * @brief				:	add to buffer to transmit on ethernet UART
 * *******************************************************************************
*/
void Eth_addToTransmit(uint8_t* data)
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
 * @brief				:	handle rx callback on ethernet UART
 * *******************************************************************************
*/
static void Eth_receivedCallback(UART_HandleTypeDef *huart)
{

//	if (((rxBuffer.write_ptr+1)%BUFFER_SIZE)!= rxBuffer.read_ptr)
//	{
	memcpy(rxBuffer.data[rxBuffer.write_ptr], rxData, PAYLOAD_SIZE);
	rxBuffer.write_ptr = (rxBuffer.write_ptr + 1) % BUFFER_SIZE;
	HAL_UART_Receive_DMA(ethUart, rxData, PAYLOAD_SIZE);
//	}
//	else
//	{
//		rxBuffer.write_ptr = (rxBuffer.write_ptr + 1)%BUFFER_SIZE;
//		rxBuffer.read_ptr = (rxBuffer.read_ptr + 1)%BUFFER_SIZE;
//	}
}

/**
 * *******************************************************************************
 * @brief				:	handle recevied data from ethernet UART
 * *******************************************************************************
*/
static void Eth_handleData(Command* command)
{
	if(command->ID == 45)
	{
		// handle CAMERA
		CAM_setCamerasSource(command->payload[0], command->payload[1], command->payload[2]);
	}
	else
	{
		CAN_addCommandToTransmit(command);
	}

}

/**
 * *******************************************************************************
 * @brief				:	handle tx callback on ethernet UART
 * *******************************************************************************
*/
static void Eth_transmitCallback(UART_HandleTypeDef *huart)
{
	transmitDone = 1;

}

static void Eth_errorCallback(UART_HandleTypeDef *huart)
{
//	if (huart->ErrorCode & HAL_UART_ERROR_ORE){
//		__HAL_UART_CLEAR_OREFLAG(huart);
//		HAL_UART_Abort(huart);
//		HAL_UART_Receive_DMA(ethUart, rxBuffer.data[rxBuffer.write_ptr], PAYLOAD_SIZE);
//	}
}

/**
 * *******************************************************************************
 * @brief				:	timer for handling data that arrived or are sent
 * *******************************************************************************
*/
static void Eth_timerCallback(TIM_HandleTypeDef *htim)
{
	//	sending data
	if( txBuffer.read_ptr != txBuffer.write_ptr)
	{
		if( transmitDone == 1)
		{
		HAL_UART_Transmit_DMA(ethUart, txBuffer.data[txBuffer.read_ptr], PAYLOAD_SIZE);
		transmitDone = 0;
		txBuffer.read_ptr = (txBuffer.read_ptr+1)%BUFFER_SIZE;
		}
	}

	// receiving data
	static Command currentCommand = {0};
	if( rxBuffer.read_ptr != rxBuffer.write_ptr)
	{
		frameDecode(rxBuffer.data[rxBuffer.read_ptr], &currentCommand);
		rxBuffer.read_ptr = (rxBuffer.read_ptr+1)%BUFFER_SIZE;
		Eth_handleData(&currentCommand);
	}

}

