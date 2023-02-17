/**
 ******************************************************************************
 * @file           : main.c
 * @author         : TODO: Add credits
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdbool.h>
#include "ethernet/eth.h"

#include <stm32h7xx_hal.h>
#include <stm32h7xx_hal_conf.h>
#include <stdbool.h>

#include "leds/leds.h"
#include "can/can.h"
#include "error_handlers/error_handlers.h"



void SystemClock_Config(void);

//#define USE_HAL_UART_REGISTER_CALLBACKS

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

int main(void) {

	__HAL_RCC_GPIOB_CLK_ENABLE();
	HAL_Init();
	SystemClock_Config();

	Leds_init();
	MX_FDCAN1_Init();
	Leds_welcomeFLash();

	Eth_Init();
	Eth_ReceiveData();

	BT_Init();
	BT_ReceiveData();

	/* Loop forever */
	while (1) {
//		Leds_toggleLed(LED5);
//		HAL_Delay(1000);
//		Leds_toggleLed(LED5);
//		HAL_Delay(1000);
	// Error_Handler();
		//Can_testMessage();
		// HAL_Delay(50);
	}
}

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
	}
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 18;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOMEDIUM;
	RCC_OscInitStruct.PLL.PLLFRACN = 6144;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1
			| RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}
