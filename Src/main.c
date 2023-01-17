/**
 ******************************************************************************
 * @file           : main.c
 * @author         : TODO: Add credits
 * @brief          : Main program body
 ******************************************************************************
 */
#include <stdbool.h>
#include "ethernet/eth.h"
#include "leds/leds.h"

TIM_HandleTypeDef htim2;

void SysTick_Handler(void) {
	HAL_IncTick();
}

int main(void) {
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_Init();

	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};

	  /** Supply configuration update enable
	  */
	  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	  /** Configure the main internal regulator output voltage
	  */
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
	    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	    RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
	    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	    RCC_OscInitStruct.PLL.PLLM = 1;
	    RCC_OscInitStruct.PLL.PLLN = 18;
	    RCC_OscInitStruct.PLL.PLLP = 2;
	    RCC_OscInitStruct.PLL.PLLQ = 3;
	    RCC_OscInitStruct.PLL.PLLR = 2;
	    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
	    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOMEDIUM;
	    RCC_OscInitStruct.PLL.PLLFRACN = 6144;
	    HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
							  |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV8;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;



  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

	Leds_init();
	Leds_welcomeFLash();

	Eth_Init();
	Eth_ReceiveData();

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 80 - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 2000-1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&htim2);
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim2);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 220;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**TIM2 GPIO Configuration
	 PA0     ------> TIM2_CH1
	 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	/* Loop forever */
	while (1) {
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,100);
		HAL_Delay(1000);
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,200);
		HAL_Delay(1000);

//		Leds_toggleLed(LED5);
//		HAL_Delay(1000);
//		Leds_toggleLed(LED5);
//		HAL_Delay(1000);
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
	  if(htim->Instance==TIM2)
	  {
	  /* USER CODE BEGIN TIM2_MspInit 0 */

	  /* USER CODE END TIM2_MspInit 0 */
	    /* Peripheral clock enable */
	    __HAL_RCC_TIM2_CLK_ENABLE();
	  /* USER CODE BEGIN TIM2_MspInit 1 */

	  /* USER CODE END TIM2_MspInit 1 */
	  }

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspInit could be implemented in the user file
   */
}
