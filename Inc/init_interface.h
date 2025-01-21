#ifndef INIT_INTERFACE_H
#define INIT_INTERFACE_H

void Init_SystemClock_Config(void);
void Init_MX_GPIO_Init(void);
void Init_MX_FDCAN1_Init(void);
void Init_MX_FDCAN2_Init(void);
void Init_MX_USART1_UART_Init(void);
void Init_MX_IWDG1_Init(void);
void Init_MX_USART3_UART_Init(void);

void Init_Error_Handler(void);

#endif /* INIT_INTERFACE_H */
