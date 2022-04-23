#ifndef INC_STM32F446XX_GPIO_DRIVERS_H_
#define INC_STM32F446XX_GPIO_DRIVERS_H_

#include"STM32F446xx.h"
#include<stdint.h>

#define GPIO_MODE_IN 0			// GPIO pin mode as input
#define GPIO_MODE_OUT 1			// GPIO pin mode as output
#define GPIO_MODE_ALT 2			// GPIO pin mode as alternate
#define GPIO_MODE_ANALOG 3		// GPIO pin mode as analog
#define GPIO_MODE_INT_FE 4		// GPIO interrupt falling edge trigger
#define GPIO_MODE_INT_RE 5		// GPIO interrupt rising edge trigger
#define GPIO_MODE_INT_RF 6		// GPIO interrupt rising/falling edge trigger

#define GPIO_OUT_TYPE_PP 0		// GPIO output Push-Pull type
#define GPIO_OUT_TYPE_OD 1		// GPIO output Open-Drain type

#define GPIO_SPEED_LOW 0		// GPIO low-speed output
#define GPIO_SPEED_MEDIUM 1		// GPIO medium-speed output
#define GPIO_SPEED_FAST 2		// GPIO fast-speed output
#define GPIO_SPEED_HIGH 3		// GPIO high-speed output

#define GPIO_NO_PUPD 0			// GPIO no Pull-up/Pull-down configuration pin
#define GPIO_PU 1				// GPIO Pull-Up configuration pin
#define GPIO_PD 2				// GPIO Pull-Down configuration pin

#define GPIO_PIN_N0 0			// GPIO pin 0
#define GPIO_PIN_N1 1			// GPIO pin 1
#define GPIO_PIN_N2 2			// GPIO pin 2
#define GPIO_PIN_N3 3			// GPIO pin 3
#define GPIO_PIN_N4 4			// GPIO pin 4
#define GPIO_PIN_N5 5			// GPIO pin 5
#define GPIO_PIN_N6 6			// GPIO pin 6
#define GPIO_PIN_N7 7			// GPIO pin 7
#define GPIO_PIN_N8 8			// GPIO pin 8
#define GPIO_PIN_N9 9			// GPIO pin 9
#define GPIO_PIN_N10 10			// GPIO pin 10
#define GPIO_PIN_N11 11			// GPIO pin 11
#define GPIO_PIN_N12 12			// GPIO pin 12
#define GPIO_PIN_N13 13			// GPIO pin 13
#define GPIO_PIN_N14 14			// GPIO pin 14
#define GPIO_PIN_N15 15			// GPIO pin 15

// Struct to configure a GPIO port
typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t	GPIO_PinPuPdControl;
	uint8_t GPIO_PinOPType;
	uint8_t GPIO_PinAltFunMode;
}GPIO_PIN_CFG;

typedef struct
{
	GPIO_REGISTERS *pGPIO;
	GPIO_PIN_CFG GPIO_PIN_CONFIG;
}GPIO_Handle;

void GPIO_Init(GPIO_Handle *pGPIOHandle);
void GPIO_DeInit(GPIO_REGISTERS *pGPIOx);
void GPIO_ClkCtrl(GPIO_REGISTERS *pGPIOx, uint8_t ENorDIS);
uint8_t GPIO_ReadPin(GPIO_REGISTERS *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadPort(GPIO_REGISTERS *pGPIOx);
void GPIO_WritePin(GPIO_REGISTERS *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WritePort(GPIO_REGISTERS *pGPIOx, uint16_t Value);
void GPIO_TogglePin(GPIO_REGISTERS *pGPIOx, uint8_t PinNumber);
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t ENorDIS);
void GPIO_IRQHandler(uint8_t PinNumber);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

#endif /* INC_STM32F446XX_GPIO_DRIVERS_H_ */
