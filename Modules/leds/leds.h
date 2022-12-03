#ifndef MODULES_LEDS_LEDS_H
#define MODULES_LEDS_LEDS_H

#define LED_1	GPIO_PIN_2
#define LED_2	GPIO_PIN_3
#define LED_3	GPIO_PIN_4
#define LED_4	GPIO_PIN_5
#define LED_5	GPIO_PIN_6

typedef enum{
	LED1,
	LED2,
	LED3,
	LED4,
	LED5,
}Leds_ID;

void Leds_init(void);
void Leds_turnOnLed(Leds_ID ledId);
void Leds_turnOffLed(Leds_ID ledId);

#endif // MODULES_LEDS_LEDS_H
