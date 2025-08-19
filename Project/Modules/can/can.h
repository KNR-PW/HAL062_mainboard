#ifndef PROJECT_MODULES_CAN_CAN_H
#define PROJECT_MODULES_CAN_CAN_H

#include "stm32h7xx.h"
#include "communication/data.h"

void CAN_init(void);
void CAN_startRecive(void);
void CAN_transmit(Command *command);


#endif //PROJECT_MODULES_CAN_CAN_H
