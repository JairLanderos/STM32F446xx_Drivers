#ifndef INC_STM32F446XX_H_
#define INC_STM32F446XX_H_

#include<stdint.h>
#include<stddef.h>

#define __vo volatile
#define __weak __attribute__((weak))

/***************************************************************************************************************/
/*                                         Processor specific details                                          */
/***************************************************************************************************************/

// ISERx ARM Cortex M4 registers (set interrupt)
#define NVIC_ISER0 ((__vo uint32_t*)0xE000E100)
#define NVIC_ISER1 ((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2 ((__vo uint32_t*)0xE000E108)
#define NVIC_ISER3 ((__vo uint32_t*)0xE000E10C)

// ICERx ARM Cortex M4 registers (clear interrupt)
#define NVIC_ICER0 ((__vo uint32_t*)0xE000E180)
#define NVIC_ICER1 ((__vo uint32_t*)0xE000E184)
#define NVIC_ICER2 ((__vo uint32_t*)0xE000E188)
#define NVIC_ICER3 ((__vo uint32_t*)0xE000E18C)

// IPR ARM Cortex M4 base address (set priority)
#define NVIC_IPR_BASE_ADDRESS ((__vo uint32_t*)0xE000E400)


/***************************************************************************************************************/
/*                                             MCU specific details                                            */
/***************************************************************************************************************/

// Base addresses of Flash and SRAM memories
#define FLASH_BASE_ADDRESS 0x08000000U
#define SRAM1_BASE_ADDRESS 0x20000000U
#define SRAM2_BASE_ADDRESS 0x2001C000U
#define ROM_BASE_ADDRESS 0x1FFF0000U

// APBx and AHBx Bus Peripheral base addresses
#define AHB2_BASE_ADDRESS 0x50000000U
#define AHB1_BASE_ADDRESS 0x40020000U
#define APB2_BASE_ADDRESS 0x40010000U
#define APB1_BASE_ADDRESS 0x40000000U

// Base addresses of AHB1 Bus peripherals
#define GPIOA_BASE_ADDRESS 0x40020000U
#define GPIOB_BASE_ADDRESS 0x40020400U
#define GPIOC_BASE_ADDRESS 0x40020800U
#define GPIOD_BASE_ADDRESS 0x40020C00U
#define GPIOE_BASE_ADDRESS 0x40021000U
#define GPIOF_BASE_ADDRESS 0x40021400U
#define GPIOG_BASE_ADDRESS 0x40021800U
#define GPIOH_BASE_ADDRESS 0x40021C00U
#define RCC_BASE_ADDRESS 0x40023800U

// Base addresses of APB1 Bus peripherals
#define I2C1_BASE_ADDRESS 0x40005400U
#define I2C2_BASE_ADDRESS 0x40005800U
#define I2C3_BASE_ADDRESS 0x40005C00U
#define SPI2_BASE_ADDRESS 0x40003800U
#define SPI3_BASE_ADDRESS 0x40003C00U
#define USART2_BASE_ADDRESS 0x40004400U
#define USART3_BASE_ADDRESS 0x40004800U
#define UART4_BASE_ADDRESS 0x40004C00U
#define UART5_BASE_ADDRESS 0x40005000U

// Base addresses of APB2 Bus peripherals
#define SPI1_BASE_ADDRESS 0x40013000U
#define USART1_BASE_ADDRESS 0x40011000U
#define USART6_BASE_ADDRESS 0x40011400U
#define EXTI_BASE_ADDRESS 0x40013C00U
#define SYSCFG_BASE_ADDRESS 0x40013800U

// Reset GPIOx register
#define GPIOA_RESET() do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_RESET() do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_RESET() do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOD_RESET() do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOE_RESET() do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOF_RESET() do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOG_RESET() do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOH_RESET() do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)


/******************Peripheral register definition structure*************************/
typedef struct
{
	__vo uint32_t MODER;
	__vo uint32_t OTYPER;
	__vo uint32_t OSPEEDER;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t LCKR;
	__vo uint32_t AFR[2];
}GPIO_REGISTERS;
#define GPIOA ((GPIO_REGISTERS*)GPIOA_BASE_ADDRESS)
#define GPIOB ((GPIO_REGISTERS*)GPIOB_BASE_ADDRESS)
#define GPIOC ((GPIO_REGISTERS*)GPIOC_BASE_ADDRESS)
#define GPIOD ((GPIO_REGISTERS*)GPIOD_BASE_ADDRESS)
#define GPIOE ((GPIO_REGISTERS*)GPIOE_BASE_ADDRESS)
#define GPIOF ((GPIO_REGISTERS*)GPIOF_BASE_ADDRESS)
#define GPIOG ((GPIO_REGISTERS*)GPIOG_BASE_ADDRESS)
#define GPIOH ((GPIO_REGISTERS*)GPIOH_BASE_ADDRESS)

typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;
	__vo uint32_t RXCRCR;
	__vo uint32_t TXCRCR;
	__vo uint32_t I2SCFGR;
	__vo uint32_t I2SPR;
}SPI_REGISTERS;
#define SPI1 ((SPI_REGISTERS*)SPI1_BASE_ADDRESS)
#define SPI2 ((SPI_REGISTERS*)SPI2_BASE_ADDRESS)
#define SPI3 ((SPI_REGISTERS*)SPI3_BASE_ADDRESS)

typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t OAR1;
	__vo uint32_t OAR2;
	__vo uint32_t DR;
	__vo uint32_t SR1;
	__vo uint32_t SR2;
	__vo uint32_t CCR;
	__vo uint32_t TRISE;
	__vo uint32_t FLTR;
}I2C_REGISTERS;
#define I2C1 ((I2C_REGISTERS*)I2C1_BASE_ADDRESS)
#define I2C2 ((I2C_REGISTERS*)I2C2_BASE_ADDRESS)
#define I2C3 ((I2C_REGISTERS*)I2C3_BASE_ADDRESS)

typedef struct
{
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t BRR;
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t CR3;
	__vo uint32_t GTPR;
}USART_REGISTERS;
#define USART1 ((USART_REGISTERS*)USART1_BASE_ADDRESS)
#define USART2 ((USART_REGISTERS*)USART2_BASE_ADDRESS)
#define USART3 ((USART_REGISTERS*)USART3_BASE_ADDRESS)
#define UART4 ((USART_REGISTERS*)UART4_BASE_ADDRESS)
#define UART5 ((USART_REGISTERS*)UART5_BASE_ADDRESS)
#define USART6 ((USART_REGISTERS*)USART6_BASE_ADDRESS)

typedef struct
{
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;
	uint32_t RESERVED0;
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
	uint32_t RESERVED1[2];
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;
	uint32_t RESERVED2;
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
	uint32_t RESERVED3[2];
	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;
	uint32_t RESERVED4;
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
	uint32_t RESERVED5[2];
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	uint32_t RESERVED6[2];
	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;
	__vo uint32_t PLLSAICFGR;
	__vo uint32_t DCKCFGR;
	__vo uint32_t CKGATENR;
	__vo uint32_t DCKCFGR2;
}RCC_REGISTERS;
#define RCC ((RCC_REGISTERS*)RCC_BASE_ADDRESS)

typedef struct
{
	__vo uint32_t IMR;
	__vo uint32_t EMR;
	__vo uint32_t RTSR;
	__vo uint32_t FTSR;
	__vo uint32_t SWIER;
	__vo uint32_t PR;
}EXTI_REGISTERS;
#define EXTI ((EXTI_REGISTERS*)EXTI_BASE_ADDRESS)

typedef struct
{
	__vo uint32_t MEMRMP;
	__vo uint32_t PMC;
	__vo uint32_t EXTICR[4];
	uint32_t RESERVED1[2];
	__vo uint32_t CMPCR;
	uint32_t RESERVED2[2];
	__vo uint32_t CFGR;
}SYSCFG_REGISTERS;
#define SYSCFG ((SYSCFG_REGISTERS*)SYSCFG_BASE_ADDRESS)

// Clock Enable Macros for GPIOx peripherals
#define GPIOA_CLOCK_ENABLE() (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_CLOCK_ENABLE() (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_CLOCK_ENABLE() (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_CLOCK_ENABLE() (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_CLOCK_ENABLE() (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_CLOCK_ENABLE() (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_CLOCK_ENABLE() (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_CLOCK_ENABLE() (RCC->AHB1ENR |= (1 << 7))

// Clock Enable Macros for I2Cx peripherals
#define I2C1_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 21))
#define I2C2_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 22))
#define I2C3_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 23))

// Clock Enable Macros for SPIx peripherals
#define SPI1_CLOCK_ENABLE() (RCC->APB2ENR |= (1 << 12))
#define SPI2_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 14))
#define SPI3_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 15))

