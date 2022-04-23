#ifndef INC_STM32F446XX_RCC_DRIVERS_H_
#define INC_STM32F446XX_RCC_DRIVERS_H_

#include"STM32F446xx.h"

//Returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);

//Returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);

#endif /* INC_STM32F446XX_RCC_DRIVERS_H_ */
