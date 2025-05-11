#include "communication/can/can.h"
#include "errorHandlers/errorHandlers.h"
#include "communication/eth/eth.h"
#include "communication/data.h"
#include "leds/leds.h"
#include <string.h>

static FDCAN_HandleTypeDef* railCan;
static FDCAN_HandleTypeDef* extCan;
static TIM_HandleTypeDef* canTim;

static CommandBuffer rxRailCanBuffer = {0};
static CommandBuffer txRailCanBuffer = {0};
static CommandBuffer rxExtCanBuffer = {0};
static CommandBuffer txExtCanBuffer = {0};

static FDCAN_TxHeaderTypeDef railCanTxHeader;
static FDCAN_TxHeaderTypeDef extCanTxHeader;


static void CAN_setupRailCanTxHeader(void);
static void CAN_setupExtCanTxHeader(void);
static void CAN_setupRailCanFilterConfig(void);
static void CAN_setupExtCanFilterConfig(void);
static void CAN_railCanReceivedCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
static void CAN_extCanReceivedCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
static void CAN_extCanTransmitCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
static void CAN_railCanTransmitCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
static void CAN_railCanHandleData(Command* command);
static void CAN_extCanHandleData(Command* command);
static void CAN_timerCallback(TIM_HandleTypeDef *htim);
static void CAN_transmitToRailCan(Command* command);
static void CAN_transmitToExtCan(Command* command);


void CAN_setRailCanPtr(FDCAN_HandleTypeDef* hCan)
{
	railCan = hCan;
}


void CAN_setExtCanPtr(FDCAN_HandleTypeDef* hCan)
{
	extCan = hCan;
}


void CAN_setTimerPtr(TIM_HandleTypeDef* hTim)
{
	canTim = hTim;
}


void CAN_registerCallbacks(void)
{
	HAL_FDCAN_RegisterRxFifo0Callback(extCan, CAN_extCanReceivedCallback);
	HAL_FDCAN_RegisterRxFifo0Callback(railCan, CAN_railCanReceivedCallback);
	HAL_FDCAN_RegisterTxBufferCompleteCallback(extCan, CAN_extCanTransmitCallback);
	HAL_FDCAN_RegisterTxBufferCompleteCallback(railCan, CAN_railCanTransmitCallback);

	HAL_TIM_RegisterCallback(canTim, HAL_TIM_PERIOD_ELAPSED_CB_ID, CAN_timerCallback);
}


void CAN_railCanInit(void)
{
	CAN_setupRailCanTxHeader();
	CAN_setupRailCanFilterConfig();
	HAL_TIM_Base_Start_IT(canTim);
	HAL_FDCAN_ActivateNotification(railCan, FDCAN_IT_TX_COMPLETE, 0);
	if (HAL_FDCAN_Start(railCan) != HAL_OK) {
		errorHandler();
	}
}


void CAN_extCanInit(void)
{
	CAN_setupExtCanTxHeader();
	CAN_setupExtCanFilterConfig();
	HAL_FDCAN_ActivateNotification(extCan, FDCAN_IT_TX_COMPLETE, 0);
	if (HAL_FDCAN_Start(extCan) != HAL_OK) {
		errorHandler();
	}
}


