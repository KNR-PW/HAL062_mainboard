/**
 ******************************************************************************
 * @file           : can.c
 * @author         : Jacek, Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Functionality of communication threw CAN
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include <stm32h7xx_hal_gpio.h>
#include <stm32h7xx_hal_cortex.h>
#include <stm32h7xx_hal_rcc.h>
#include <stm32h7xx_hal_fdcan.h>
#include <stm32h7xx_hal.h>
#include <string.h>
#include "leds/leds.h"
#include "can/can.h"
#include "error_handlers/error_handlers.h"

/* Global variables -----------------------------------------------------------*/

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;

FDCAN_TxHeaderTypeDef TxHeader_CAN1;
FDCAN_TxHeaderTypeDef TxHeader_CAN2;
FDCAN_RxHeaderTypeDef RxHeader;

uint8_t testData[] = { 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xFA, 0xFB, 0xFC };

/* Static variables -----------------------------------------------------------*/

extern MessageTypeDef UART_MessageRecieved; //< Stores message from UART (bt or eth)

/* Functions ------------------------------------------------------------------*/

/**
 ******************************************************************************
 * @brief          : Initialization of CAN1
 ******************************************************************************
 */
void FDCAN1_Init(void) {

	FDCAN_FilterTypeDef sFilterConfig;

	hfdcan1.Instance = FDCAN1;
	hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan1.Init.AutoRetransmission = DISABLE;
	hfdcan1.Init.TransmitPause = DISABLE;
	hfdcan1.Init.ProtocolException = DISABLE;
	hfdcan1.Init.NominalPrescaler = 8; // 0.5 Mhz
	hfdcan1.Init.NominalSyncJumpWidth = 1;
	hfdcan1.Init.NominalTimeSeg1 = 4;
	hfdcan1.Init.NominalTimeSeg2 = 5;
	hfdcan1.Init.DataPrescaler = 8; // 0.5 MHz
	hfdcan1.Init.DataSyncJumpWidth = 1;
	hfdcan1.Init.DataTimeSeg1 = 4;
	hfdcan1.Init.DataTimeSeg2 = 5;
	hfdcan1.Init.MessageRAMOffset = 0;
	hfdcan1.Init.StdFiltersNbr = 1;
	hfdcan1.Init.ExtFiltersNbr = 0;
	hfdcan1.Init.RxFifo0ElmtsNbr = 0;
	hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
	hfdcan1.Init.RxFifo1ElmtsNbr = 0;
	hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
	hfdcan1.Init.RxBuffersNbr = 1;
	hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
	hfdcan1.Init.TxEventsNbr = 0;
	hfdcan1.Init.TxBuffersNbr = 1;
	hfdcan1.Init.TxFifoQueueElmtsNbr = 0;
	hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
	if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK) {
		Error_Handler();
	}

	/* Configure standard ID reception filter to Rx buffer 0 */
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
#if 0
  sFilterConfig.FilterType = FDCAN_FILTER_DUAL; // Ignore because FDCAN_FILTER_TO_RXBUFFER
#endif
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXBUFFER;
	sFilterConfig.FilterID1 = 0x2; // ID Node2
#if 0
  sFilterConfig.FilterID2 = 0x0; // Ignore because FDCAN_FILTER_TO_RXBUFFER
#endif
	sFilterConfig.RxBufferIndex = 0;
	if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}

	/* Start the FDCAN module */
	if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK) {
		Error_Handler();
	}

}

/**
 ******************************************************************************
 * @brief          : Initialization of CAN2
 ******************************************************************************
 */

void FDCAN2_Init(void) {

	FDCAN_FilterTypeDef sFilterConfig;

	hfdcan2.Instance = FDCAN2;
	hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan2.Init.AutoRetransmission = DISABLE;
	hfdcan2.Init.TransmitPause = DISABLE;
	hfdcan2.Init.ProtocolException = DISABLE;
	hfdcan2.Init.NominalPrescaler = 40; // 0.5 Mhz
	hfdcan2.Init.NominalSyncJumpWidth = 1;
	hfdcan2.Init.NominalTimeSeg1 = 9;
	hfdcan2.Init.NominalTimeSeg2 = 8;
	hfdcan2.Init.DataPrescaler = 40; // 0.5 MHz
	hfdcan2.Init.DataSyncJumpWidth = 1;
	hfdcan2.Init.DataTimeSeg1 = 9;
	hfdcan2.Init.DataTimeSeg2 = 8;
	hfdcan2.Init.MessageRAMOffset = 0;
	hfdcan2.Init.StdFiltersNbr = 1;
	hfdcan2.Init.ExtFiltersNbr = 0;
	hfdcan2.Init.RxFifo0ElmtsNbr = 0;
	hfdcan2.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
	hfdcan2.Init.RxFifo1ElmtsNbr = 0;
	hfdcan2.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
	hfdcan2.Init.RxBuffersNbr = 1;
	hfdcan2.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
	hfdcan2.Init.TxEventsNbr = 0;
	hfdcan2.Init.TxBuffersNbr = 1;
	hfdcan2.Init.TxFifoQueueElmtsNbr = 0;
	hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	hfdcan2.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
	if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK) {
		Error_Handler();
	}

	/* Configure standard ID reception filter to Rx buffer 0 */
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
#if 0
  sFilterConfig.FilterType = FDCAN_FILTER_DUAL; // Ignore because FDCAN_FILTER_TO_RXBUFFER
#endif
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXBUFFER;
	sFilterConfig.FilterID1 = 0x2; // ID Node2
#if 0
  sFilterConfig.FilterID2 = 0x0; // Ignore because FDCAN_FILTER_TO_RXBUFFER
