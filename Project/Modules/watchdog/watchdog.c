#include "watchdog/watchdog.h"
#include "errorHandlers/errorHandlers.h"

extern IWDG_HandleTypeDef hiwdg1;
static IWDG_HandleTypeDef *watchdog_handle = &hiwdg1;

extern TIM_HandleTypeDef htim7;
static TIM_HandleTypeDef *timer_handle = &htim7;

static uint8_t uart_counter = 0;

static void timer_PeriodElapsedClb(TIM_HandleTypeDef *htim);

void WATCHDOG_init(void) {
	HAL_TIM_RegisterCallback(timer_handle, HAL_TIM_PERIOD_ELAPSED_CB_ID, timer_PeriodElapsedClb);
	HAL_TIM_Base_Start_IT(timer_handle);
}


void WATCHDOG_UART_Refresh(void) {
	uart_counter = 0;
}

static void timer_PeriodElapsedClb(TIM_HandleTypeDef* htim) {
#ifdef RESET_ON_COMS_LOSS
	if (uart_counter > 30) {error(0, 0, 0);}
	uart_counter++;
#endif

	HAL_IWDG_Refresh(watchdog_handle);
}
