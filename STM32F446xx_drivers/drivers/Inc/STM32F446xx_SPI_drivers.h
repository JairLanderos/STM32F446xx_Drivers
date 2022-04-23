#ifndef INC_STM32F446XX_SPI_DRIVERS_H_
#define INC_STM32F446XX_SPI_DRIVERS_H_

#include"STM32F446xx.h"

#define SPI_MASTER_MODE 1
#define SPI_SLAVE_MODE 0

#define SPI_FULL_DUPLEX 1
#define SPI_HALF_DUPLEX 2
#define SPI_RX_ONLY 3

#define SPI_SCLK_SPEED_DIV2 0
#define SPI_SCLK_SPEED_DIV4 1
#define SPI_SCLK_SPEED_DIV8 2
#define SPI_SCLK_SPEED_DIV16 3
#define SPI_SCLK_SPEED_DIV32 4
#define SPI_SCLK_SPEED_DIV64 5
#define SPI_SCLK_SPEED_DIV128 6
#define SPI_SCLK_SPEED_DIV256 7

#define SPI_DATA_FRAME_8BIT 0
#define SPI_DATA_FRAME_16BIT 1

#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW 0

#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW 0

#define SPI_SSM_ENABLE 1
#define SPI_SSM_DISABLE 0

#define SPI_TXE_FLAG (1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG (1 << SPI_SR_RXNE)
#define SPI_BSY_FLAG (1 << SPI_SR_BSY)

#define SPI_READY 0
#define SPI_BUSY_RX 1
#define SPI_BUSY_TX 2

#define SPI_EVENT_TX_CMPLT 1
#define SPI_EVENT_RX_CMPLT 2
#define SPI_EVENT_OVR_ERR 3
#define SPI_EVENT_CRC_ERR 4

// Structure to configure SPI settings
typedef struct
{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SCLKSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_CFG;

typedef struct
{
	SPI_REGISTERS *pSPI;
	SPI_CFG SPI_CONFIG;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLength;
	uint32_t RxLength;
	uint8_t TxState;
	uint8_t RxState;
}SPI_Handle;


void SPI_Init(SPI_Handle *pSPIHandle);
void SPI_DeInit(SPI_REGISTERS *pSPIx);
void SPI_ClkCtrl(SPI_REGISTERS *pSPIx, uint8_t ENorDIS);
void SPI_SendData(SPI_REGISTERS *pSPIx, uint8_t *pTxBuffer, uint32_t Length);
void SPI_ReceiveData(SPI_REGISTERS *pSPIx, uint8_t *pRxBuffer, uint32_t Length);
uint8_t SPI_SendData_Int(SPI_Handle *pSPIHandle, uint8_t *pTxBuffer, uint32_t Length);
uint8_t SPI_ReceiveData_Int(SPI_Handle *pSPIHandle, uint8_t *pRxBuffer, uint32_t Length);
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t ENorDIS);
void SPI_IRQHandler(SPI_Handle *pSPIHandle);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_PeripheralControl(SPI_REGISTERS *pSPIx, uint8_t ENorDIS);
void SPI_SSIConfig(SPI_REGISTERS *pSPIx, uint8_t ENorDIS);
void SPI_SSOEConfig(SPI_REGISTERS *pSPIx, uint8_t ENorDIS);
uint8_t SPI_GetFlagStatus(SPI_REGISTERS *pSPIx, uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_REGISTERS *pSPIx);
void SPI_CloseTransmission(SPI_Handle *pSPIHandle);
void SPI_CloseReception(SPI_Handle *pSPIHandle);
void SPI_ApplicationEventCallback(SPI_Handle *pSPIHandle, uint8_t AppEvent);

#endif /* INC_STM32F446XX_SPI_DRIVERS_H_ */
