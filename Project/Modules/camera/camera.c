#include "camera.h"

static TIM_HandleTypeDef* camTim;

static CameraSwitches cameras = {
		.redCamera=SOURCE_1,
		.yellowCamera=SOURCE_1,
		.blueCamera = SOURCE_1
		};


static void CAM_setCameraPwmLow(TIM_HandleTypeDef *htim);
static void CAM_setCameraPwmHigh(TIM_HandleTypeDef *htim);


void CAM_setCamerasTimerPtr(TIM_HandleTypeDef *htim)
{
	camTim = htim;
}

void CAM_registerCallbacks(void)
{
	HAL_TIM_RegisterCallback(camTim, HAL_TIM_PERIOD_ELAPSED_CB_ID, CAM_setCameraPwmHigh);
	HAL_TIM_RegisterCallback(camTim, HAL_TIM_OC_DELAY_ELAPSED_CB_ID, CAM_setCameraPwmLow);
}

void CAM_start(void)
{
	  HAL_TIM_Base_Start_IT(camTim);
	  HAL_TIM_OC_Start_IT(camTim, TIM_CHANNEL_1);
	  HAL_TIM_OC_Start_IT(camTim, TIM_CHANNEL_2);
	  HAL_TIM_OC_Start_IT(camTim, TIM_CHANNEL_3);
}

void CAM_setCamerasSource(CameraSource blueCam, CameraSource redCam, CameraSource yellowCam) {
	cameras.redCamera = redCam;
	cameras.yellowCamera = yellowCam;
	cameras.blueCamera = blueCam;
}


/* =========================================================================================
 * ================================= STATIC FUNCTIONS ======================================
 * =========================================================================================*/


static void CAM_setCameraPwmHigh(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_WritePin(CAM_PORT, CAM_RED_Pin, 1);
	HAL_GPIO_WritePin(CAM_PORT, CAM_YELLOW_Pin, 1);
	HAL_GPIO_WritePin(CAM_PORT, CAM_BLUE_Pin, 1);
}

static void CAM_setCameraPwmLow(TIM_HandleTypeDef *htim)
{
	switch (HAL_TIM_GetActiveChannel(camTim)) {
	    case HAL_TIM_ACTIVE_CHANNEL_1:
	    	if (cameras.redCamera == SOURCE_1) {
	    		HAL_GPIO_WritePin(CAM_PORT, CAM_RED_Pin, 0);
	    	}
	    	if (cameras.yellowCamera == SOURCE_1) {
	    		HAL_GPIO_WritePin(CAM_PORT, CAM_YELLOW_Pin, 0);
	    	}
	    	if (cameras.blueCamera == SOURCE_1) {
	    		HAL_GPIO_WritePin(CAM_PORT, CAM_BLUE_Pin, 0);
	    	}
	      break;
	    case HAL_TIM_ACTIVE_CHANNEL_2:
	    	if (cameras.redCamera == SOURCE_2) {
	    		HAL_GPIO_WritePin(CAM_PORT, CAM_RED_Pin, 0);
	    	}
	    	if (cameras.yellowCamera == SOURCE_2) {
	    		HAL_GPIO_WritePin(CAM_PORT, CAM_YELLOW_Pin, 0);
	    	}
	    	if (cameras.blueCamera == SOURCE_2) {
	    		HAL_GPIO_WritePin(CAM_PORT, CAM_BLUE_Pin, 0);
	    	}
	      break;
	    case HAL_TIM_ACTIVE_CHANNEL_3:
	    	if (cameras.redCamera == SOURCE_3) {
	    		HAL_GPIO_WritePin(CAM_PORT, CAM_RED_Pin, 0);
	    	}
	    	if (cameras.yellowCamera == SOURCE_3) {
	    		HAL_GPIO_WritePin(CAM_PORT, CAM_YELLOW_Pin, 0);
	    	}
	    	if (cameras.blueCamera == SOURCE_3) {
	    		HAL_GPIO_WritePin(CAM_PORT, CAM_BLUE_Pin, 0);
	    	}
	      break;
	    default:
	      break;
	    }
}
