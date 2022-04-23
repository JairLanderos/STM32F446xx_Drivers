#ifndef INC_STM32F446XX_I2C_DRIVERS_H_
#define INC_STM32F446XX_I2C_DRIVERS_H_

#include"STM32F446xx.h"
#include<stdint.h>

#define I2C_SPEED_SM 100000
#define I2C_SPEED_FM 400000

#define I2C_ACK_ENABLE 1
#define I2C_ACK_DISABLE 0

#define I2C_DUTY_2 0
#define I2C_DUTY_16_9 1

#define I2C_TXE_FLAG (1 << I2C_SR1_TXE)
#define I2C_RXNE_FLAG (1 << I2C_SR1_RXNE)
#define I2C_SB_FLAG (1 << I2C_SR1_SB)
#define I2C_ADDR_FLAG (1 << I2C_SR1_ADDR)
#define I2C_BTF_FLAG (1 << I2C_SR1_BTF)
#define I2C_STOPF_FLAG (1 << I2C_SR1_STOPF)
#define I2C_BERR_FLAG (1 << I2C_SR1_BERR)
#define I2C_ARLO_FLAG (1 << I2C_SR1_ARLO)
#define I2C_AF_FLAG (1 << I2C_SR1_AF)
#define I2C_OVR_FLAG (1 << I2C_SR1_OVR)
#define I2C_TIMEOUT_FLAG (1 << I2C_SR1_TIMEOUT)

#define I2C_NO_SR RESET
#define I2C_SR SET

#define I2C_READY 0
#define I2C_BUSY_IN_RX 1
#define I2C_BUSY_IN_TX 2

#define I2C_EV_TX_CMPLT 0
#define I2C_EV_RX_CMPLT 1
#define I2C_EV_STOP 2

#define I2C_ERROR_BERR 3
#define I2C_ERROR_ARLO 4
#define I2C_ERROR_AF 5
#define I2C_ERROR_OVR 6
#define I2C_ERROR_TIMEOUT 7

#define I2C_EV_DATA_REQ 8
#define I2C_EV_DATA_RCV 9

// Structure to configure I2C settings
typedef struct
{
	uint32_t I2C_SCLSpeed;
	uint8_t I2C_DeviceAddress;
	uint8_t I2C_ACKControl;
	uint16_t I2C_FMDutyCycle;
}I2C_CFG;

typedef struct
{
	I2C_REGISTERS *pI2Cx;
	I2C_CFG I2C_CONFIG;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLength;
	uint32_t RxLength;
	uint8_t TxRxState;
	uint8_t DevAddr;
	uint32_t RxSize;
	uint8_t Sr;
}I2C_Handle;


void I2C_Init(I2C_Handle *pI2CHandle);
void I2C_DeInit(I2C_REGISTERS *pI2Cx);
void I2C_ClkCtrl(I2C_REGISTERS *pI2Cx, uint8_t ENorDIS);
void I2C_IRQConfig(uint8_t IRQNumber, uint8_t ENorDIS);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void I2C_PeripheralControl(I2C_REGISTERS *pI2Cx, uint8_t ENorDIS);
uint8_t I2C_GetFlagStatus(I2C_REGISTERS *pI2Cx, uint32_t FlagName);
void I2C_ApplicationEventCallback(I2C_Handle *pI2CHandle, uint8_t AppEvent);
void I2C_MasterSendData(I2C_Handle *pI2CHandle, uint8_t *pTxBuffer, uint8_t Length, uint8_t SlaveAddress, uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle *pI2CHandle, uint8_t *pRxBuffer, uint8_t Length, uint8_t SlaveAddress, uint8_t Sr);
uint8_t I2C_MasterSendData_Interrupt(I2C_Handle *pI2CHandle, uint8_t *pTxBuffer, uint8_t Length, uint8_t SlaveAddress, uint8_t Sr);
uint8_t I2C_MasterReceiveData_Interrupt(I2C_Handle *pI2CHandle, uint8_t *pRxBuffer, uint8_t Length, uint8_t SlaveAddress, uint8_t Sr);
void I2C_EV_IRQHandling(I2C_Handle *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle *pI2CHandle);
void I2C_CloseReceiveData(I2C_Handle *pI2CHandle);
void I2C_CloseSendData(I2C_Handle *pI2CHandle);
void I2C_GenerateStopCondition(I2C_REGISTERS *pI2Cx);
uint32_t RCC_GetPCLK1Value(void);
uint32_t RCC_GetPLLClock(void);
void I2C_SlaveSendData(I2C_REGISTERS *pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_REGISTERS *pI2Cx);
void I2C_SlaveEnableCallbackEvents(I2C_REGISTERS *pI2Cx, uint8_t ENorDIS);


#endif /* INC_STM32F446XX_I2C_DRIVERS_H_ */
