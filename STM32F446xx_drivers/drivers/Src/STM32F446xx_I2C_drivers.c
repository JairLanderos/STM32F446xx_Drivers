#include"STM32F446xx_I2C_drivers.h"
#include<stdint.h>

static void I2C_GenerateStartCondition(I2C_REGISTERS *pI2Cx);
static void I2C_ExecuteAddressPhase_Write(I2C_REGISTERS *pI2Cx, uint8_t SlaveAddress);
static void I2C_ClearADDRFlag(I2C_Handle *pI2CHandle);
static void I2C_ExecuteAddressPhase_Read(I2C_REGISTERS *pI2Cx, uint8_t SlaveAddress);
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle *pI2CHandle);

void I2C_Init(I2C_Handle *pI2CHandle)
{
	uint32_t temp = 0;

	I2C_ClkCtrl(pI2CHandle->pI2Cx, ENABLE);

	// ACK Control
	temp |= (pI2CHandle->I2C_CONFIG.I2C_ACKControl << I2C_CR1_ACK);
	pI2CHandle->pI2Cx->CR1 = temp;

	// FREQ Control
	temp = 0;
	temp |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (temp & 0x3F);

	// Address Control
	temp = 0;
	temp |= (pI2CHandle->I2C_CONFIG.I2C_DeviceAddress << 1);
	temp |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 = temp;

	// CCR calculations
	uint16_t ccr_value = 0;
	temp = 0;
	if(pI2CHandle->I2C_CONFIG.I2C_SCLSpeed <= I2C_SPEED_SM)
	{
		ccr_value = RCC_GetPCLK1Value() / (2*pI2CHandle->I2C_CONFIG.I2C_SCLSpeed);
		temp |= (ccr_value & 0xFFF);
	}else
	{
		temp |= (1 << I2C_CCR_FS);
		temp |= (pI2CHandle->I2C_CONFIG.I2C_FMDutyCycle << I2C_CCR_DUTY);
		if(pI2CHandle->I2C_CONFIG.I2C_FMDutyCycle == I2C_DUTY_2)
		{
			ccr_value = RCC_GetPCLK1Value() / (3*pI2CHandle->I2C_CONFIG.I2C_SCLSpeed);
		}else
		{
			ccr_value = RCC_GetPCLK1Value() / (25*pI2CHandle->I2C_CONFIG.I2C_SCLSpeed);
		}
		temp |= (ccr_value & 0xFFF);
	}
	pI2CHandle->pI2Cx->CCR = temp;

	// TRISE Configuration
	if(pI2CHandle->I2C_CONFIG.I2C_SCLSpeed <= I2C_SPEED_SM)
		{
			temp = (RCC_GetPCLK1Value() / 1000000U) + 1;
		}else
		{
			temp = ((RCC_GetPCLK1Value() * 300) / 1000000000U) + 1;
		}
	pI2CHandle->pI2Cx->TRISE = (temp & 0x3F);
}


uint32_t RCC_GetPLLClock()
{
	return 0;
}

void I2C_DeInit(I2C_REGISTERS *pI2Cx)
{

}

void I2C_ClkCtrl(I2C_REGISTERS *pI2Cx, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		if(pI2Cx == I2C1)
		{
			I2C1_CLOCK_ENABLE();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_CLOCK_ENABLE();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_CLOCK_ENABLE();
		}
	}
	else
	{
		if(pI2Cx == I2C1)
		{
			I2C1_CLOCK_DISABLE();
		}
		else if(pI2Cx == I2C2)
		{
			I2C2_CLOCK_DISABLE();
		}
		else if(pI2Cx == I2C3)
		{
			I2C3_CLOCK_DISABLE();
		}
	}
}

void I2C_IRQConfig(uint8_t IRQNumber, uint8_t ENorDIS)
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

void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = (IRQNumber%4)*8;
	*(NVIC_IPR_BASE_ADDRESS + iprx) |= (IRQPriority << (iprx_section - 4));
}

