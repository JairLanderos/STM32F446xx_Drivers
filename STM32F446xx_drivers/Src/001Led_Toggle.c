#include"STM32F446xx.h"

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i++);
}


int main(void)
{
	GPIO_Handle GPIOLed;
	GPIOLed.pGPIO = GPIOB;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N8;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinOPType = GPIO_OUT_TYPE_PP;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_PU;

	GPIO_ClkCtrl(GPIOB, ENABLE);
	GPIO_Init(&GPIOLed);
	while(1)
	{
		GPIO_TogglePin(GPIOB, GPIO_PIN_N8);
		delay();
	}
}
