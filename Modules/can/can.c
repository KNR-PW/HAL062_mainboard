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

FDCAN_FilterTypeDef sFilterConfig;
FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_TxHeaderTypeDef TxHeader_CAN2;
FDCAN_RxHeaderTypeDef RxHeader;
uint8_t TxData_Node1_To_Node2[] = { 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xFA, 0xFB,
		0xFC };
uint8_t RxData_From_Node2[12];

static MessageTypeDef UART_MessageRecieved;

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;


command commandList[150];
int speed_left;
int speed_right;

void Error_Handler(void);

void MX_FDCAN1_Init(void) {

	hfdcan1.Instance = FDCAN2;
	hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan1.Init.AutoRetransmission = DISABLE;
	hfdcan1.Init.TransmitPause = DISABLE;
	hfdcan1.Init.ProtocolException = DISABLE;
	hfdcan1.Init.NominalPrescaler = 150; // 0.5 Mhz
	hfdcan1.Init.NominalSyncJumpWidth = 1;
	hfdcan1.Init.NominalTimeSeg1 = 9;
	hfdcan1.Init.NominalTimeSeg2 = 8;
	hfdcan1.Init.DataPrescaler = 150; // 0.5 MHz
	hfdcan1.Init.DataSyncJumpWidth = 1;
	hfdcan1.Init.DataTimeSeg1 = 9;
	hfdcan1.Init.DataTimeSeg2 = 8;
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

void Can_testMessage(void) {
	TxHeader.Identifier = 0x01;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0x0; // Ignore because FDCAN_NO_TX_EVENTS
	if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan1, &TxHeader,
			TxData_Node1_To_Node2, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	hfdcan1.Instance->TXBAR = 0x1u;

	/* Send Tx buffer message */
	if (HAL_FDCAN_EnableTxBufferRequest(&hfdcan1, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	/* Polling for transmission complete on buffer index 0 */
	while (HAL_FDCAN_IsTxBufferMessagePending(&hfdcan1, FDCAN_TX_BUFFER0) == 1)
		;
//	CAN_TxHeader.StdId = 20;
//	CAN_TxHeader.ExtId = 20;
//	CAN_TxHeader.IDE = CAN_ID_STD;
//	CAN_TxHeader.RTR = CAN_RTR_DATA;
//	CAN_TxHeader.DLC = 8;
//	uint8_t dane[8];
//	dane[0] = 0xAA;
//	dane[1] = 0xAA;
//	for (int i = 2; i < 8; i++) {
//		dane[i] = 0;
//	}
//	HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeader, dane, &CAN_TxMailbox);
	//Leds_toggleLed(LED4);
}

void HAL_MspInit(void) {
	__HAL_RCC_SYSCFG_CLK_ENABLE();
}

/**
 * @brief FDCAN MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hfdcan: FDCAN handle pointer
 * @retval None
 */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };
	if (hfdcan->Instance == FDCAN2) {

		/** Initializes the peripherals clock
		 */
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
		PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
			//Error_Handler();
		}

		/* Peripheral clock enable */
		__HAL_RCC_FDCAN_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**FDCAN1 GPIO Configuration
		 PB8     ------> FDCAN1_RX
		 PB9     ------> FDCAN1_TX
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/* USER CODE BEGIN FDCAN1_MspInit 1 */

		/* USER CODE END FDCAN1_MspInit 1 */
	}

}

/**
 * @brief FDCAN MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hfdcan: FDCAN handle pointer
 * @retval None
 */
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *hfdcan) {
	if (hfdcan->Instance == FDCAN1) {
		/* Peripheral clock disable */
		__HAL_RCC_FDCAN_CLK_DISABLE();

		/**FDCAN1 GPIO Configuration
		 PB8     ------> FDCAN1_RX
		 PB9     ------> FDCAN1_TX
		 */
		HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1);

	}

}


void sendTest() {
	TxHeader.Identifier = 0x01u;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0x0; // Ignore because FDCAN_NO_TX_EVENTS

//	CAN_TxHeader.StdId = 20;
//	CAN_TxHeader.ExtId = 20;
//	CAN_TxHeader.IDE = CAN_ID_STD;
//	CAN_TxHeader.RTR = CAN_RTR_DATA;
//	CAN_TxHeader.DLC = 8;
//	uint8_t dane[8];
//	dane[0] = speed_left;
//	dane[1] = speed_right;
//	for (int i = 2; i < 8; i++) {
//		dane[i] = 0;
//	}
//	HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeader, dane, &CAN_TxMailbox);
//	GPIO_LEDToggle(LED_RED);
}

