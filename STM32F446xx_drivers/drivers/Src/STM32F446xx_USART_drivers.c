#include"STM32F446xx_USART_drivers.h"
#include<stdint.h>

void USART_ClkCtrl(USART_REGISTERS *pUSARTx, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		if(pUSARTx == USART1)
		{
			USART1_CLOCK_ENABLE();
		}
		else if(pUSARTx == USART2)
		{
			USART2_CLOCK_ENABLE();
		}
		else if(pUSARTx == USART3)
		{
			USART3_CLOCK_ENABLE();
		}
	}
	else
	{
		if(pUSARTx == USART1)
		{
			USART1_CLOCK_DISABLE();
		}
		else if(pUSARTx == USART2)
		{
			USART2_CLOCK_DISABLE();
		}
		else if(pUSARTx == USART3)
		{
			USART3_CLOCK_DISABLE();
		}
	}
}



void USART_PeripheralControl(USART_REGISTERS *pUSARTx, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		pUSARTx->CR1 |= (1 << USART_CR1_UE);
	}else
	{
		pUSARTx->CR1 &= ~(1 << USART_CR1_UE);
	}
}

void USART_Init(USART_Handle *pUSARTHandle)
{
	uint32_t temp = 0;

	/*******************************Configuration of CR1**************************************/

	//Enable the clock for given USART peripheral
	USART_ClkCtrl(pUSARTHandle->pUSARTx, ENABLE);

	//Enable USART Tx and Rx engines according to user configuration
	if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX)
	{
		//Enable Receiver bit
		temp |= (1 << USART_CR1_RE);
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX)
	{
		//Enable Transmitter bit
		temp |= (1 << USART_CR1_TE);
	}else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)
	{
		//Enable both Receiver and Transmitter bits
		temp |= ( (1 << USART_CR1_TE) | (1 << USART_CR1_RE));
	}

	//Configure the word length
	temp |= (pUSARTHandle->USART_Config.USART_WordLength << USART_CR1_M);

	//Configure parity control
	if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EVEN)
	{
		//Enable parity control bit (EVEN by deafult)
		temp |= (1 << USART_CR1_PCE);
	}else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_ODD)
	{
		//Enable parity control bit
		temp |= (1 << USART_CR1_PCE);

		//Enable ODD parity
		temp |= (1 << USART_CR1_PS);
	}

	//Program CR1
	pUSARTHandle->pUSARTx->CR1 = temp;

	/*******************************Configuration of CR2**************************************/

	temp = 0;

	//Configure number of stop bits
	temp |= (pUSARTHandle->USART_Config.USART_NoOfStopBits << USART_CR2_STOP);

	//Program CR2
	pUSARTHandle->pUSARTx->CR2 = temp;

	/*******************************Configuration of CR3**************************************/

	temp = 0;

	//Configure USART hardware flow control
	if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOWCTRL_CTS)
	{
		temp |= (1 << USART_CR3_CTSE);
	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOWCTRL_RTS)
	{
		temp |= (1 << USART_CR3_RTSE);
	}else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOWCTRL_CTS_RTS)
	{
		temp |= ( (1 << USART_CR3_RTSE) | (1 << USART_CR3_CTSE));
	}

	//Program CR3
	pUSARTHandle->pUSARTx->CR3 = temp;

	/*******************************Configure the Baud Rate**************************************/
	USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USART_Config.USART_Baud);
}

void USART_SendData(USART_Handle *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Length)
{
	uint16_t *pData;

	//Loop over until "Length" bytes are transfered
	for (uint32_t i = 0 ; i < Length ; i++)
	{
		//Wait until TXE flag is set
		while ( !USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE));

		//Check the length of the word
		if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//If 9BIT, then load DR with 2 bytes but only the first 9
			pData = (uint16_t*)pTxBuffer;
			pUSARTHandle->pUSARTx->DR = (*pData & (uint16_t)0x01FF);

			//Check for parity control
			if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity, so 9 bits are sent. We must increase pTxBuffer twice then
				pTxBuffer++;
				pTxBuffer++;
			}else
			{
				//Parity bit is used, so 8 bits are sent. The 9th bit is replaced by parity bit by hardware
				pTxBuffer++;
			}
		}else
		{
			//8 bit data transfer
			pUSARTHandle->pUSARTx->DR = (*pTxBuffer & (uint8_t)0xFF);

			//Point to the next byte to send
			pTxBuffer++;
		}
	}

	//Wait until TC flag is set (transmission completed)
	while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC));
}