// Clock Enable Macros for USARTx peripherals
#define USART1_CLOCK_ENABLE() (RCC->APB2ENR |= (1 << 4))
#define USART2_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 17))
#define USART3_CLOCK_ENABLE() (RCC->APB1ENR |= (1 << 18))

// Clock Disable Macros for GPIOx peripherals
#define GPIOA_CLOCK_DISABLE() (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_CLOCK_DISABLE() (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_CLOCK_DISABLE() (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_CLOCK_DISABLE() (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_CLOCK_DISABLE() (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_CLOCK_DISABLE() (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_CLOCK_DISABLE() (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_CLOCK_DISABLE() (RCC->AHB1ENR &= ~(1 << 7))

// Clock Disable Macros for I2Cx peripherals
#define I2C1_CLOCK_DISABLE() (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_CLOCK_DISABLE() (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_CLOCK_DISABLE() (RCC->APB1ENR &= ~(1 << 23))

// Clock Disable Macros for SPIx peripherals
#define SPI1_CLOCK_DISABLE() (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_CLOCK_DISABLE() (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_CLOCK_DISABLE() (RCC->APB1ENR &= ~(1 << 15))

// Clock Disable Macros for USARTx peripherals
#define USART1_CLOCK_DISABLE() (RCC->APB2ENR &= ~(1 << 4))
#define USART2_CLOCK_DISABLE() (RCC->APB1ENR &= ~(1 << 17))
#define USART3_CLOCK_DISABLE() (RCC->APB1ENR &= ~(1 << 18))

// Clock Enable Macro for SYSCFG
#define SYSCFG_CLOCK_ENABLE() (RCC->APB2ENR |= (1 << 14))

// Clock Disable Macro for SYSCFG
#define SYSCFG_CLOCK_DISABLE() (RCC->APB2ENR &= ~(1 << 14))

// Macro to return the port number
#define GPIO_BASE_ADDRESS_TO_CODE(x) ( (x == GPIOA)?0:\
									 (x == GPIOB)?1:\
									 (x == GPIOC)?2:\
									 (x == GPIOD)?3:\
									 (x == GPIOE)?4:\
									 (x == GPIOF)?5:\
									 (x == GPIOG)?6:\
									 (x == GPIOH)?7:0 )

// IRQ numbers related to MCU EXTI peripheral
#define IRQ_NO_EXTI0 6
#define IRQ_NO_EXTI1 7
#define IRQ_NO_EXTI2 8
#define IRQ_NO_EXTI3 9
#define IRQ_NO_EXTI4 10
#define IRQ_NO_EXTI9_5 23
#define IRQ_NO_EXTI15_10 40

// IRQ numbers related to SPI
#define IRQ_NO_SPI1 35
#define IRQ_NO_SPI2 36
#define IRQ_NO_SPI3 51

// IRQ numbers related to I2C
#define IRQ_NO_I2C1_EV 31
#define IRQ_NO_I2C1_ER 32

/*****************************************************************/
/*		           	Bit positions for SPI					 	 */
/*****************************************************************/

// Bit positions for SPI CR1 register
#define SPI_CR1_CPHA 0
#define SPI_CR1_CPOL 1
#define SPI_CR1_MSTR 2
#define SPI_CR1_BR 3
#define SPI_CR1_SPE 6
#define SPI_CR1_LBSFIRST 7
#define SPI_CR1_SSI 8
#define SPI_CR1_SSM 9
#define SPI_CR1_RXONLY 10
#define SPI_CR1_DFF 11
#define SPI_CR1_CRCNEXT 12
#define SPI_CR1_CRCEN 13
#define SPI_CR1_BIDIOE 14
#define SPI_CR1_BIDIMODE 15

// Bit positions for SPI CR2 register
#define SPI_CR2_RXDMAEN 0
#define SPI_CR2_TXDMAEN 1
#define SPI_CR2_SSOE 2
#define SPI_CR2_FRF 4
#define SPI_CR2_ERRIE 5
#define SPI_CR2_RXNEIE 6
#define SPI_CR2_TXEIE 7

// Bit positions for SPI SR register
#define SPI_SR_RXNE 0
#define SPI_SR_TXE 1
#define SPI_SR_CHSIDE 2
#define SPI_SR_UDR 3
#define SPI_SR_CRCERR 4
#define SPI_SR_MODF 5
#define SPI_SR_OVR 6
#define SPI_SR_BSY 7
#define SPI_SR_FRE 8

