#ifndef CUBE_INTERFACE_H
#define CUBE_INTERFACE_H

#include "stm32h7xx_hal.h"


/* External variables --------------------------------------------------------*/
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;
extern IWDG_HandleTypeDef hiwdg1;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim7;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;


/* Peripherals declarations --------------------------------------------------*/
void Cube_SystemClock_Config(void);
// void Cube_MX_GPIO_Init(void);
void Cube_MX_FDCAN1_Init(void);
void Cube_MX_FDCAN2_Init(void);
void Cube_MX_USART1_UART_Init(void);
void Cube_MX_IWDG1_Init(void);
void Cube_MX_USART3_UART_Init(void);
void Cube_MX_TIM4_Init(void);
void Cube_MX_TIM7_Init(void);
void Cube_MX_DMA_Init(void);

void Cube_Error_Handler(void);


/* Interrupts declarations ---------------------------------------------------*/
void Cube_SysTick_Handler(void);


#endif /* CUBE_INTERFACE_H */
