#ifndef MODULES_CAN_CAN_INIT_H_
#define MODULES_CAN_CAN_INIT_H_


extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;



void FDCAN1_Init(void);
void FDCAN2_Init(void);

#endif /* MODULES_CAN_CAN_INIT_H_ */
