#include"STM32F446xx.h"

#define PRESSED 0			// Button is active high when released

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i++);
}


int main(void)
{
	GPIO_Handle GPIOLed, GPIOButton;


	GPIOLed.pGPIO = GPIOC;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N10;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinOPType = GPIO_OUT_TYPE_PP;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_ClkCtrl(GPIOC, ENABLE);
	GPIO_Init(&GPIOLed);

	GPIOButton.pGPIO = GPIOC;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N11;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_IN;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_PU;

	GPIO_ClkCtrl(GPIOC, ENABLE);
	GPIO_Init(&GPIOButton);

	while(1)
	{
		if(GPIO_ReadPin(GPIOC, GPIO_PIN_N11) == PRESSED)
		{
			delay();
			GPIO_TogglePin(GPIOC, GPIO_PIN_N10);
		}

	}
}
