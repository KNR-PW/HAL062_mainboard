#include <stm32h7xx_hal_conf.h>
#include <stm32h7xx_hal.h>

enum ETH_STATUS {
	ETH_OK = 0,
	ETH_ERROR = 1
};

enum ETH_STATUS eth_init(USART_TypeDef* uart_instance);

