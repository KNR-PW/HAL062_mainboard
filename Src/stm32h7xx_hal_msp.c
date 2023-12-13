
#include <stm32h7xx_hal.h>
#include "error_handlers/error_handlers.h"


extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;


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


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(huart->Instance==USART3)
  {

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();


    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    PD11     ------> USART3_CTS
    PD12     ------> USART3_RTS
    */
//    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Stream0;
    hdma_usart3_rx.Init.Request = DMA_REQUEST_USART3_RX;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_usart3_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_usart3_rx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_usart3_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(huart,hdmarx,hdma_usart3_rx);

    /* USART3_TX Init */
    hdma_usart3_tx.Instance = DMA1_Stream1;
    hdma_usart3_tx.Init.Request = DMA_REQUEST_USART3_TX;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
    hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_usart3_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_usart3_tx.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_usart3_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(huart,hdmatx,hdma_usart3_tx);

  }else if(huart->Instance==USART1){

	  /* TODO NIE JESTEM PEWIEN CZY ZEGARY DOBRZE */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	  {
		Error_Handler();
	  }

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();


    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART3 DMA Init */
	/* USART3_RX Init */
	hdma_usart1_rx.Instance = DMA2_Stream0;
	hdma_usart1_rx.Init.Request = DMA_REQUEST_USART1_RX;
	hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_rx.Init.Mode = DMA_NORMAL;
	hdma_usart1_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
	hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	hdma_usart1_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_usart1_rx.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_usart1_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
	if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK){
	  Error_Handler();
	}

	__HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);

	/* USART3_TX Init */
	hdma_usart1_tx.Instance = DMA2_Stream1;
	hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
	hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart1_tx.Init.Mode = DMA_NORMAL;
	hdma_usart1_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
	hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	hdma_usart1_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	hdma_usart1_tx.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_usart1_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
	if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
	{
	  Error_Handler();
	}

	__HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);

  }

}


void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_12);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(huart->hdmarx);
    HAL_DMA_DeInit(huart->hdmatx);

  }else if(huart->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(huart->hdmarx);
    HAL_DMA_DeInit(huart->hdmatx);

  }

}
