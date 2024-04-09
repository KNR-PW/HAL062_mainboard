/**
 ******************************************************************************
 * @file          	: communication.h
 * @author        	: Krystian Czechowicz, Adam Rybojad
 * @brief         	: Wireless communications functionality
 * @details			: This file contains both ethernet and bluetooth
 * 					  UART configuration. To undarstad it is required to see
 * 					  frame documentation. (You can see in communication.c)
 ******************************************************************************
 */


#ifndef MODULE_COMMUNICATION_COMMUNICATION_H
#define MODULE_COMMUNICATION_COMMUNICATION_H


/* Includes ------------------------------------------------------------------*/

#include <stm32h7xx_hal.h>
#include <stdbool.h>

/* Structures -----------------------------------------------------------------*/

struct commands{
	uint8_t messages[20][19];
	uint8_t numberOfCommands;
};

/* Functions ------------------------------------------------------------------*/

/**
 * *******************************************************************************
 * @brief	:	UART ethernet initialization
 * *******************************************************************************
*/
bool Eth_Init();


/**
 * *******************************************************************************
 * @brief	:	UART bluetooth initialization
 * *******************************************************************************
*/
bool BT_Init();

/**
 * *******************************************************************************
 * @brief		:	Sending data threw bluetooth
 * @params ID	:	ID of data (check frame documentation)
 * @params info	:	Information to send (check frame documentation)
 * *******************************************************************************
*/
bool BT_sendData(char *ID, char *info);

/**
 * *******************************************************************************
 * @brief		:	Sending data threw ethernet
 * @params ID	:	ID of data (check frame documentation)
 * @params info	:	Information to send (check frame documentation)
 * *******************************************************************************
*/
bool Eth_sendData(char *ID, char *info);

/**
 * *******************************************************************************
 * @brief				:	Decoding data from UART
 * @params rawMessage	:	message from UART (bt or eth)
 * *******************************************************************************
*/
void UART_Decode(uint8_t* rawMessage);

/**
 * *******************************************************************************
 * @brief				:	Begin listening on ethernet UART
 * *******************************************************************************
*/
bool Eth_ReceiveData();

/**
 * *******************************************************************************
 * @brief				:	Begin listening on bluetooth UART
 * *******************************************************************************
*/
bool BT_ReceiveData() ;


#endif // MODULE_COMMUNICATION_COMMUNICATION_H