void USART_ReceiveData(USART_Handle *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Length)
{
	//Loop over until "Length" bytes are received
	for (uint32_t i = 0 ; i < Length ; i++)
	{
		//Wait until RXNE flag is set
		while ( !USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE));

		//Check the length of the word
		if (pUSARTHandle->USART_Config.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//Check for parity control
			if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//Read only first 9 bits
				*((uint16_t*)pRxBuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);

				//Increment pRxBuffer twice
				pRxBuffer++;
				pRxBuffer++;
			}else
			{
				//Read only 8 bits, the 9th bit is parity set by hardware
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0xFF);

				//Increment pRxBuffer
				pRxBuffer++;
			}
		}else
		{
			//Check for parity control
			if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//Read 8 bits from DR
				*pRxBuffer = pUSARTHandle->pUSARTx->DR;
			}else
			{
				//Read 7 bits, the 8th bits is parity set by hardware
				*pRxBuffer = (pUSARTHandle->pUSARTx->DR & (uint8_t)0x7F);
			}

			//Increment pRxBuffer
			pRxBuffer++;
		}
	}
}

uint8_t USART_SendData_Interrupt(USART_Handle *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Length)
{
	uint8_t tx_state = pUSARTHandle->TxBusyState;

	if (tx_state != USART_BUSY_IN_TX)
	{
		pUSARTHandle->TxLength = Length;
		pUSARTHandle->pTxBuffer = pTxBuffer;
		pUSARTHandle->TxBusyState = USART_BUSY_IN_TX;

		//Enable TXEIE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

		//Enable TCIE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
	}

	return tx_state;
}

uint8_t USART_ReceiveData_Interrupt(USART_Handle *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Length)
{
	uint8_t rx_state = pUSARTHandle->RxBusyState;

	if (rx_state != USART_BUSY_IN_TX)
	{
		pUSARTHandle->RxLength = Length;
		pUSARTHandle->pRxBuffer = pRxBuffer;
		pUSARTHandle->RxBusyState = USART_BUSY_IN_RX;

		//Enable RXNEIE
		pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
	}

	return rx_state;
}

uint8_t USART_GetFlagStatus(USART_REGISTERS *pUSARTx, uint8_t FlagName)
{
	if(pUSARTx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

void USART_ClearFlag(USART_REGISTERS *pUSARTx, uint8_t FlagName);

void USART_IRQInterrupConfig(uint8_t IRQNumber, uint8_t ENorDIS)
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

void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = (IRQNumber%4)*8;
	*(NVIC_IPR_BASE_ADDRESS + iprx) |= (IRQPriority << (iprx_section - 4));
}

void USART_IRQHandling(USART_Handle *pUSARTHandle);

void USART_SetBaudRate(USART_REGISTERS *pUSARTx, uint32_t BaudRate)
{

	//Variable to hold the APB clock
	uint32_t PCLKx;

	uint32_t usartdiv;

	//variables to hold Mantissa and Fraction values
	uint32_t M_part,F_part;

	uint32_t tempreg = 0;

	//Get the value of APB bus clock in to the variable PCLKx
	if(pUSARTx == USART1 || pUSARTx == USART6)
	{
	   //USART1 and USART6 are hanging on APB2 bus
	   PCLKx = RCC_GetPCLK2Value();
	}else
	{
	   PCLKx = RCC_GetPCLK1Value();
	}

	//Check for OVER8 configuration bit
	if(pUSARTx->CR1 & (1 << USART_CR1_OVER8))
	{
	   //OVER8 = 1 , over sampling by 8
	   usartdiv = ((25 * PCLKx) / (2 *BaudRate));
	}else
	{
	   //over sampling by 16
		usartdiv = ((25 * PCLKx) / (4 *BaudRate));
	}

	//Calculate the Mantissa part
	M_part = usartdiv/100;

	//Place the Mantissa part in appropriate bit position . refer USART_BRR
	tempreg |= M_part << 4;

	//Extract the fraction part
	F_part = (usartdiv - (M_part * 100));

	//Calculate the final fractional
	if(pUSARTx->CR1 & ( 1 << USART_CR1_OVER8))
	{
	  //OVER8 = 1 , over sampling by 8
	  F_part = ((( F_part * 8)+ 50) / 100)& ((uint8_t)0x07);
	}else
	{
	   //over sampling by 16
	   F_part = ((( F_part * 16)+ 50) / 100) & ((uint8_t)0x0F);
	}

	//Place the fractional part in appropriate bit position . refer USART_BRR
	tempreg |= F_part;

	//copy the value of tempreg in to BRR register
	pUSARTx->BRR = tempreg;
}
