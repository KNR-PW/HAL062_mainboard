
#include <stm32h7xx_hal.h>

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();

}

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED=0;

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* hfdcan)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hfdcan->Instance==FDCAN1)
   {
     PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
     PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
     if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
     {
       Error_Handler();
     }

     /* Peripheral clock enable */
     HAL_RCC_FDCAN_CLK_ENABLED++;
     if(HAL_RCC_FDCAN_CLK_ENABLED==1){
       __HAL_RCC_FDCAN_CLK_ENABLE();
     }

     __HAL_RCC_GPIOD_CLK_ENABLE();
     /**FDCAN1 GPIO Configuration
     PD0     ------> FDCAN1_RX
     PD1     ------> FDCAN1_TX
     */
     GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
     GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
     HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

     /* FDCAN1 interrupt Init */
     HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
     HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
     HAL_NVIC_SetPriority(FDCAN_CAL_IRQn, 0, 0);
     HAL_NVIC_EnableIRQ(FDCAN_CAL_IRQn);

   }
   else if(hfdcan->Instance==FDCAN2)
  {
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_FDCAN;
    PeriphClkInitStruct.FdcanClockSelection = RCC_FDCANCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN2 GPIO Configuration
    PB5     ------> FDCAN2_RX
    PB6     ------> FDCAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN2 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);
    HAL_NVIC_SetPriority(FDCAN_CAL_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN_CAL_IRQn);
  }

}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* hfdcan)
{
	  if(hfdcan->Instance==FDCAN1)
	  {

	    /* Peripheral clock disable */
	    HAL_RCC_FDCAN_CLK_ENABLED--;
	    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
	      __HAL_RCC_FDCAN_CLK_DISABLE();
	    }

	    /**FDCAN1 GPIO Configuration
	    PD0     ------> FDCAN1_RX
	    PD1     ------> FDCAN1_TX
	    */
	    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

	    /* FDCAN1 interrupt DeInit */
	    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
	    HAL_NVIC_DisableIRQ(FDCAN1_IT1_IRQn);

	  }
	else if(hfdcan->Instance==FDCAN2)
  {
    /* Peripheral clock disable */
	    HAL_RCC_FDCAN_CLK_ENABLED--;
	    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
	      __HAL_RCC_FDCAN_CLK_DISABLE();
	    }

    /**FDCAN2 GPIO Configuration
    PB5     ------> FDCAN2_RX
    PB6     ------> FDCAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_6);

    /* FDCAN2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN2_IT1_IRQn);
    HAL_NVIC_DisableIRQ(FDCAN_CAL_IRQn);
  }

}
