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
uint8_t data[32];
#define SLAVE_ADDR 0x68
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
	I2C1Handle.I2C_CONFIG.I2C_DeviceAddress = 0x61;
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

	I2C_PeripheralControl(I2C1, ENABLE);

	I2C1Handle.pI2Cx->CR1 |= (1 << I2C_CR1_ACK);

	while(1)
	{
		while( !(GPIO_ReadPin(GPIOC, GPIO_PIN_N10) == PRESSED));
		delay();

		command_code = 0x51;
		while (I2C_MasterSendData_Interrupt(&I2C1Handle, &command_code, 1, SLAVE_ADDR, I2C_SR) != I2C_READY);
		while (I2C_MasterReceiveData_Interrupt(&I2C1Handle, &len, 1, SLAVE_ADDR, I2C_SR) != I2C_READY);

		command_code = 0x52;
		while (I2C_MasterSendData_Interrupt(&I2C1Handle, &command_code, 1, SLAVE_ADDR, I2C_SR) != I2C_READY);
		while (I2C_MasterReceiveData_Interrupt(&I2C1Handle, data, len, SLAVE_ADDR, I2C_NO_SR) != I2C_READY);

	}

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
	if (AppEvent == I2C_EV_TX_CMPLT)
	{
		printf("Tx is completed\n");
	}else if (AppEvent == I2C_EV_RX_CMPLT)
	{
		printf("Rx is completed\n");
	}else if (AppEvent == I2C_ERROR_AF)
	{
		printf("Error: ACK failure");
		I2C_CloseSendData(pI2CHandle);
		I2C_GenerateStopCondition(I2C1);
		while(1);
	}


}


