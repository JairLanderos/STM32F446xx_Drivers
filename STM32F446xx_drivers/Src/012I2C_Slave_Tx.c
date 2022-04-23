#include<stdio.h>
#include<string.h>
#include<STM32F446xx.h>

/*********************************************************************************/
/*                       Usage of MCU pins for I2C1:                             */
/*                        														 */
/*								PB6 -> SCL  									 */
/*								PB9 -> SDA      								 */
/*********************************************************************************/
/*						Alternate function mode -> AF4							 */
/*********************************************************************************/

I2C_Handle I2C1Handle;
uint8_t data[32] = "STM32 Slave mode testing";
#define SLAVE_ADDR 0x69
#define PRESSED 0			// Button is active high when released

void delay(void)
{
	for(uint32_t i = 0; i < 500000/2; i++);
}

void I2C1_GPIOInit(void)
{
	GPIO_Handle I2CPins;

	I2CPins.pGPIO = GPIOB;
	I2CPins.GPIO_PIN_CONFIG.GPIO_PinMode = GPIO_MODE_ALT;
	I2CPins.GPIO_PIN_CONFIG.GPIO_PinOPType = GPIO_OUT_TYPE_OD;
	I2CPins.GPIO_PIN_CONFIG.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	I2CPins.GPIO_PIN_CONFIG.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PIN_CONFIG.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPins.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N6;
	GPIO_Init(&I2CPins);

	//SDA
	I2CPins.GPIO_PIN_CONFIG.GPIO_PinNumber = GPIO_PIN_N9;
	GPIO_Init(&I2CPins);
}

void I2C1_Init(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_CONFIG.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_CONFIG.I2C_DeviceAddress = SLAVE_ADDR;
	I2C1Handle.I2C_CONFIG.I2C_FMDutyCycle = I2C_DUTY_2;
	I2C1Handle.I2C_CONFIG.I2C_SCLSpeed = I2C_SPEED_SM;

	I2C_Init(&I2C1Handle);
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

int main()
{
	uint8_t command_code;
	uint8_t len;

	GPIOButton_Init();

	I2C1_GPIOInit();

	I2C1_Init();

	I2C_IRQConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQConfig(IRQ_NO_I2C1_ER, ENABLE);
	I2C_SlaveEnableCallbackEvents(I2C1, ENABLE);

	I2C_PeripheralControl(I2C1, ENABLE);

	I2C1Handle.pI2Cx->CR1 |= (1 << I2C_CR1_ACK);

	while(1);

}


void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}


void I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEventCallback(I2C_Handle *pI2CHandle, uint8_t AppEvent)
{
	static uint8_t command_code = 0;
	static uint8_t Count = 0;

	if (AppEvent == I2C_EV_DATA_REQ)
	{
		if (command_code == 0x51)
		{
			I2C_SlaveSendData(pI2CHandle->pI2Cx, strlen((char*)data));
		}else if (command_code == 0x52)
		{
			I2C_SlaveSendData(pI2CHandle->pI2Cx, data[Count++]);
		}
	}else if (AppEvent == I2C_EV_DATA_RCV)
	{
		command_code = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);
	}else if (AppEvent == I2C_ERROR_AF)
	{
		command_code = 0xff;
		Count = 0;
	}else if (AppEvent == I2C_EV_STOP)
	{

	}
}


