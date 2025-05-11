/**
 ******************************************************************************
 * @file           : can.h
 * @author         : Jacek, Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Functionality of communication threw CAN
 ******************************************************************************
 */

#ifndef PROJECT_MODULES_CAN_CAN_H
#define PROJECT_MODULES_CAN_CAN_H

/* Includes -------------------------------------------------------------------*/
#include "communication/data.h"

/* Functions ------------------------------------------------------------------*/

void CAN_setRailCanPtr(FDCAN_HandleTypeDef* railCan);
void CAN_setExtCanPtr(FDCAN_HandleTypeDef* extCan);
void CAN_setTimerPtr(TIM_HandleTypeDef* hTim);
void CAN_registerCallbacks(void);
void CAN_railCanInit(void);
void CAN_extCanInit(void);

void CAN_addCommandToTransmit(Command* command);
void CAN_railCanStartListening(void);
void CAN_extCanStartListening(void);


#endif //PROJECT_MODULES_CAN_CAN_H