#endif
	sFilterConfig.RxBufferIndex = 0;
	if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}

	/* Start the FDCAN module */
	if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 ******************************************************************************
 * @brief         	: Sending test message via CAN1
 * @details			: Message has ID = 0x01 and data =
 * 					 { 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xFA, 0xFB, 0xFC };
 ******************************************************************************
 */
void Can_testMessage(void) {

	//CAN1 frame seeting
	TxHeader_CAN1.Identifier = 0x01;
	TxHeader_CAN1.IdType = FDCAN_STANDARD_ID;
	TxHeader_CAN1.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader_CAN1.DataLength = FDCAN_DLC_BYTES_5;
	TxHeader_CAN1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader_CAN1.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader_CAN1.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader_CAN1.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader_CAN1.MessageMarker = 0x0;

	//adding message to buffer
	if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan1, &TxHeader_CAN1, testData,
	FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	//activating transmision request flag
	hfdcan1.Instance->TXBAR = 0x1u;

	// Send Tx buffer message
	if (HAL_FDCAN_EnableTxBufferRequest(&hfdcan1, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	/* Polling for transmission complete on buffer index 0 */
	while (HAL_FDCAN_IsTxBufferMessagePending(&hfdcan1, FDCAN_TX_BUFFER0) == 1)
		;
	Leds_toggleLed(LED4);
}

/**
 ******************************************************************************
 * @details			:	Saves message to UART_MessageReceived and calls
 * 						transferTo function that will pass function to
 * 						CAN1 or CAN2 depending on message ID
 ******************************************************************************
 */
void COM_RunUartAction() {
	UART_MessageRecieved.ID = message->ID;
	memcpy(UART_MessageRecieved.data, message->data, 8);
	UART_MessageRecieved.lenght = message->lenght;
	transferTo();
}

/**
 ******************************************************************************
 * @details			:	Transfers data to appropriate CAN accrording to ID:
 * 						(0-127] - CAN2 (rail - motorboards/sensorboards)
 * 						[128-256) - CAN1 (outside - manipulator/lab)
 ******************************************************************************
 */
void transferTo() {
	if (UART_MessageRecieved.ID > 0 && UART_MessageRecieved.ID < 128)
		transferToCan2();
	else if (UART_MessageRecieved.ID > 127 && UART_MessageRecieved.ID < 256)
		transferToCan1();
	return;
}

/**
 ******************************************************************************
 * @brief         	: Transfer data via CAN1 - outside (manipulator/lab)
 ******************************************************************************
 */
void transferToCan1() {

	//CAN1 frame seeting
	TxHeader_CAN1.Identifier = UART_MessageRecieved.ID; 	//< ID of message
	TxHeader_CAN1.IdType = FDCAN_STANDARD_ID; 			//< Standard ID is used
	TxHeader_CAN1.TxFrameType = FDCAN_DATA_FRAME; 	//< Frame to transfer data
	TxHeader_CAN1.DataLength = FDCAN_DLC_BYTES_8; 			//< 8 BYTES of data
	TxHeader_CAN1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader_CAN1.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader_CAN1.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader_CAN1.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader_CAN1.MessageMarker = 0x0; 	// <Ignore because FDCAN_NO_TX_EVENTS

	//creating local data table to send
	uint8_t dane[8];
	for (int i = 0; i < 8; i++) {
		dane[i] = UART_MessageRecieved.data[i];
	}

	//adding message to buffer
	if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan1, &TxHeader_CAN1, dane,
	FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	//activating transmision request flag
	hfdcan1.Instance->TXBAR = 0x1u;

	// Send Tx buffer message
	if (HAL_FDCAN_EnableTxBufferRequest(&hfdcan1, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	// Polling for transmission complete on buffer index 0
	while (HAL_FDCAN_IsTxBufferMessagePending(&hfdcan1, FDCAN_TX_BUFFER0) == 1) {
		__NOP();
	}

	// Toggle LED5 to know that message was sent
	Leds_toggleLed(LED5);
}

/**
 ******************************************************************************
 * @brief         	: Transfer data via CAN2 - rail (motorboards/sensorboards)
 ******************************************************************************
 */
void transferToCan2() {

	//CAN2 frame seeting
	TxHeader_CAN2.Identifier = UART_MessageRecieved.ID;		//< ID of message
	TxHeader_CAN2.IdType = FDCAN_STANDARD_ID; 			//< Standard ID is used
	TxHeader_CAN2.TxFrameType = FDCAN_DATA_FRAME; 	//< Frame to transfer data
	TxHeader_CAN2.DataLength = FDCAN_DLC_BYTES_8; 			//< 8 BYTES of data
	TxHeader_CAN2.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader_CAN2.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader_CAN2.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader_CAN2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader_CAN2.MessageMarker = 0x0; 	//< Ignore because FDCAN_NO_TX_EVENTS

	//creating local data table to send
	uint8_t dane[8];
	for (int i = 0; i < 8; i++) {
		dane[i] = UART_MessageRecieved.data[i];
	}

	//adding message to buffer
	if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan2, &TxHeader_CAN2, dane,
	FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	//activating transmision request flag
	hfdcan2.Instance->TXBAR = 0x1u;

	// Send Tx buffer message
	if (HAL_FDCAN_EnableTxBufferRequest(&hfdcan2, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	// Polling for transmission complete on buffer index 0
	while (HAL_FDCAN_IsTxBufferMessagePending(&hfdcan2, FDCAN_TX_BUFFER0) == 1) {
		__NOP();
	}

	// Toggle LED4 to know that message was sent
	Leds_toggleLed(LED4);

}