void transferTo() {
	if (UART_MessageRecieved.ID > 0 && UART_MessageRecieved.ID < 128)
		transferToCan1();
	else if (UART_MessageRecieved.ID > 127 && UART_MessageRecieved.ID < 256)
		transferToCan2();
	return;
}

void COM_RunUartAction(MessageTypeDef* message) {
	UART_MessageRecieved.ID = message->ID;
	memcpy(UART_MessageRecieved.data, message->data, 8);
	UART_MessageRecieved.lenght = message->lenght;
	transferTo();
	/*
	 UART_MessageTypeDef msg;
	 msg.ID=CAN_RxHeader.StdId;
	 for (int i=0;i<8;i++)
	 {
	 msg.data[i]= CAN_RxMsg[i];
	 }
	 msg.lenght=CAN_RxHeader.DLC;

	 return;
	 */
}
//void COM_RunCanAction() {
//	transferToUart();
//}
//
//void transferToUart() {
//	MessageTypeDef msg;
//	msg.ID = CAN_RxHeader.StdId;
//	for (int i = 0; i < 8; i++) {
//		msg.data[i] = CAN_RxMsg[i];
//	}
//	msg.lenght = CAN_RxHeader.DLC;
//	UART3_Send(msg);
//}

void transferToCan1() {
	TxHeader.Identifier = 0x01u;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_ON;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0x0; // Ignore because FDCAN_NO_TX_EVENTS

	if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan1, &TxHeader,
			UART_MessageRecieved.data, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	hfdcan1.Instance->TXBAR = 0x1u;

	/* Send Tx buffer message */
	if (HAL_FDCAN_EnableTxBufferRequest(&hfdcan1, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	/* Polling for transmission complete on buffer index 0 */
	while (HAL_FDCAN_IsTxBufferMessagePending(&hfdcan1, FDCAN_TX_BUFFER0) == 1)
		;

//	CAN_TxHeader.StdId = MessageRecieved.ID;
//	CAN_TxHeader.ExtId = MessageRecieved.ID;
//	CAN_TxHeader.IDE = CAN_ID_STD;
//	CAN_TxHeader.RTR = CAN_RTR_DATA;
//	CAN_TxHeader.DLC = MessageRecieved.lenght;
//	HAL_CAN_AddTxMessage(&hcan1, &CAN_TxHeader, MessageRecieved.data,
//			&CAN_TxMailbox);
	//GPIO_LEDToggle(LED_RED);
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

	uint8_t dane[8];
	for (int i = 0; i < 8; i++) {
		dane[i] = UART_MessageRecieved.data[i];
	}

	if (HAL_FDCAN_AddMessageToTxBuffer(&hfdcan2, &TxHeader,
			dane, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	hfdcan2.Instance->TXBAR = 0x1u;

	/* Send Tx buffer message */
	if (HAL_FDCAN_EnableTxBufferRequest(&hfdcan2, FDCAN_TX_BUFFER0) != HAL_OK) {
		Error_Handler();
	}

	/* Polling for transmission complete on buffer index 0 */
	while (HAL_FDCAN_IsTxBufferMessagePending(&hfdcan2, FDCAN_TX_BUFFER0) == 1)
		;



//	CAN_TxHeader2.StdId = UART_MessageRecieved.ID;
//	CAN_TxHeader2.ExtId = UART_MessageRecieved.ID;
//	CAN_TxHeader2.IDE = CAN_ID_STD;
//	CAN_TxHeader2.RTR = CAN_RTR_DATA;
//	CAN_TxHeader2.DLC = UART_MessageRecieved.lenght;
//
//	HAL_CAN_AddTxMessage(&hcan2, &CAN_TxHeader2, dane, &CAN_TxMailbox2);
//	GPIO_LEDToggle(LED_RED);

}

void ignoreCAN() {
	return;
}

void ignoreUART() {
	return;
}