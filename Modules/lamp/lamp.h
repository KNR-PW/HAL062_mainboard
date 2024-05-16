#ifndef MODULE_LAMP_LAMP_H
#define MODULE_LAMP_LAMP_H


#define LED1_PIN 111
#define LED2_PIN 112
#define LED3_PIN 113
#define LED_PORT GPIOC

extern TIM_HandleTypeDef htim16;
void GPIO_init(void);
void MX_TIM16_Init(void);

void handleLED(uint8_t *data);
void Set_Max_Value(uint8_t *data);
extern TIM_HandleTypeDef htim16;

#endif
