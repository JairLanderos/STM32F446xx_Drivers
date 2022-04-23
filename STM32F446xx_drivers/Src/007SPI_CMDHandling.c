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

// Command codes for Arduino
#define COMMAND_LED_CTRL 0x50
#define COMMAND_SENSOR_READ 0x51
#define COMMAND_LED_READ 0x52
#define COMMAND_PRINT 0x53
#define COMMAND_ID_READ 0x54

#define LED_ON 1
#define LED_OFF 0

// Arduino analog pins
#define ANALOG_PIN0 0
#define ANALOG_PIN1 1
#define ANALOG_PIN2 2
#define ANALOG_PIN3 3
#define ANALOG_PIN4 4

#define LED_PIN 9

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
	SPI_Pins.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N14;
	GPIO_Init(&SPI_Pins);

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
	for(uint32_t i = 0 ; i < 500000 ; i++);
}

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		return 1;
	}
	return 0;
}

int main()
{
	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;

	GPIOButton_Init();

	SPI2_GPIOInit();

	SPI2_Init();

	SPI_SSOEConfig(SPI2, ENABLE);

	while(1){

		/*********************************************************/
		/*			   CMD_LED_CTRL <PIN, ON>			     	 */
		/*********************************************************/

		while( !(GPIO_ReadPin(GPIOC, GPIO_PIN_N10) == PRESSED));
		delay();

		SPI_PeripheralControl(SPI2, ENABLE);

		uint8_t command_code = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];

		// Send the command to the slave
		SPI_SendData(SPI2, &command_code, 1);
		// Dummy read just to clear off the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);
		// Send random byte to slave to get the response to above command from MISO line (ACK or NACK)
		SPI_SendData(SPI2, &dummy_write, 1);
		// Read if the command is acknowledged by the slave
		SPI_ReceiveData(SPI2, &ackbyte, 1);
		// Verify the response
		if(SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;
			args[1] = LED_ON;
			SPI_SendData(SPI2, args, 2);
		}

		/*********************************************************/
		/*			   CMD_SENSOR_READ <ANALOG_PIN>			 	 */
		/*********************************************************/

		while( !(GPIO_ReadPin(GPIOC, GPIO_PIN_N10) == PRESSED));
		delay();

		command_code = COMMAND_SENSOR_READ;

		// Send the command to the slave
		SPI_SendData(SPI2, &command_code, 1);
		// Dummy read just to clear off the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);
		// Send random byte to slave to get the response to above command from MISO line (ACK or NACK)
		SPI_SendData(SPI2, &dummy_write, 1);
		// Read if the command is acknowledged by the slave
		SPI_ReceiveData(SPI2, &ackbyte, 1);
		// Verify the response
		if(SPI_VerifyResponse(ackbyte))
		{
			args[0] = ANALOG_PIN0;
			SPI_SendData(SPI2, args, 1);
			// Dummy read just to clear off the RXNE
			SPI_ReceiveData(SPI2, &dummy_read, 1);
			delay();
			// Send random byte to slave to get the response to above command from MISO line (ACK or NACK)
			SPI_SendData(SPI2, &dummy_write, 1);
			uint8_t analog_read;
			SPI_ReceiveData(SPI2, &analog_read, 1);
		}

		/*********************************************************/
		/*			         CMD_LED_READ <PIN>		   		 	 */
		/*********************************************************/

		while( !(GPIO_ReadPin(GPIOC, GPIO_PIN_N10) == PRESSED));
		delay();

		command_code = COMMAND_LED_READ;

		// Send the command to the slave
		SPI_SendData(SPI2, &command_code, 1);
		// Dummy read just to clear off the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);
		// Send random byte to slave to get the response to above command from MISO line (ACK or NACK)
		SPI_SendData(SPI2, &dummy_write, 1);
		// Read if the command is acknowledged by the slave
		SPI_ReceiveData(SPI2, &ackbyte, 1);
		// Verify the response
		if(SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;
			SPI_SendData(SPI2, args, 1);
			// Dummy read just to clear off the RXNE
			SPI_ReceiveData(SPI2, &dummy_read, 1);
			delay();
			// Send random byte to slave to get the response to above command from MISO line (ACK or NACK)
			SPI_SendData(SPI2, &dummy_write, 1);
			uint8_t pin_read;
			SPI_ReceiveData(SPI2, &pin_read, 1);
		}

		/*********************************************************/
		/*			   CMD_LED_CTRL <PIN, OFF>			     	 */
		/*********************************************************/

		while( !(GPIO_ReadPin(GPIOC, GPIO_PIN_N10) == PRESSED));
		delay();

		command_code = COMMAND_LED_CTRL;

		// Send the command to the slave
		SPI_SendData(SPI2, &command_code, 1);
		// Dummy read just to clear off the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);
		// Send random byte to slave to get the response to above command from MISO line (ACK or NACK)
		SPI_SendData(SPI2, &dummy_write, 1);
		// Read if the command is acknowledged by the slave
		SPI_ReceiveData(SPI2, &ackbyte, 1);
		// Verify the response
		if(SPI_VerifyResponse(ackbyte))
		{
			args[0] = LED_PIN;
			args[1] = LED_OFF;
			SPI_SendData(SPI2, args, 2);
		}


		while( SPI_GetFlagStatus(SPI2,SPI_BSY_FLAG));

		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;
}
