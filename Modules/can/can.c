/**
 ******************************************************************************
 * @file           : can.c
 * @author         : Jacek Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Function prototypes for CAN protocol.
 ******************************************************************************
 */

#include <stm32h7xx_hal.h>
#include <string.h>

#include "leds/leds.h"
#include "can/can.h"
#include "error_handlers/error_handlers.h"

/**
 * @brief Header type for transmitting data to manipulator board.
 */
FDCAN_TxHeaderTypeDef TxHeader_CAN1;

/**
 * @brief Header type for transmitting data to other driver boards.
 */
FDCAN_TxHeaderTypeDef TxHeader_CAN2;

/* UNUSED */
FDCAN_RxHeaderTypeDef RxHeader;
uint8_t TxData_Node1_To_Node2[] = { 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xFA, 0xFB,
		0xFC };
uint8_t RxData_From_Node2[12];

static MessageTypeDef UART_MessageRecieved;

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

command commandList[150];
int speed_left;
int speed_right;

/**
 * @brief Sets dummy message for testing CAN to manipulator
 */
void Can_testMessage(void) {
	TxHeader_CAN1.Identifier = 0x01;
	TxHeader_CAN1.IdType = FDCAN_STANDARD_ID;
	TxHeader_CAN1.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader_CAN1.DataLength = FDCAN_DLC_BYTES_5;
	TxHeader_CAN1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader_CAN1.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader_CAN1.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader_CAN1.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader_CAN1.MessageMarker = 0x0; // Ignore because FDCAN_NO_TX_EVENTS
	uint8_t dane[] = {0, 1 ,0, 1, 0, 1, 0, 1};
	if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan1, &TxHeader_CAN1,
				dane, FDCAN_TX_BUFFER0) != HAL_OK) {
			Error_Handler();
		}

	/* Sets FDCAN Tx Buffer Add Request register */
	hfdcan1.Instance->TXBAR = 0x1u;

	/* Send Tx buffer message */
	if (HAL_FDCAN_EnableTxBufferRequest(&hfdcan1, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	/* Polling for transmission complete on buffer index 0 */
	while (HAL_FDCAN_IsTxBufferMessagePending(&hfdcan1, FDCAN_TX_BUFFER0) == 1)
		;

	/* Toggle blue LED4 after sending CAN message*/
	Leds_toggleLed(LED4);
}


/**
 * @brief Handles received UART message from ethernet (either bluetooth or Wi-Fi)
 * @note  After this function, ID attribue is NOT used and is NOT sent to CAN bus
 */
void transferTo(void) {
	/* If ID is between 0 and 128 it shall be a message to other driver boards*/
	if (UART_MessageRecieved.ID > 0 && UART_MessageRecieved.ID < 128)
		transferToCan2();

	/* If ID is between 127 and 256 it shall be a message to manipulator boards*/
	else if (UART_MessageRecieved.ID > 127 && UART_MessageRecieved.ID < 256)
		transferToCan1();
	return;
}

/**
 * @brief Function called in HAL_UART_RxCpltCallback(),
 * 		  handles received message and sets parameters of
 * 		  local static UART_MessageRecieved variable
 * @note  UART_MessageRecieved is also declared in eth file,
 * 		  but as volatile static.
 */
void COM_RunUartAction(MessageTypeDef *message) {
	UART_MessageRecieved.ID = message->ID;

	/* memcpy() func is more efficient than assigning all 8 elements of data array */
	memcpy(UART_MessageRecieved.data, message->data, 8);
	UART_MessageRecieved.lenght = message->lenght;

	/* Calls function handling CAN buses */
	transferTo();
}

/**
 * @brief Initializes TxHeader for CAN1 (manipulator) bus and assigns
 * 		  data from received UART message.
 */
void transferToCan1() {
	TxHeader_CAN1.Identifier = UART_MessageRecieved.ID;
	TxHeader_CAN1.IdType = FDCAN_STANDARD_ID;
	TxHeader_CAN1.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader_CAN1.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader_CAN1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader_CAN1.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader_CAN1.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader_CAN1.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader_CAN1.MessageMarker = 0x0; // Ignore because FDCAN_NO_TX_EVENTS

	/* Use memcpy rather than for loop to assign UART values to transfer data */
	uint8_t dane[8];
	memcpy(dane, UART_MessageRecieved.data, 8);
//	uint8_t dane[8];
//	for (int i = 0; i < 8; i++) {
//		dane[i] = UART_MessageRecieved.data[i];
//	}

	/* Add data to CAN buffer */
	if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan1, &TxHeader_CAN1,
			dane, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	/* Sets FDCAN Tx Buffer Add Request register */
	hfdcan1.Instance->TXBAR = 0x1u;

	/* Send Tx buffer message */
	if (HAL_FDCAN_EnableTxBufferRequest(&hfdcan1, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	/* Polling for transmission complete on buffer index 0 */
	while (HAL_FDCAN_IsTxBufferMessagePending(&hfdcan1, FDCAN_TX_BUFFER0) == 1);

	/* Toggle LED5 after sending CAN message*/
	Leds_toggleLed(LED4);
}

void transferToCan2() {
	TxHeader_CAN2.Identifier = UART_MessageRecieved.ID;
	TxHeader_CAN2.IdType = FDCAN_STANDARD_ID;
	TxHeader_CAN2.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader_CAN2.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader_CAN2.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader_CAN2.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader_CAN2.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader_CAN2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader_CAN2.MessageMarker = 0x0; // Ignore because FDCAN_NO_TX_EVENTS

	/* Use memcpy rather than for loop to assign UART values to transfer data */
	uint8_t dane[8];
	memcpy(dane, UART_MessageRecieved.data, 8);
//	dane[0] = UART_MessageRecieved.ID;
//	for (int i = 0; i < 8; i++) {
//		dane[i] = UART_MessageRecieved.data[i];
//	}

	/* Add data to CAN buffer */
	if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan2, &TxHeader_CAN2, dane,
			FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	/* Sets FDCAN Tx Buffer Add Request register */
	hfdcan2.Instance->TXBAR = 0x1u;

	/* Send Tx buffer message */
	if (HAL_FDCAN_EnableTxBufferRequest(&hfdcan2, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	/* Polling for transmission complete on buffer index 0 */
	while (HAL_FDCAN_IsTxBufferMessagePending(&hfdcan2, FDCAN_TX_BUFFER0) == 1)
		;
	/* Toggle LED5 after sending CAN message to other boards */
	Leds_toggleLed(LED5);

}

/* TODO */
void ignoreCAN() {
	return;
}

/* TODO */
void ignoreUART() {
	return;
}
