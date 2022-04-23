#include<stdio.h>
#include<string.h>
#include "STM32F446xx.h"

char msg[1024] = "Oye, TE AMOO <3";
#define PRESSED 0			// Button is active high when released

USART_Handle usart2_handle;

void USART2_Init(void)
{
	usart2_handle.pUSARTx = USART1;
	usart2_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart2_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOWCTRL_NONE;
	usart2_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	usart2_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart2_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART_Init(&usart2_handle);
}

void 	USART2_GPIOInit(void)
{
	GPIO_Handle usart_gpios;

	usart_gpios.pGPIO = GPIOA;
	usart_gpios.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_ALT;
	usart_gpios.GPIO_PIN_CONFIG.GPIO_PinOPType = GPIO_OUT_TYPE_PP;
	usart_gpios.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	usart_gpios.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;
	usart_gpios.GPIO_PIN_CONFIG.GPIO_PinAltFunMode =7;

	//USART2 TX
	usart_gpios.GPIO_PIN_CONFIG.GPIO_PinNumber  = GPIO_PIN_N9;
	GPIO_Init(&usart_gpios);

	//USART2 RX
	usart_gpios.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N10;
	GPIO_Init(&usart_gpios);


}

void GPIOButton_Init()
{

	GPIO_Handle GPIOButton;

	GPIOButton.pGPIO = GPIOC;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N10;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_IN;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_PU;

	GPIO_Init(&GPIOButton);
}

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}


int main(void)
{

	GPIOButton_Init();

	USART2_GPIOInit();

    USART2_Init();

    USART_PeripheralControl(USART1,ENABLE);

    while(1)
    {
		//wait till button is pressed
    	while( !(GPIO_ReadPin(GPIOC, GPIO_PIN_N10) == PRESSED))
    	{
    		;
    	}

		//to avoid button de-bouncing related issues 200ms of delay
		delay();


		USART_SendData(&usart2_handle,(uint8_t*)msg,strlen(msg));

    }

	return 0;
}
