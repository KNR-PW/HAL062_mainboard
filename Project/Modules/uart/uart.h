#ifndef PROJECT_MODULES_UART_UART_H
#define PROJECT_MODULES_UART_UART_H

#include "stm32h7xx.h"
#include "communication/data.h"

#define PAYLOAD_SIZE 19

void UART_init(void);
void UART_startRecive(void);
void UART_transmit(Command *command);


#endif //PROJECT_MODULES_UART_UART_H
