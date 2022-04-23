#include"STM32F446xx.h"
#include<string.h>
#include<stdio.h>

/*********************************************************************************/
/*                       Usage of MCU pins for SPI2:                             */
/*                        														 */
/*								PB15 -> MOSI									 */
/*								PB14 -> MISO									 */
/*								PB13 -> SCLK									 */
/*								PB12 -> NSS										 */
/*********************************************************************************/
/*						Alternate function mode -> AF5							 */
/*********************************************************************************/

#define PRESSED 0			// Button is active high when released

void SPI2_GPIOInit(void)
{
	GPIO_Handle SPI_Pins;
	SPI_Pins.pGPIO = GPIOB;
	SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_ALT;
	SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinAltFunMode = 5;
	SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinOPType = GPIO_OUT_TYPE_PP;
	SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLK
	SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N13;
	GPIO_Init(&SPI_Pins);

	// MOSI
	SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N15;
	GPIO_Init(&SPI_Pins);

	// MISO
	//SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N14;
	//GPIO_Init(&SPI_Pins);

	// NSS
	SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N12;
	GPIO_Init(&SPI_Pins);
}

void SPI2_Init(void)
{
	SPI_Handle SPI2Handle;
	SPI2Handle.pSPI = SPI2;
	SPI2Handle.SPI_CONFIG.SPI_BusConfig = SPI_FULL_DUPLEX;
	SPI2Handle.SPI_CONFIG.SPI_DeviceMode = SPI_MASTER_MODE;
	SPI2Handle.SPI_CONFIG.SPI_SCLKSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2Handle.SPI_CONFIG.SPI_DFF = SPI_DATA_FRAME_8BIT;
	SPI2Handle.SPI_CONFIG.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPI_CONFIG.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPI_CONFIG.SPI_SSM = SPI_SSM_DISABLE;

	SPI_Init(&SPI2Handle);
}

void GPIOButton_Init()
{

	GPIO_Handle GPIOButton, GPIOLed;

	GPIOLed.pGPIO = GPIOC;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N11;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinOPType = GPIO_OUT_TYPE_PP;
	GPIOLed.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_ClkCtrl(GPIOC, ENABLE);
	GPIO_Init(&GPIOLed);

	GPIOButton.pGPIO = GPIOC;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N10;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_IN;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOButton.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_PU;

	GPIO_Init(&GPIOButton);
}

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000 ; i++);
}

int main()
{
	char user_data[] = "Hello World";

	GPIOButton_Init();

	SPI2_GPIOInit();

	SPI2_Init();

	SPI_SSOEConfig(SPI2, ENABLE);

	while(1){
		while( !(GPIO_ReadPin(GPIOC, GPIO_PIN_N10) == PRESSED));
		delay();

		SPI_PeripheralControl(SPI2, ENABLE);

		uint8_t dataLength = strlen(user_data);

		GPIO_TogglePin(GPIOC, GPIO_PIN_N11);
		delay();
		GPIO_TogglePin(GPIOC, GPIO_PIN_N11);

		SPI_SendData(SPI2, &dataLength, 1);

		SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

		while( SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));

		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;
}
