/**
 * @file leds.h
 * @brief Provides high level leds driver.
 */

#ifndef MODULES_LEDS_LEDS_H
#define MODULES_LEDS_LEDS_H

#define LED_1	GPIO_PIN_2
#define LED_2	GPIO_PIN_3
#define LED_3	GPIO_PIN_4
#define LED_4	GPIO_PIN_5
#define LED_5	GPIO_PIN_6

// @brief LEDs ID structure
typedef enum{
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
}Leds_ID;

// @brief Initialize high level board's led driver.
void Leds_init(void);

// @brief Turns on particular led.
// @param ledId Led ID
void Leds_turnOnLed(Leds_ID ledId);

// @brief Tursn off particular led.
// @param ledId Led ID
void Leds_turnOffLed(Leds_ID ledId);

// @brief Toggles particular led.
// @param ledId Led ID
void Leds_toggleLed(Leds_ID ledId);

// @brief Turns on every led during board initialization.
void Leds_welcomeFLash(void);

#endif // MODULES_LEDS_LEDS_H
