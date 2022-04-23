#include"STM32F446xx_SPI_drivers.h"
#include<stdint.h>

static void SPI_TXE_Interrupt_Handle(SPI_Handle *pSPIHandle);
static void SPI_RXNE_Interrupt_Handle(SPI_Handle *pSPIHandle);
static void SPI_OVR_Interrupt_Handle(SPI_Handle *pSPIHandle);


void SPI_Init(SPI_Handle *pSPIHandle)
{
	SPI_ClkCtrl(pSPIHandle->pSPI, ENABLE);

	// 1. Configure device mode (Master or Slave)
	uint32_t temp = 0;
	temp |= (pSPIHandle->SPI_CONFIG.SPI_DeviceMode << SPI_CR1_MSTR);

	// 2. Configure bus configuration (Full-Duplex, Half-Duplex or RX-Only)
	if(pSPIHandle->SPI_CONFIG.SPI_BusConfig == SPI_FULL_DUPLEX)
	{
		temp &= ~(1 << SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPI_CONFIG.SPI_BusConfig == SPI_HALF_DUPLEX)
	{
		temp |= (1 << SPI_CR1_BIDIMODE);
	}else if(pSPIHandle->SPI_CONFIG.SPI_BusConfig == SPI_RX_ONLY)
	{
		temp &= ~(1 << SPI_CR1_BIDIMODE);
		temp |= (1 << SPI_CR1_RXONLY);
	}

	// 3. Configure SCLK speed (Baud Rate)
	temp |= (pSPIHandle->SPI_CONFIG.SPI_SCLKSpeed << SPI_CR1_BR);

	// 4. Configure the DFF (8-bit or 16-bit data frame)
	temp |= (pSPIHandle->SPI_CONFIG.SPI_DFF << SPI_CR1_DFF);

	// 5. Configure the CPOL
	temp |= (pSPIHandle->SPI_CONFIG.SPI_CPOL << SPI_CR1_CPOL);

	// 6. Configure the CPHA
	temp |= (pSPIHandle->SPI_CONFIG.SPI_CPHA << SPI_CR1_CPHA);

	// 7. Configure the SSM
	temp |= (pSPIHandle->SPI_CONFIG.SPI_SSM << SPI_CR1_SSM);

	pSPIHandle->pSPI->CR1 = temp;
}

void SPI_DeInit(SPI_REGISTERS *pSPIx)
{

}

void SPI_ClkCtrl(SPI_REGISTERS *pSPIx, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_CLOCK_ENABLE();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_CLOCK_ENABLE();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_CLOCK_ENABLE();
		}
	}
	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_CLOCK_DISABLE();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_CLOCK_DISABLE();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_CLOCK_DISABLE();
		}
	}
}

uint8_t SPI_GetFlagStatus(SPI_REGISTERS *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void SPI_SendData(SPI_REGISTERS *pSPIx, uint8_t *pTxBuffer, uint32_t Length)
{
	while(Length > 0)
	{
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			// 16-bit
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Length -= 2;
			(uint16_t*)pTxBuffer++;
		}else
		{
			// 8-bit
			*((volatile uint8_t *)&pSPIx->DR) = *pTxBuffer;
			Length--;
			pTxBuffer++;
		}

	}
}

void SPI_ReceiveData(SPI_REGISTERS *pSPIx, uint8_t *pRxBuffer, uint32_t Length)
{
	while(Length > 0)
	{
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);
		if(pSPIx->CR1 & (1 << SPI_CR1_DFF))
		{
			// 16-bit
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			Length -= 2;
			(uint16_t*)pRxBuffer++;
		}else
		{
			// 8-bit
			*pRxBuffer = pSPIx->DR;
			Length--;
			pRxBuffer++;
		}

	}
}

void SPI_IRQConfig(uint8_t IRQNumber, uint8_t ENorDIS)
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

void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = (IRQNumber%4)*8;
	*(NVIC_IPR_BASE_ADDRESS + iprx) |= (IRQPriority << (iprx_section - 4));
}

void SPI_PeripheralControl(SPI_REGISTERS *pSPIx, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_REGISTERS *pSPIx, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

void SPI_SSOEConfig(SPI_REGISTERS *pSPIx, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPI_CR2_SSOE);
	}else
	{
		pSPIx->CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}