void I2C_PeripheralControl(I2C_REGISTERS *pI2Cx, uint8_t ENorDIS)
{
	if(ENorDIS == ENABLE)
	{
		pI2Cx->CR1 |= (1 << I2C_CR1_PE);
	}else
	{
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}
}

uint8_t I2C_GetFlagStatus(I2C_REGISTERS *pI2Cx, uint32_t FlagName)
{
	if(pI2Cx->SR1 & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

__weak void I2C_ApplicationEventCallback(I2C_Handle *pI2CHandle, uint8_t AppEvent)
{

}

void I2C_MasterSendData(I2C_Handle *pI2CHandle, uint8_t *pTxBuffer, uint8_t Length, uint8_t SlaveAddress, uint8_t Sr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB flag from SR1 register
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG));

	//3. Send the address of the slave with R/nW bit set (1 to read / 0 to write)
	I2C_ExecuteAddressPhase_Write(pI2CHandle->pI2Cx, SlaveAddress);

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG));

	//5. Clear the ADDR flag according to its software sequence
	I2C_ClearADDRFlag(pI2CHandle);

	//6. Send the data until length becomes 0
	while(Length > 0)
	{
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TXE_FLAG));
		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer++;
		Length--;
	}

	//7. When Length becomes 0 wait for TXE=1 and BTF=1 before generating the stop condition
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TXE_FLAG));
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_BTF_FLAG));

	//8. Generate the STOP condition
	if(Sr == I2C_NO_SR)
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
}

static void I2C_GenerateStartCondition(I2C_REGISTERS *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_START);
}

static void I2C_ExecuteAddressPhase_Write(I2C_REGISTERS *pI2Cx, uint8_t SlaveAddress)
{
	SlaveAddress = SlaveAddress << 1;
	SlaveAddress &= ~(1);
	pI2Cx->DR = SlaveAddress;
}

static void I2C_ClearADDRFlag(I2C_Handle *pI2CHandle)
{
	uint32_t dummy_read;

	//Check for device mode
	if (pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	{
		if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX )
		{
			if (pI2CHandle->RxSize == 1)
			{
				//First disable the ACK
				pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);

				//Clear the ADDR flag
				dummy_read = pI2CHandle->pI2Cx->SR1;
				dummy_read = pI2CHandle->pI2Cx->SR2;
				(void)dummy_read;
			}
		}else
		{
			//Clear the ADDR flag
			dummy_read = pI2CHandle->pI2Cx->SR1;
			dummy_read = pI2CHandle->pI2Cx->SR2;
			(void)dummy_read;
		}
	}else
	{
		//Clear the ADDR flag
		dummy_read = pI2CHandle->pI2Cx->SR1;
		dummy_read = pI2CHandle->pI2Cx->SR2;
		(void)dummy_read;
	}
}

void I2C_GenerateStopCondition(I2C_REGISTERS *pI2Cx)
{
	pI2Cx->CR1 |= (1 << I2C_CR1_STOP);
}

void I2C_MasterReceiveData(I2C_Handle *pI2CHandle, uint8_t *pRxBuffer, uint8_t Length, uint8_t SlaveAddress, uint8_t Sr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_SB_FLAG));

	//3. Send the address of the slave with R/nW bit set (1 to read / 0 to write)
	I2C_ExecuteAddressPhase_Read(pI2CHandle->pI2Cx, SlaveAddress);

	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_ADDR_FLAG));

	//5.1 Procedure to read only 1 byte from slave
	if(Length == 1)
	{
		//Disable Acking
		pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);

		//Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//Wait until RXNE becomes 1
		while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_RXNE_FLAG));

		//Generate the STOP condition
		if(Sr == I2C_NO_SR)
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

		//Read data in to buffer
		*pRxBuffer = pI2CHandle->pI2Cx->DR;
	}

	//5.2 Procedure to read data from slave when length > 1
	if(Length > 1)
	{
		//Clear the ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		for(uint32_t i = Length; i > 0; i--)
		{
			//Wait until RXNE becomes 1
			while(!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_RXNE_FLAG));

			if(i == 2)	//If last 2 bytes are remaining
			{
				//Clear the ACK bit
				pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);

				//Generate the STOP condition
				if(Sr == I2C_NO_SR)
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}

			//Read the data from data register in to buffer
			*pRxBuffer = pI2CHandle->pI2Cx->DR;

			//Next byte
			pRxBuffer++;
		}
	}

	//Re-enable acking
	if(pI2CHandle->I2C_CONFIG.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	}
}