void CAN_railCanStartListening(void)
{
	HAL_FDCAN_ActivateNotification(railCan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}


void CAN_extCanStartListening(void)
{
	HAL_FDCAN_ActivateNotification(extCan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}


void CAN_addCommandToTransmit(Command* command)
{
	if (command->ID > 127 && command->ID < 256)
	{
//		memcpy(&(txExtCanBuffer.commands[txExtCanBuffer.write_ptr]), command, sizeof(Command));
		commandDeepCopy(&(txExtCanBuffer.commands[txExtCanBuffer.write_ptr]), command);
//		if ((txExtCanBuffer.write_ptr+1)%BUFFER_SIZE != txExtCanBuffer.read_ptr)
//		{
			txExtCanBuffer.write_ptr = (txExtCanBuffer.write_ptr+1)%BUFFER_SIZE;
//		}
//		else
//		{
//			txExtCanBuffer.write_ptr = (txExtCanBuffer.write_ptr+1)%BUFFER_SIZE;
//			txExtCanBuffer.read_ptr = (txExtCanBuffer.read_ptr+1)%BUFFER_SIZE;
//		}
	}
	else if (command->ID > 0 && command->ID < 128)
	{

//		memcpy(&(txRailCanBuffer.commands[txRailCanBuffer.write_ptr]), command, sizeof(Command));
		commandDeepCopy(&(txRailCanBuffer.commands[txRailCanBuffer.write_ptr]), command);
//		if ((txExtCanBuffer.write_ptr+1)%BUFFER_SIZE != txExtCanBuffer.read_ptr)
//		{
			txRailCanBuffer.write_ptr = (txRailCanBuffer.write_ptr+1)%BUFFER_SIZE;
//		}
//		else
//		{
//			txRailCanBuffer.write_ptr = (txRailCanBuffer.write_ptr+1)%BUFFER_SIZE;
//			txRailCanBuffer.read_ptr = (txRailCanBuffer.read_ptr+1)%BUFFER_SIZE;
//		}

	}
}


/* =========================================================================================
 * ================================= STATIC FUNCTIONS ======================================
 * =========================================================================================*/


static void CAN_transmitToRailCan(Command* command)
{

	railCanTxHeader.Identifier = command->ID; //< ID of message

	//adding message to buffer
	while (HAL_FDCAN_AddMessageToTxBuffer(railCan, &railCanTxHeader, command->payload, FDCAN_TX_BUFFER0) != HAL_OK) {
		__NOP();
	}

	//activating transmision request flag
	railCan->Instance->TXBAR = 0x1u;

	// Send Tx buffer message
	while (HAL_FDCAN_EnableTxBufferRequest(railCan, FDCAN_TX_BUFFER0) != HAL_OK) {
//		errorHandler();
	}

	// Toggle LED4 to know that message was sent
	Leds_toggleLed(LED4);
//	txRailCanBuffer.read_ptr = (txRailCanBuffer.read_ptr + 1)%BUFFER_SIZE;
}


static void CAN_transmitToExtCan(Command* command)
{

	extCanTxHeader.Identifier = command->ID; //< ID of message

	//adding message to buffer
	while (HAL_FDCAN_AddMessageToTxBuffer(extCan, &extCanTxHeader, command->payload, FDCAN_TX_BUFFER0) != HAL_OK) {
		__NOP();
	}

	//activating transmision request flag
	extCan->Instance->TXBAR = 0x1u;

	// Send Tx buffer message
	while (HAL_FDCAN_EnableTxBufferRequest(extCan, FDCAN_TX_BUFFER0) != HAL_OK) {
//		errorHandler();
	}

	// Toggle LED5 to know that message was sent
	Leds_toggleLed(LED5);


}

static void CAN_railCanReceivedCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{

	static FDCAN_RxHeaderTypeDef rxRailCanHeader = {0};

	HAL_FDCAN_GetRxMessage(
			railCan,
			FDCAN_RX_FIFO0,
			&rxRailCanHeader,
			rxRailCanBuffer.commands[rxRailCanBuffer.write_ptr].payload
			);
	rxRailCanBuffer.commands[rxRailCanBuffer.write_ptr].ID = rxRailCanHeader.Identifier;

//	if ((rxExtCanBuffer.write_ptr+1)%BUFFER_SIZE != rxExtCanBuffer.read_ptr)
//	{
		rxRailCanBuffer.write_ptr = (rxRailCanBuffer.write_ptr+1)%BUFFER_SIZE;
//	}
//	else
//	{
//		rxRailCanBuffer.write_ptr = (rxRailCanBuffer.write_ptr+1)%BUFFER_SIZE;
//		rxRailCanBuffer.read_ptr = (rxRailCanBuffer.read_ptr+1)%BUFFER_SIZE;
//	}

	HAL_FDCAN_ActivateNotification(railCan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}


static void CAN_extCanReceivedCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{

	static FDCAN_RxHeaderTypeDef rxExtCanHeader = {0};

	HAL_FDCAN_GetRxMessage(
			extCan,
			FDCAN_RX_FIFO0,
			&rxExtCanHeader,
			rxExtCanBuffer.commands[rxExtCanBuffer.write_ptr].payload
			);
	rxExtCanBuffer.commands[rxExtCanBuffer.write_ptr].ID = rxExtCanHeader.Identifier;

//	if ((rxExtCanBuffer.write_ptr+1)%BUFFER_SIZE != rxExtCanBuffer.read_ptr)
//	{
		rxExtCanBuffer.write_ptr = (rxExtCanBuffer.write_ptr+1)%BUFFER_SIZE;
//	}
//	else
//	{
//		rxExtCanBuffer.write_ptr = (rxExtCanBuffer.write_ptr+1)%BUFFER_SIZE;
//		rxExtCanBuffer.read_ptr = (rxExtCanBuffer.read_ptr+1)%BUFFER_SIZE;
//	}

	HAL_FDCAN_ActivateNotification(extCan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}


static void CAN_extCanHandleData(Command* command)
{
	static uint8_t* data = {0};
	frameEncode(command, data);
	Eth_addToTransmit(data);
}


static void CAN_railCanHandleData(Command* command)
{
	static uint8_t* data = {0};
	frameEncode(command, data);
	Eth_addToTransmit(data);
}


static void CAN_railCanTransmitCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{

}


static void CAN_extCanTransmitCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{

}


static void CAN_timerCallback(TIM_HandleTypeDef *htim)
{
	if(txExtCanBuffer.read_ptr != txExtCanBuffer.write_ptr)
	{
		CAN_transmitToExtCan(&(txExtCanBuffer.commands[txExtCanBuffer.read_ptr]));
		txExtCanBuffer.read_ptr = (txExtCanBuffer.read_ptr + 1)%BUFFER_SIZE;
	}

	if(txRailCanBuffer.read_ptr != txRailCanBuffer.write_ptr)
	{
		CAN_transmitToRailCan(&(txRailCanBuffer.commands[txRailCanBuffer.read_ptr]));
		txRailCanBuffer.read_ptr = (txRailCanBuffer.read_ptr + 1)%BUFFER_SIZE;
	}

	if(rxExtCanBuffer.read_ptr != rxExtCanBuffer.write_ptr)
	{
		CAN_extCanHandleData(&(rxExtCanBuffer.commands[rxExtCanBuffer.read_ptr]));
		rxExtCanBuffer.read_ptr = (rxExtCanBuffer.read_ptr+1)%BUFFER_SIZE;
	}

	if(rxRailCanBuffer.read_ptr != rxRailCanBuffer.write_ptr)
	{
		CAN_railCanHandleData(&(rxRailCanBuffer.commands[rxRailCanBuffer.read_ptr]));
		rxRailCanBuffer.read_ptr = (rxRailCanBuffer.read_ptr+1)%BUFFER_SIZE;
	}
}


static void CAN_setupRailCanTxHeader(void)
{
	//CAN2 frame seeting
	railCanTxHeader.IdType = FDCAN_STANDARD_ID; 			//< Standard ID is used
	railCanTxHeader.TxFrameType = FDCAN_DATA_FRAME; 	//< Frame to transfer data
	railCanTxHeader.DataLength = FDCAN_DLC_BYTES_8; 			//< 8 BYTES of data
	railCanTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	railCanTxHeader.BitRateSwitch = FDCAN_BRS_ON;
	railCanTxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	railCanTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	railCanTxHeader.MessageMarker = 0x0; 	//< Ignore because FDCAN_NO_TX_EVENTS
}


static void CAN_setupExtCanTxHeader(void)
{
	//CAN1 frame seeting
	extCanTxHeader.IdType = FDCAN_STANDARD_ID; 			//< Standard ID is used
	extCanTxHeader.TxFrameType = FDCAN_DATA_FRAME; 	//< Frame to transfer data
	extCanTxHeader.DataLength = FDCAN_DLC_BYTES_8; 			//< 8 BYTES of data
	extCanTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	extCanTxHeader.BitRateSwitch = FDCAN_BRS_ON;
	extCanTxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	extCanTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	extCanTxHeader.MessageMarker = 0x0; 	// <Ignore because FDCAN_NO_TX_EVENTS

}


static void CAN_setupRailCanFilterConfig(void)
{
	FDCAN_FilterTypeDef sFilterConfig;

	/* Configure standard ID reception filter to Rx buffer 0 */
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_RANGE; // Ignore because FDCAN_FILTER_TO_RXBUFFER
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXBUFFER;
	sFilterConfig.FilterID1 = 0x00000000; // ID Node2
	sFilterConfig.FilterID2 = 0xFFFFFFFF; // Ignore because FDCAN_FILTER_TO_RXBUFFER
	sFilterConfig.RxBufferIndex = 0;

	if (HAL_FDCAN_ConfigFilter(railCan, &sFilterConfig) != HAL_OK) {
		errorHandler();
	}
}


static void CAN_setupExtCanFilterConfig(void)
{
	FDCAN_FilterTypeDef sFilterConfig;

	/* Configure standard ID reception filter to Rx buffer 0 */
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_RANGE; // Ignore because FDCAN_FILTER_TO_RXBUFFER
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = 0x00000000; // ID Node2
	sFilterConfig.FilterID2 = 0xFFFFFFFF; // Ignore because FDCAN_FILTER_TO_RXBUFFER
	sFilterConfig.RxBufferIndex = 0;
	if (HAL_FDCAN_ConfigFilter(extCan, &sFilterConfig) != HAL_OK) {
		errorHandler();
	}
}
