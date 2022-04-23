#ifndef INC_STM32F446XX_USART_DRIVERS_H_
#define INC_STM32F446XX_USART_DRIVERS_H_

#include"STM32F446xx.h"
#include<stdint.h>

/*****************************************************************/
/*		           	USART Flags from SR 					 	 */
/*****************************************************************/

#define USART_FLAG_PE (1 << USART_SR_PE)
#define USART_FLAG_FE (1 << USART_SR_FE)
#define USART_FLAG_NF (1 << USART_SR_NF)
#define USART_FLAG_ORE (1 << USART_SR_ORE)
#define USART_FLAG_IDLE (1 << USART_SR_IDLE)
#define USART_FLAG_RXNE (1 << USART_SR_RXNE)
#define USART_FLAG_TC (1 << USART_SR_TC)
#define USART_FLAG_TXE (1 << USART_SR_TXE)
#define USART_FLAG_LBD (1 << USART_SR_LBD)
#define USART_FLAG_CTS (1 << USART_SR_CTS)


/*****************************************************************/
/*		           	User macros to configure USART 			 	 */
/*****************************************************************/

#define USART_MODE_ONLY_TX 0
#define USART_MODE_ONLY_RX 1
#define USART_MODE_TXRX 2

#define USART_STD_BAUD_1200 1200
#define USART_STD_BAUD_2400 2400
#define USART_STD_BAUD_9600 9600
#define USART_STD_BAUD_19200 19200
#define USART_STD_BAUD_38400 38400
#define USART_STD_BAUD_57600 57600
#define USART_STD_BAUD_115200 115200
#define USART_STD_BAUD_230400 230400
#define USART_STD_BAUD_460800 460800
#define USART_STD_BAUD_921600 921600
#define USART_STD_BAUD_2M 2000000
#define USART_STD_BAUD_3M 3000000

#define USART_PARITY_ODD 2
#define USART_PARITY_EVEN 1
#define USART_PARITY_DISABLE 0

#define USART_WORDLEN_8BITS 0
#define USART_WORDLEN_9BITS 1

#define USART_STOPBITS_1 0
#define USART_STOPBITS_0_5 1
#define USART_STOPBITS_2 2
#define USART_STOPBITS_1_5 3

#define USART_HW_FLOWCTRL_NONE 0
#define USART_HW_FLOWCTRL_CTS 1
#define USART_HW_FLOWCTRL_RTS 2
#define USART_HW_FLOWCTRL_CTS_RTS 3

#define USART_BUSY_IN_RX 1
#define USART_BUSY_IN_TX 2
#define USART_READY 0


typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_CFG;

typedef struct
{
	USART_REGISTERS *pUSARTx;
	USART_CFG USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLength;
	uint32_t RxLength;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_Handle;



/*****************************************************************/
/*		           	Function Prototypes 					 	 */
/*****************************************************************/

void USART_ClkCtrl(USART_REGISTERS *pUSARTx, uint8_t ENorDIS);

void USART_PeripheralControl(USART_REGISTERS *pUSARTx, uint8_t ENorDIS);

void USART_Init(USART_Handle *pUSARTHandle);

void USART_SendData(USART_Handle *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Length);

void USART_ReceiveData(USART_Handle *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Length);

uint8_t USART_SendData_Interrupt(USART_Handle *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Length);

uint8_t USART_ReceiveData_Interrupt(USART_Handle *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Length);

uint8_t USART_GetFlagStatus(USART_REGISTERS *pUSARTx, uint8_t FlagName);

void USART_ClearFlag(USART_REGISTERS *pUSARTx, uint8_t FlagName);

void USART_IRQInterrupConfig(uint8_t IRQNumber, uint8_t ENorDIS);

void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

void USART_IRQHandling(USART_Handle *pUSARTHandle);

void USART_SetBaudRate(USART_REGISTERS *pUSARTx, uint32_t BaudRate);


#endif /* INC_STM32F446XX_USART_DRIVERS_H_ */
