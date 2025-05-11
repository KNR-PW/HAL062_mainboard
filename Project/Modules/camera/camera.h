#ifndef PROJECT_MODULES_CAMERA_CAMERA_H
#define PROJECT_MODULES_CAMERA_CAMERA_H

#include <stm32h7xx_hal.h>

#define CAM_BLUE_Pin GPIO_PIN_0
#define CAM_YELLOW_Pin GPIO_PIN_1
#define CAM_RED_Pin GPIO_PIN_2
#define CAM_PORT GPIOC

typedef enum CameraSource
{
	SOURCE_1 = 1,
	SOURCE_2 = 2,
	SOURCE_3 = 3,
} CameraSource;

typedef struct CameraSwitches
{
	CameraSource redCamera;
	CameraSource yellowCamera;
	CameraSource blueCamera;
} CameraSwitches;

void CAM_setCamerasTimerPtr(TIM_HandleTypeDef *htim);
void CAM_registerCallbacks(void);
void CAM_start(void);

void CAM_setCamerasSource(CameraSource redCam, CameraSource yellowCam, CameraSource blueCam);

#endif // PROJECT_MODULES_CAMERA_CAMERA_H
