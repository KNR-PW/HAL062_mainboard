#ifndef MODULES_CAN_CAN_H_
#define MODULES_CAN_CAN_H_

/* NOT USED */
//void COM_LoadCommandList(void);
//void sendTest();


void Can_testMessage(void);
void transferToCan1(void);
void transferToCan2(void);

//void COM_RunCanAction();
void COM_RunUartAction();


/* MessageTypeDef struct for handling received UART data */
typedef struct
{
	uint8_t ID;
	uint8_t lenght;
	uint8_t data[8];
}MessageTypeDef;

/* TODO NOT USED */
typedef struct command{
	uint8_t ID;
	uint8_t length;
	void (*reciveUartAction) ();
	void (*reciveCanAction) ();
}command;


#endif /* MODULES_CAN_CAN_H_ */
