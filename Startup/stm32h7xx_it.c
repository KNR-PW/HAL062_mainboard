#include <stm32h7xx_hal.h>
#include <stm32h7xx_it.h>
#include <stdbool.h>

#include "leds/leds.h"
#include "can/can.h"

extern UART_HandleTypeDef ethHuart;
extern UART_HandleTypeDef btHuart;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;

extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan1;

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
	static int work_led_cnt = 0;
	static bool work_led_state = false;

	HAL_IncTick();

	if (work_led_state && work_led_cnt >= 100) {
		work_led_cnt = 0u;
		work_led_state = false;
		Leds_turnOffLed(LED1);
	}
	if (!work_led_state && work_led_cnt >= 400) {
		work_led_cnt = 0u;
		work_led_state = true;
		Leds_turnOnLed(LED1);
	}
	work_led_cnt++;
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

void USART1_IRQHandler() {
	HAL_UART_IRQHandler(&ethHuart);
}

void USART3_IRQHandler() {
	HAL_UART_IRQHandler(&btHuart);
}


/*****************************************************/
/* POTENCJALNY BUG TODO */

void DMA_STR0_IRQHandler(void) {

	HAL_DMA_IRQHandler(&hdma_usart1_rx);

	__HAL_DMA_CLEAR_FLAG(&hdma_usart1_rx,
			__HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart1_rx));

	HAL_DMA_IRQHandler(&hdma_usart3_rx);

		__HAL_DMA_CLEAR_FLAG(&hdma_usart3_rx,
				__HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart3_rx));
}

void DMA_STR1_IRQHandler(void) {

	HAL_DMA_IRQHandler(&hdma_usart3_tx);

	__HAL_DMA_CLEAR_FLAG(&hdma_usart3_tx,
			__HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart3_tx));

	HAL_DMA_IRQHandler(&hdma_usart1_tx);

		__HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx,
				__HAL_DMA_GET_TC_FLAG_INDEX(&hdma_usart1_tx));
}