uint8_t SPI_SendData_Int(SPI_Handle *pSPIHandle, uint8_t *pTxBuffer, uint32_t Length)
{
	uint8_t state = pSPIHandle->TxState;
	if(state != SPI_BUSY_TX)
	{
		// 1. Save TxBuffer and Length in global variables in order to access them inside the IRQ Handler
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLength = Length;

		// 2. Mark the SPI state as busy
		pSPIHandle->TxState = SPI_BUSY_TX;

		// 3. Enable TXEIE from CR2 to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPI->CR2 |= (1 << SPI_CR2_TXEIE);
	}
	return state;
}

uint8_t SPI_ReceiveData_Int(SPI_Handle *pSPIHandle, uint8_t *pRxBuffer, uint32_t Length)
{
	uint8_t state = pSPIHandle->RxState;
	if(state != SPI_BUSY_RX)
	{
		// 1. Save RxBuffer and Length in global variables in order to access them inside the IRQ Handler
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLength = Length;

		// 2. Mark the SPI state as busy
		pSPIHandle->RxState = SPI_BUSY_RX;

		// 3. Enable RXNEIE from CR2 to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPI->CR2 |= (1 << SPI_CR2_RXNEIE);
	}
	return state;
}

void SPI_IRQHandler(SPI_Handle *pSPIHandle)
{
	uint8_t temp1, temp2;

	// Check for TXE
	temp1 = pSPIHandle->pSPI->SR & (1 << SPI_SR_TXE);
	temp2 = pSPIHandle->pSPI->CR2 & (1 << SPI_CR2_TXEIE);
	if(temp1 && temp2)
	{
		SPI_TXE_Interrupt_Handle(pSPIHandle);

	}

	// Check for RXNE
	temp1 = pSPIHandle->pSPI->SR & (1 << SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPI->CR2 & (1 << SPI_CR2_RXNEIE);
	if(temp1 && temp2)
	{
		SPI_RXNE_Interrupt_Handle(pSPIHandle);

	}

	// Check for OVR flag
	temp1 = pSPIHandle->pSPI->SR & (1 << SPI_SR_OVR);
	temp2 = pSPIHandle->pSPI->CR2 & (1 << SPI_CR2_ERRIE);
	if(temp1 && temp2)
	{
		SPI_OVR_Interrupt_Handle(pSPIHandle);
	}
}


/***********************************************************/
/*			       	  HELPER FUNCTIONS				       */
/***********************************************************/

static void SPI_TXE_Interrupt_Handle(SPI_Handle *pSPIHandle)
{
	if(pSPIHandle->pSPI->CR1 & (1 << SPI_CR1_DFF))
	{
		// 16-bit
		pSPIHandle->pSPI->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLength -= 2;
		(uint16_t*)(pSPIHandle->pTxBuffer)++;
	}else
	{
		// 8-bit
		pSPIHandle->pSPI->DR = *(pSPIHandle->pTxBuffer);
		(pSPIHandle->TxLength)--;
		(pSPIHandle->pTxBuffer)++;
	}

	if(! pSPIHandle->TxLength)
	{
		// Close the SPI transmission
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);
	}
}

static void SPI_RXNE_Interrupt_Handle(SPI_Handle *pSPIHandle)
{
	if((pSPIHandle->pSPI->CR1) & (1 << SPI_CR1_DFF))
	{
		// 16-bit
		*((uint16_t*)pSPIHandle->pRxBuffer) = (uint16_t)pSPIHandle->pSPI->DR;
		pSPIHandle->RxLength -= 2;
		pSPIHandle->pRxBuffer -= 2;
	}else
	{
		// 8-bit
		*(pSPIHandle->pRxBuffer) = pSPIHandle->pSPI->DR;
		(pSPIHandle->RxLength)--;
		(pSPIHandle->pRxBuffer)--;
	}

	if(! pSPIHandle->RxLength)
	{
		// Close the SPI reception
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}
}

static void SPI_OVR_Interrupt_Handle(SPI_Handle *pSPIHandle)
{
	uint8_t temp;
	// Clear OVR flag
	if(pSPIHandle->TxState != SPI_BUSY_TX)
	{
		temp = pSPIHandle->pSPI->DR;
		temp = pSPIHandle->pSPI->SR;
	}
	(void)temp;

	// Inform the application
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);
}

void SPI_CloseTransmission(SPI_Handle *pSPIHandle)
{
	// Close the SPI transmission
	pSPIHandle->pSPI->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLength = 0;
	pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle *pSPIHandle)
{
	pSPIHandle->pSPI->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLength = 0;
	pSPIHandle->RxState = SPI_READY;
}

void SPI_ClearOVRFlag(SPI_REGISTERS *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

__weak void SPI_ApplicationEventCallback(SPI_Handle *pSPIHandle, uint8_t AppEvent)
{

}

