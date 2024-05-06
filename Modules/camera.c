void handleCamera(uint8_t *data) {
    // Sprawdzenie czy dane spełniają warunki i włączenie odpowiedniej diody
    if (data[1] == 1) {
        HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_SET);
    }
    else
    {
    	HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_RESET);
    }
    if (data[1] == 2) {
        HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_SET);
    }
    else
        {
        	HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_RESET);
        }
    if (data[1] == 3) {


        HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_SET);
    }
    else
            {
            	HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_RESET);
            }
   if (data[2] == 1) {
        HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_SET);
    }
    else
    {
    	HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_RESET);
    }
    if (data[2] == 2) {
        HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_SET);
    }
    else
        {
        	HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_RESET);
        }
    if (data[2] == 3) {


        HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_SET);
    }
    else
            {
            	HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_RESET);
            }
   if (data[3] == 1) {
        HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_SET);
    }
    else
    {
    	HAL_GPIO_WritePin(LED_PORT, LED1_PIN, GPIO_PIN_RESET);
    }
    if (data[3] == 2) {
        HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_SET);
    }
    else
        {
        	HAL_GPIO_WritePin(LED_PORT, LED2_PIN, GPIO_PIN_RESET);
        }
    if (data[3] == 3) {


        HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_SET);
    }
    else
            {
            	HAL_GPIO_WritePin(LED_PORT, LED3_PIN, GPIO_PIN_RESET);
            }
}
