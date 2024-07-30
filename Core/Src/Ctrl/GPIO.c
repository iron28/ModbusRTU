#include <Ctrl/GPIO.h>

uint8_t getID(void) {
	uint8_t u8ID = 0;
	u8ID += (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)) << 0;
	u8ID += (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)) << 1;
	u8ID += (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)) << 2;
	u8ID += (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)) << 3;

	return u8ID;
}

void togRDY(void) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
}

void togRX(void) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
}

void togTX(void) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
}