static void I2C_ExecuteAddressPhase_Read(I2C_REGISTERS *pI2Cx, uint8_t SlaveAddress)
{
	SlaveAddress = SlaveAddress << 1;
	SlaveAddress |= 1;
	pI2Cx->DR = SlaveAddress;
}


uint8_t I2C_MasterSendData_Interrupt(I2C_Handle *pI2CHandle, uint8_t *pTxBuffer, uint8_t Length, uint8_t SlaveAddress, uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_RX) &&  (busystate != I2C_BUSY_IN_TX) )
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLength = Length;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddress;
		pI2CHandle->Sr = Sr;

		//Generate the START condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Enable the ITBUFEN Control bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Enable the ITEVTEN Control bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Enable the ITERREN Control bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}

	return busystate;
}


uint8_t I2C_MasterReceiveData_Interrupt(I2C_Handle *pI2CHandle, uint8_t *pRxBuffer, uint8_t Length, uint8_t SlaveAddress, uint8_t Sr)
{
	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_RX) &&  (busystate != I2C_BUSY_IN_TX) )
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLength = Length;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->DevAddr = SlaveAddress;
		pI2CHandle->RxSize = Length;
		pI2CHandle->Sr = Sr;

		//Generate the START condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Enable the ITBUFEN Control bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Enable the ITEVTEN Control bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Enable the ITERREN Control bit
		pI2CHandle->pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}

	return busystate;
}

void I2C_EV_IRQHandling(I2C_Handle *pI2CHandle)
{
	uint32_t temp1, temp2, temp3;

	temp1 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITBUFEN);

	//1. Handler for interrupt generated by SB event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_SB);
	if (temp1 && temp3)
	{
		if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddressPhase_Write(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddressPhase_Read(pI2CHandle->pI2Cx, pI2CHandle->DevAddr);
		}
	}

	//2. Handler for interrupt generated by ADDR event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ADDR);
	if (temp1 && temp3)
	{
		I2C_ClearADDRFlag(pI2CHandle);
	}

	//3. Handler for interrupt generated by BTF event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BTF);
	if (temp1 && temp3)
	{
		if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			if (pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE))
			{
				if (pI2CHandle->TxLength == 0)
				{
					if (pI2CHandle->Sr == I2C_NO_SR)
					{
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}

					I2C_CloseSendData(pI2CHandle);

					I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_TX_CMPLT);
				}

			}
		}else if (pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			;
		}
	}

	//4. Handler for interrupt generated by STOPF event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_STOPF);
	if (temp1 && temp3)
	{
		//We have to clear the STOPF flag by reading SR1 (already done in temp3) and then writing to CR1
		pI2CHandle->pI2Cx->CR1 |= 0x0000;

		//Notify the application that STOP condition has been detected
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
	}

	//5. Handler for interrupt generated by TXE event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TXE);
	if (temp1 && temp2 && temp3)
	{
		if (pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			if (pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
			{
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}else
		{
			if (pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA))
			{
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
			}
		}
	}

	//6. Handler for interrupt generated by RXNE event
	temp3 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_RXNE);
	if (temp1 && temp2 && temp3)
	{
		if (pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
			{
				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		}else
		{
			if (!(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_TRA)))
			{
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
			}
		}
	}
}