/*****************************************************************/
/*		           	Bit positions for I2C					 	 */
/*****************************************************************/

// Bit positions for I2C CR1 register
#define I2C_CR1_PE 0
#define I2C_CR1_NOSTRETCH 7
#define I2C_CR1_START 8
#define I2C_CR1_STOP 9
#define I2C_CR1_ACK 10
#define I2C_CR1_SWRST 15

// Bit positions for I2C CR2 register
#define I2C_CR2_FREQ 0
#define I2C_CR2_ITERREN 8
#define I2C_CR2_ITEVTEN 9
#define I2C_CR2_ITBUFEN 10

// Bit positions for I2C SR1 register
#define I2C_SR1_SB 0
#define I2C_SR1_ADDR 1
#define I2C_SR1_BTF 2
#define I2C_SR1_ADD10 3
#define I2C_SR1_STOPF 4
#define I2C_SR1_RXNE 6
#define I2C_SR1_TXE 7
#define I2C_SR1_BERR 8
#define I2C_SR1_ARLO 9
#define I2C_SR1_AF 10
#define I2C_SR1_OVR 11
#define I2C_SR1_TIMEOUT 14

// Bit positions for I2C SR2 register
#define I2C_SR2_MSL 0
#define I2C_SR2_BUSY 1
#define I2C_SR2_TRA 2
#define I2C_SR2_GENCALL 4
#define I2C_SR2_DUALF 7

// Bit positions for I2C CCR register
#define I2C_CCR_CCR 0
#define I2C_CCR_DUTY 14
#define I2C_CCR_FS 15

/*****************************************************************/
/*		           	Bit positions for USART					 	 */
/*****************************************************************/

// Bit positions for SR register
#define USART_SR_PE 0
#define USART_SR_FE 1
#define USART_SR_NF 2
#define USART_SR_ORE 3
#define USART_SR_IDLE 4
#define USART_SR_RXNE 5
#define USART_SR_TC 6
#define USART_SR_TXE 7
#define USART_SR_LBD 8
#define USART_SR_CTS 9

// Bit positions for CR1 register
#define USART_CR1_SBK 0
#define USART_CR1_RWU 1
#define USART_CR1_RE 2
#define USART_CR1_TE 3
#define USART_CR1_IDLEIE 4
#define USART_CR1_RXNEIE 5
#define USART_CR1_TCIE 6
#define USART_CR1_TXEIE 7
#define USART_CR1_PEIE 8
#define USART_CR1_PS 9
#define USART_CR1_PCE 10
#define USART_CR1_WAKE 11
#define USART_CR1_M 12
#define USART_CR1_UE 13
#define USART_CR1_OVER8 15

// Bit positions for CR2 register
#define USART_CR2_ADD 0
#define USART_CR2_LBDL 5
#define USART_CR2_LBDIE 6
#define USART_CR2_LBCL 8
#define USART_CR2_CPHA 9
#define USART_CR2_CPOL 10
#define USART_CR2_CLKEN 11
#define USART_CR2_STOP 12
#define USART_CR2_LINEN 14

// Bit positions for CR3 register
#define USART_CR3_EIE 0
#define USART_CR3_IREN 1
#define USART_CR3_IRLP 2
#define USART_CR3_HDSEL 3
#define USART_CR3_NACK 4
#define USART_CR3_SCEN 5
#define USART_CR3_DMAR 6
#define USART_CR3_DMAT 7
#define USART_CR3_RTSE 8
#define USART_CR3_CTSE 9
#define USART_CR3_CTSIE 10
#define USART_CR3_ONEBIT 11

/*****************************************************************/
/*		           		   Generic macros					 	 */
/*****************************************************************/

#define ENABLE 1
#define DISABLE 0
#define SET ENABLE
#define RESET DISABLE
#define GPIO_PIN_SET SET
#define GPIO_PIN_RESET RESET
#define FLAG_RESET RESET
#define FLAG_SET SET

#include"STM32F446xx_GPIO_drivers.h"
#include"STM32F446xx_SPI_drivers.h"
#include"STM32F446xx_I2C_drivers.h"
#include"STM32F446xx_USART_drivers.h"
#include"STM32F446xx_RCC_drivers.h"

#endif /* INC_STM32F446XX_H_ */
