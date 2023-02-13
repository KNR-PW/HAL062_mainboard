/*
 * Can.h
 *
 *  Created on: 19 gru 2022
 *      Author: Jacek
 */

#ifndef MODULES_CAN_CAN_H_
#define MODULES_CAN_CAN_H_

void MX_FDCAN1_Init(void);

void COM_LoadCommandList(void);

//void COM_RunCanAction();
void COM_RunUartAction();
void sendTest();

typedef struct
{
	uint8_t ID;
	uint8_t lenght;
	uint8_t data[8];
}MessageTypeDef;

typedef struct command{
	uint8_t ID;
	uint8_t length;
	void (*reciveUartAction) ();
	void (*reciveCanAction) ();
}command;


#endif /* MODULES_CAN_CAN_H_ */
