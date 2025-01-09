#include <stm32h7xx_hal.h>
#include <stm32h7xx_it.h>

#include "leds/leds.h"
#include "can/can.h"
#include "communication/communication.h"

#ifdef FEATURE_LED_ALIVE_DISPLAY
bool alive_display_state = 1;
static uint16_t work_led_count = 0;
#endif

extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan1;
extern UART_HandleTypeDef ethHuart;
extern UART_HandleTypeDef btHuart;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

void NMI_Handler(void) {
	while (1) {
	}
}

void HardFault_Handler(void) {
	while (1) {
	}
}

void MemManage_Handler(void) {
	while (1) {
	}
}

void BusFault_Handler(void) {
	while (1) {
	}
}

void UsageFault_Handler(void) {
	while (1) {
	}
}

void SVC_Handler(void) {

}

void DebugMon_Handler(void) {

}

void PendSV_Handler(void) {

}

void SysTick_Handler(void) {
	HAL_IncTick();

#ifdef FEATURE_LED_ALIVE_DISPLAY

	if (alive_display_state && work_led_count >= 500) {
		work_led_count = 0;
		Leds_toggle(LED_1);
	}
	work_led_count++;

#endif
}

void FDCAN1_IT0_IRQHandler(void) {
	HAL_FDCAN_IRQHandler(&hfdcan1);
}

void FDCAN2_IT0_IRQHandler(void) {
	HAL_FDCAN_IRQHandler(&hfdcan2);
}

void FDCAN1_IT1_IRQHandler(void) {
	HAL_FDCAN_IRQHandler(&hfdcan1);
}

void FDCAN2_IT1_IRQHandler(void) {
	HAL_FDCAN_IRQHandler(&hfdcan2);
}

void FDCAN_CAL_IRQHandler(void) {
	HAL_FDCAN_IRQHandler(&hfdcan2);
	HAL_FDCAN_IRQHandler(&hfdcan1);
}

void USART1_IRQHandler(void) {
	HAL_UART_IRQHandler(&ethHuart);
}

void USART3_IRQHandler(void) {
	HAL_UART_IRQHandler(&btHuart);
}

void TIM7_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim7);
}

void TIM4_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim4);
}

void DMA_STR0_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

void DMA_STR1_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

