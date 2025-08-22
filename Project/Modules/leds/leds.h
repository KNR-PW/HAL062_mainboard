#ifndef PROJECT_MODULES_LEDS_LEDS_H
#define PROJECT_MODULES_LEDS_LEDS_H

#include <stm32h7xx_hal.h>

#define LED_1	GPIO_PIN_2 // alive
#define LED_2	GPIO_PIN_3 // debug/error
#define LED_3	GPIO_PIN_4
#define LED_4	GPIO_PIN_5 // main can bus
#define LED_5	GPIO_PIN_6 // ext can bus
#define LED_ALL		LED_1 | LED_2 | LED_3 | LED_4 | LED_5
#define LED_PORT GPIOG

#define LED_TURN_ON(led)	HAL_GPIO_WritePin(LED_PORT, led, 0x01u)
#define LED_TURN_OFF(led)	HAL_GPIO_WritePin(LED_PORT, led, 0x00u)
#define LED_TOGGLE(led)		HAL_GPIO_TogglePin(LED_PORT, led)

#endif // PROJECT_MODULES_LEDS_LEDS_H

