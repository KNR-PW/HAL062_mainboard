#ifndef MODULE_LAMP_LAMP_H
#define MODULE_LAMP_LAMP_H


#define LED1_PIN 111
#define LED2_PIN 112
#define LED3_PIN 113
#define LED_PORT GPIOC


void GPIO_init(void);

void handleLED(uint8_t *data);

#endif