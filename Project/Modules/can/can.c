#include "leds/leds.h"
#include "can/can.h"

extern FDCAN_HandleTypeDef hfdcan2;
static FDCAN_HandleTypeDef *rail_can_handle = &hfdcan2;

extern FDCAN_HandleTypeDef hfdcan1;
static FDCAN_HandleTypeDef *ext_can_handle = &hfdcan1;

static FDCAN_TxHeaderTypeDef railCanTxHeader;
static FDCAN_TxHeaderTypeDef extCanTxHeader;

static void CAN_setupTxHeaders(void);
static void CAN_setupFilterConfigs(void);

static void CAN_TXCompleteClb(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
static void CAN_RXCompleteClb(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs);
static void CAN_errorClb(FDCAN_HandleTypeDef *hfdcan, uint32_t status);


void CAN_init(void) {
	CAN_setupTxHeaders();
	CAN_setupFilterConfigs();

	HAL_FDCAN_RegisterRxFifo0Callback(rail_can_handle, CAN_RXCompleteClb);
	HAL_FDCAN_RegisterRxFifo0Callback(ext_can_handle, CAN_RXCompleteClb);

	HAL_FDCAN_RegisterTxBufferCompleteCallback(rail_can_handle, CAN_TXCompleteClb);
	HAL_FDCAN_RegisterTxBufferCompleteCallback(ext_can_handle, CAN_TXCompleteClb);

	HAL_FDCAN_RegisterErrorStatusCallback(rail_can_handle, CAN_errorClb);
	HAL_FDCAN_RegisterErrorStatusCallback(ext_can_handle, CAN_errorClb);
}


void CAN_startRecive(void) {
	HAL_FDCAN_ActivateNotification(rail_can_handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	HAL_FDCAN_ActivateNotification(ext_can_handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}


void CAN_transmit(Command *command) {
	FDCAN_HandleTypeDef *handle;
	FDCAN_TxHeaderTypeDef *header;

	if (command->ID > 0 && command->ID < 128){
		handle = rail_can_handle;
		header = &railCanTxHeader;
	} else {
		handle = ext_can_handle;
		header = &extCanTxHeader;
	}

	if (HAL_FDCAN_GetTxFifoFreeLevel(handle) != 0u) {
		LED_TOGGLE(LED_5);
		return;
	}

	railCanTxHeader.Identifier = command->ID; //< ID of message

	// I have no clue about this function checking that the CAN handle 
	// is enabled, as it checks only for HAL_FDCAN_STATE_BUSY and throws an 
	// error otherwise. If code throws HAL_FDCAN_ERROR_NOT_INITIALIZED for no 
	// reason this is the culprit
	HAL_FDCAN_AddMessageToTxFifoQ(handle, header, command->payload);
}


static void CAN_TXCompleteClb(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	(void) hfdcan;
	(void) RxFifo0ITs;
}


static void CAN_RXCompleteClb(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	(void) hfdcan;
	(void) RxFifo0ITs;

	static FDCAN_RxHeaderTypeDef canHeader = {0};
	Command command = { 0 };

	HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &canHeader, command.payload);
	command.ID = (uint8_t) canHeader.Identifier;
}


static void CAN_errorClb(FDCAN_HandleTypeDef *hfdcan, uint32_t status) {
	(void) hfdcan;
	(void) status;
}


static void CAN_setupTxHeaders(void) {
	// CAN2 frame seeting
	railCanTxHeader.IdType = FDCAN_STANDARD_ID; 			//< Standard ID is used
	railCanTxHeader.TxFrameType = FDCAN_DATA_FRAME; 	//< Frame to transfer data
	railCanTxHeader.DataLength = FDCAN_DLC_BYTES_8; 			//< 8 BYTES of data
	railCanTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	railCanTxHeader.BitRateSwitch = FDCAN_BRS_ON;
	railCanTxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	railCanTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	railCanTxHeader.MessageMarker = 0x0; 	//< Ignore because FDCAN_NO_TX_EVENTS

	// CAN1 frame seeting
	extCanTxHeader.IdType = FDCAN_STANDARD_ID; 			//< Standard ID is used
	extCanTxHeader.TxFrameType = FDCAN_DATA_FRAME; 	//< Frame to transfer data
	extCanTxHeader.DataLength = FDCAN_DLC_BYTES_8; 			//< 8 BYTES of data
	extCanTxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	extCanTxHeader.BitRateSwitch = FDCAN_BRS_ON;
	extCanTxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	extCanTxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	extCanTxHeader.MessageMarker = 0x0; 	// <Ignore because FDCAN_NO_TX_EVENTS
}



static void CAN_setupFilterConfigs(void) {
	FDCAN_FilterTypeDef sFilterConfig;

	/* Configure standard ID reception filter to Rx buffer 0 */
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_RANGE; // Ignore because FDCAN_FILTER_TO_RXBUFFER
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXBUFFER;
	sFilterConfig.FilterID1 = 0x00000000; // ID Node2
	sFilterConfig.FilterID2 = 0xFFFFFFFF; // Ignore because FDCAN_FILTER_TO_RXBUFFER
	sFilterConfig.RxBufferIndex = 0;

	if (HAL_FDCAN_ConfigFilter(rail_can_handle, &sFilterConfig) != HAL_OK) {
		LED_TOGGLE(LED_5);
	}

	/* Configure standard ID reception filter to Rx buffer 0 */
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_RANGE; // Ignore because FDCAN_FILTER_TO_RXBUFFER
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = 0x00000000; // ID Node2
	sFilterConfig.FilterID2 = 0xFFFFFFFF; // Ignore because FDCAN_FILTER_TO_RXBUFFER
	sFilterConfig.RxBufferIndex = 0;
	if (HAL_FDCAN_ConfigFilter(ext_can_handle, &sFilterConfig) != HAL_OK) {
		LED_TOGGLE(LED_5);
	}
}

