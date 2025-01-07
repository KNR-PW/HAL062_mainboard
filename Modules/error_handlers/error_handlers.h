/**
 ******************************************************************************
 * @file           : error_handlers.h
 * @author         : Jacek Prokopczuk, Krystian Czechowicz, Adam Rybojad
 * @brief          : Handling all errors that is detected TODO
 ******************************************************************************
 */

#ifndef ERROR_HANDLERS_ERROR_HANDLERS_H_
#define ERROR_HANDLERS_ERROR_HANDLERS_H_

#define MAINBOARD_ERROR_ID ((uint8_t) 70)
#define DEBUG_LEDS

typedef enum {
	MAINError_test,
	CAN1Error_init,
	CAN1Error_configFilter,
	CAN1Error_start,
	CAN1Error_confPerifCLK,

	CAN1Error_transferAddMessege,
	CAN1Error_transferEnableTx,

	CAN1Error_fifoGetMessege,
	CAN1Error_fifoActivateNotification,

	CAN2Error_init,
	CAN2Error_configFilter,
	CAN2Error_start,
	CAN2Error_confPerifCLK,

	CAN2Error_transferAddMessege,
	CAN2Error_transferEnableTx,

	CAN2Error_fifoGetMessege,
	CAN2Error_fifoActivateNotification,

	COMError_watchdogInit,

	COMError_Bt,
	COMError_BtPeriphClock,

	COMError_Eth,
	COMError_EthPeriphClock,
	COMError_EthRX,
	COMError_EthTX,

	TIM4Error_baseInit,
	TIM4Error_configClock,
	TIM4Error_OCInit,
	TIM4Error_configSync,
	TIM4Error_configChannel,
	TIM7Error_baseInit,
	TIM7Error_configSync,
	TIM16Error_baseInit,

	SysClkError_mainOscConfig,
	SysClkError_mainClockConfig,
} Error_code;

typedef enum {
	ErrorHandler_noErrorFunc,
	MAINEErrorFunc_test,
	CAN1ErrorFunc_init,
	CAN1ErrorFunc_transfer,
	CAN1ErrorFunc_fifo,

	CAN2ErrorFunc_init,
	CAN2ErrorFunc_transfer,
	CAN2ErrorFunc_fifo,

	COMErrorFunc_watchdogInit,
	COMErrorFunc_BtInit,
	COMErrorFunc_EthInit,
	COMErrorFunc_Bt,
	COMErrorFunc_Eth,

	TIM4ErrorFunc_init,
	TIM7ErrorFunc_init,
	TIM16ErrorFunc_init,

	SysClkErrorFunc_init,
} Error_function;

/* Functions ----------------------------------------------------------------- */

/** 
 ****************************************************************************** 
 * Function to handle the errors that may occur during the runtime 
 * tries to restore the functional state of the program, most often by redoing 
 * some initialization steps
 *
 * The error handling logic is located in the switch-case construction, 
 * each value of Error_function enum corresponds its own case.
 *
 * USAGE:
 *    Error_Handler(Error_function, Error_code);
 *    return; 
 *
 * It is IMPORTANT to return from function after calling Error_Handler,
 * failure to do so will lead to unexpected behavior.
 *
 * @param error_func  solely responsible for the behavior of the error_handler 
 * Does Not influence the error code sent outside via coms
 *
 * @param error_code  solely responsible for the error code sent outside
 * Does Not influence the behavior of error handling  
 ****************************************************************************** 
 */
void Error_Handler(Error_function error_func, Error_code error_code);

#endif /* ERROR_HANDLERS_ERROR_HANDLERS_H_ */
