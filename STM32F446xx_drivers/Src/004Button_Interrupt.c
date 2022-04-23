#include"STM32F446xx.h"
#include<string.h>

#define PRESSED 0			// Button is active high when released

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i++);
}


int main(void)
{
	GPIO_Handle GPIOLed, GPIOButton;
	memset(&GPIOLed, 0, sizeof(GPIOLed));
	memset(&GPIOLed, 0, sizeof(GPIOButton));

	GPIOLed.pGPIO = GPIOC;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N10;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinOPType = GPIO_OUT_TYPE_PP;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_ClkCtrl(GPIOC, ENABLE);
	GPIO_Init(&GPIOLed);

	// Button is used to trigger interrupt
	GPIOButton.pGPIO = GPIOC;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N11;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_INT_FE;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_PU;

	GPIO_ClkCtrl(GPIOC, ENABLE);
	GPIO_Init(&GPIOButton);

	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, 15);
	GPIO_IRQConfig(IRQ_NO_EXTI15_10, ENABLE);

	while(1);

}

void EXTI15_10_IRQHandler(void)
{
	delay();
	GPIO_IRQHandler(GPIO_PIN_N11);
	GPIO_TogglePin(GPIOC, GPIO_PIN_N10);
}

