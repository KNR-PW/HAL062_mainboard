#include "leds/leds.h"
#include "uart/uart.h"
#include "can/can.h"
#include "camera/camera.h"
#include "errorHandlers/errorHandlers.h"
#include "watchdog/watchdog.h"

extern UART_HandleTypeDef huart6; // huart1
extern UART_HandleTypeDef huart3;

static UART_HandleTypeDef *eth_uart_handle = &huart6;
static UART_HandleTypeDef *bt_uart_handle = &huart3;

static void UART_TXCompleteClb(UART_HandleTypeDef *huart);
static void UART_RXCompleteClb(UART_HandleTypeDef *huart); 
static void UART_ErrorClb(UART_HandleTypeDef *huart);

static void knrFrame_encode(uint8_t *data, uint8_t *encoded, uint8_t data_length);
static uint8_t knrFrame_decode(uint8_t *encoded, uint8_t *data, uint8_t data_length);

static uint8_t eth_data[PAYLOAD_SIZE] = { 0 };
static uint8_t bt_data[PAYLOAD_SIZE] = { 0 };


void UART_init(void) {
	HAL_UART_RegisterCallback(eth_uart_handle, HAL_UART_TX_COMPLETE_CB_ID, UART_TXCompleteClb);
	HAL_UART_RegisterCallback(bt_uart_handle, HAL_UART_TX_COMPLETE_CB_ID, UART_TXCompleteClb);
	
	HAL_UART_RegisterCallback(eth_uart_handle, HAL_UART_RX_COMPLETE_CB_ID, UART_RXCompleteClb);
	HAL_UART_RegisterCallback(bt_uart_handle, HAL_UART_RX_COMPLETE_CB_ID, UART_RXCompleteClb);
	
	HAL_UART_RegisterCallback(eth_uart_handle, HAL_UART_ERROR_CB_ID, UART_ErrorClb);
	HAL_UART_RegisterCallback(bt_uart_handle, HAL_UART_ERROR_CB_ID, UART_ErrorClb);
}


void UART_startRecive(void) {
	HAL_UART_Receive_DMA(eth_uart_handle, eth_data, PAYLOAD_SIZE);
	HAL_UART_Receive_DMA(bt_uart_handle, bt_data, PAYLOAD_SIZE);
}


void UART_transmit(Command *command) {
	if (eth_uart_handle->gState != HAL_UART_STATE_READY || bt_uart_handle->gState != HAL_UART_STATE_READY) {
		warn(__FILE__, __LINE__, 0);
		return;
	}

	uint8_t txFrame[19] = { [0] = '#', [1 ... 18] = 'X' };

	knrFrame_encode(&command->ID, txFrame + 1, 1);
	// no info about frame length from CAN 
	knrFrame_encode(command->payload, txFrame + 3, 8);

	HAL_UART_Transmit_DMA(eth_uart_handle, txFrame, PAYLOAD_SIZE);
	HAL_UART_Transmit_DMA(bt_uart_handle, txFrame, PAYLOAD_SIZE);
}


static void UART_TXCompleteClb(UART_HandleTypeDef *huart) {(void) huart;}


static void UART_RXRecoverClb(UART_HandleTypeDef *huart) {
	uint8_t *buff;

	if (huart == eth_uart_handle) {
		buff = eth_data;
	} else {
		buff = bt_data;
	}

	HAL_UART_RegisterCallback(huart, HAL_UART_RX_COMPLETE_CB_ID, UART_RXCompleteClb);
	HAL_UART_Receive_DMA(huart, buff, PAYLOAD_SIZE);
}


static void UART_RXCompleteClb(UART_HandleTypeDef *huart) {
	Command command = { 0 };
	uint8_t *buff;

	WATCHDOG_UART_Refresh();

	if (huart == eth_uart_handle) {
		buff = eth_data;
	} else {
		buff = bt_data;
	}

	if (buff[0] != '#') {
		warn(__FILE__, __LINE__, 0);
		HAL_UART_RegisterCallback(huart, HAL_UART_TX_COMPLETE_CB_ID, UART_RXRecoverClb);

		uint8_t i = 0;
		while (i<18 && (buff[i] != '#')) {i++;}
		HAL_UART_Receive_DMA(huart, buff, i);

		return;
	}

	knrFrame_decode(buff + 1, &command.ID, 1);
	knrFrame_decode(buff + 3, command.payload, 8);

	if (command.ID == 45) {
		CAM_setCamerasSource(command.payload[0], command.payload[1], command.payload[2]);
		HAL_UART_Receive_DMA(huart, buff, PAYLOAD_SIZE);
		return;
	}

	CAN_transmit(&command);

	HAL_UART_Receive_DMA(huart, buff, PAYLOAD_SIZE);
}


static void UART_ErrorClb(UART_HandleTypeDef *huart) {
	(void) huart;
	error(__FILE__, __LINE__, huart->ErrorCode);
}


void knrFrame_encode(uint8_t *data, uint8_t *encoded, uint8_t data_length) {
	const uint8_t HEX_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
			'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	for (uint8_t i = 0; i < data_length; i++) {
		encoded[i * 2] = HEX_DIGITS[(data[i] >> 4)];
		encoded[i * 2 + 1] = HEX_DIGITS[(data[i] & 0x0F)];
	}
}


uint8_t knrFrame_decode(uint8_t *encoded, uint8_t *data, uint8_t data_length) {
	for (uint8_t i = 0; i < data_length; i++) {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
		// decode major half 
		switch (encoded[i * 2]) {
		case '0' ... '9':
			data[i] = (encoded[i * 2] - 0x30) << 4;
			break;

		case 'A' ... 'F':
			data[i] = (encoded[i * 2] - 0x37) << 4;
			break;

		case 'X': // end of message
			return i;

		default:
			return 0;
		}
#pragma GCC diagnostic pop //-Wconversion

		// decode minor half
		switch (encoded[i * 2 + 1]) {
		case '0' ... '9':
			data[i] += (encoded[i * 2 + 1] - 0x30);
			continue;

		case 'A' ... 'F':
			data[i] += (encoded[i * 2 + 1] - 0x37);
			continue;

		default:
			return 0;
		}
	}
	return data_length;
}