static void I2C_MasterHandleTXEInterrupt(I2C_Handle *pI2CHandle)
{
	if (pI2CHandle->TxLength > 0)
	{
		//Load the data into DR
		pI2CHandle->pI2Cx->DR = *(pI2CHandle->pTxBuffer);
		//Decrement the TxLength
		pI2CHandle->TxLength--;
		//Increment the buffer address
		pI2CHandle->pTxBuffer++;
	}
}

static void I2C_MasterHandleRXNEInterrupt(I2C_Handle *pI2CHandle)
{
	if (pI2CHandle->RxSize == 1)
	{
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->RxLength--;
	}

	if (pI2CHandle->RxSize > 1)
	{
		if (pI2CHandle->RxLength == 2)
		{
			//Clear the ACK bit
			pI2CHandle->pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
		}
		//Read DR
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
		pI2CHandle->pRxBuffer++;
		pI2CHandle->RxLength--;
	}

	if (pI2CHandle->RxLength == 0)
	{
		//Generate the STOP condition
		if (pI2CHandle->Sr == I2C_NO_SR)
		{
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}
		//Close the I2C RX
		I2C_CloseReceiveData(pI2CHandle);
		//Notify the application
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
	}
}

void I2C_ER_IRQHandling(I2C_Handle *pI2CHandle)
{
	uint32_t temp1, temp2;
	temp2 = pI2CHandle->pI2Cx->CR2 & (1 << I2C_CR2_ITERREN);

	//1. Handler for interrupt generated by BERR error
	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_BERR);
	if (temp1 && temp2)
	{
		//Clear the BERR flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_BERR);

		//Notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_BERR);
	}

	//2. Handler for interrupt generated by ARLO error
	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_ARLO);
	if (temp1 && temp2)
	{
		//Clear the ARLO flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_ARLO);

		//Notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_ARLO);
	}

	//3. Handler for interrupt generated by OVR error
	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_OVR);
	if (temp1 && temp2)
	{
		//Clear the OVR flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_OVR);

		//Notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_OVR);
	}

	//4. Handler for interrupt generated by AF error
	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_AF);
	if (temp1 && temp2)
	{
		//Clear the AF flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_AF);

		//Notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_AF);
	}

	//5. Handler for interrupt generated by TIMEOUT error
	temp1 = pI2CHandle->pI2Cx->SR1 & (1 << I2C_SR1_TIMEOUT);
	if (temp1 && temp2)
	{
		//Clear the TIMEOUT flag
		pI2CHandle->pI2Cx->SR1 &= ~(1 << I2C_SR1_TIMEOUT);

		//Notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle, I2C_ERROR_TIMEOUT);
	}
}

void I2C_CloseReceiveData(I2C_Handle *pI2CHandle)
{
	//Disable ITBUFEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Disable ITEVTEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLength = 0;
	pI2CHandle->RxSize = 0;
	//Re-enable acking
	if(pI2CHandle->I2C_CONFIG.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		pI2CHandle->pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	}
}

void I2C_CloseSendData(I2C_Handle *pI2CHandle)
{
	//Disable ITBUFEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Disable ITEVTEN Control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLength = 0;
}

void I2C_SlaveSendData(I2C_REGISTERS *pI2Cx, uint8_t data)
{
	pI2Cx->DR = data;
}

uint8_t I2C_SlaveReceiveData(I2C_REGISTERS *pI2Cx)
{
	return pI2Cx->DR;
}


void I2C_SlaveEnableCallbackEvents(I2C_REGISTERS *pI2Cx, uint8_t ENorDIS)
{
	if (ENorDIS == ENABLE)
	{
		//Enable the ITBUFEN Control bit
		pI2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);

		//Enable the ITEVTEN Control bit
		pI2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Enable the ITERREN Control bit
		pI2Cx->CR2 |= (1 << I2C_CR2_ITERREN);
	}else
	{
		//Enable the ITBUFEN Control bit
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

		//Enable the ITEVTEN Control bit
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

		//Enable the ITERREN Control bit
		pI2Cx->CR2 &= ~(1 << I2C_CR2_ITERREN);
	}
}
