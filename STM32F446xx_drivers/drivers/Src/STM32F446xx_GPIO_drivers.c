#include"STM32F446xx_GPIO_drivers.h"
#include<stdint.h>

void GPIO_Init(GPIO_Handle *pGPIOHandle)
{
	GPIO_ClkCtrl(pGPIOHandle->pGPIO, ENABLE);

	// 1. Mode configuration
	uint32_t temp = 0;
	if(pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		temp = (pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinMode << (2*pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber));
		pGPIOHandle->pGPIO->MODER &= ~(0x3 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
		pGPIOHandle->pGPIO->MODER |= temp;
		temp = 0;
	}else
	{
		if(pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinMode == GPIO_MODE_INT_FE)
		{
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinMode == GPIO_MODE_INT_RE)
		{
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
		}else if(pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinMode == GPIO_MODE_INT_RF)
		{
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
		}

		uint32_t temp1 = pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber / 4;
		uint32_t temp2 = (pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber % 4)*4;
		uint32_t PortCode = GPIO_BASE_ADDRESS_TO_CODE(pGPIOHandle->pGPIO);
		SYSCFG_CLOCK_ENABLE();
		SYSCFG->EXTICR[temp1] |= (PortCode << temp2);


		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);

	}

	// 2. Speed configuration
	temp = 0;
	temp = (pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinSpeed << (2*pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber));
	pGPIOHandle->pGPIO->OSPEEDER &= ~(0x3 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
	pGPIOHandle->pGPIO->OSPEEDER |= temp;

	// 3. Pull-Up/Pull-Down configuration
	temp = 0;
	temp = (pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinPuPdControl << (2*pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber));
	pGPIOHandle->pGPIO->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
	pGPIOHandle->pGPIO->PUPDR |= temp;

	// 4. OPType configuration
	temp = 0;
	temp = (pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinOPType << (pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber));
	pGPIOHandle->pGPIO->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber);
	pGPIOHandle->pGPIO->OTYPER |= temp;

	// 5. Alternate function configuration
	temp = 0;
	if(pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinMode == GPIO_MODE_ALT)
	{
		uint8_t temp1, temp2;
		temp1 = (pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber / 8);
		temp2 = (pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinNumber % 8);
		pGPIOHandle->pGPIO->AFR[temp1] &= ~(0xF << (4*temp2));
		pGPIOHandle->pGPIO->AFR[temp1] |= (pGPIOHandle->GPIO_PIN_CONFIG.GPIO_PinAltFunMode << (4*temp2));
	}
}

void GPIO_DeInit(GPIO_REGISTERS *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_RESET();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOB_RESET();
	}
	else if(pGPIOx == GPIOC)
	{
		GPIOC_RESET();
	}
	else if(pGPIOx == GPIOD)
	{
		GPIOD_RESET();
	}
	else if(pGPIOx == GPIOE)
	{
		GPIOE_RESET();
	}
	else if(pGPIOx == GPIOF)
	{
		GPIOF_RESET();
	}
	else if(pGPIOx == GPIOG)
	{
		GPIOG_RESET();
	}
	else if(pGPIOx == GPIOH)
	{
		GPIOH_RESET();
	}
}

void GPIO_ClkCtrl(GPIO_REGISTERS *pGPIOx, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_CLOCK_ENABLE();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_CLOCK_ENABLE();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_CLOCK_ENABLE();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_CLOCK_ENABLE();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_CLOCK_ENABLE();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_CLOCK_ENABLE();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_CLOCK_ENABLE();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_CLOCK_ENABLE();
		}
	}
	else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_CLOCK_DISABLE();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_CLOCK_DISABLE();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_CLOCK_DISABLE();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_CLOCK_DISABLE();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_CLOCK_DISABLE();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_CLOCK_DISABLE();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_CLOCK_DISABLE();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_CLOCK_DISABLE();
		}
	}
}



uint8_t GPIO_ReadPin(GPIO_REGISTERS *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	return value;
}

uint16_t GPIO_ReadPort(GPIO_REGISTERS *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)pGPIOx->IDR;
	return value;
}

void GPIO_WritePin(GPIO_REGISTERS *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1 << PinNumber);
	}else
	{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

void GPIO_WritePort(GPIO_REGISTERS *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

void GPIO_TogglePin(GPIO_REGISTERS *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);
}

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			// Program ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);
		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			// Program ISER1 register
			*NVIC_ISER1 |= (1 << IRQNumber % 32);
		}else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			// Program ISER2 register
			*NVIC_ISER2 |= (1 << IRQNumber % 64);
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			// Program ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);
		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			// Program ICER1 register
			*NVIC_ICER1 |= (1 << IRQNumber % 32);
		}else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			// Program ICER2 register
			*NVIC_ICER2 |= (1 << IRQNumber % 64);
		}
	}
}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = (IRQNumber%4)*8;
	*(NVIC_IPR_BASE_ADDRESS + iprx) |= (IRQPriority << (iprx_section - 4));
}

void GPIO_IRQHandler(uint8_t PinNumber)
{
	if(EXTI->PR & (1 << PinNumber))
	{
		EXTI->PR |= (1 << PinNumber);
	}
}


