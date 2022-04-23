#include"STM32F446xx_RCC_drivers.h"

uint16_t AHB_Prescaler[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_Prescaler[4] = {2,4,8,16};
uint8_t APB2_Prescaler[4] = {2,4,8,16};

uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1, SystemClk;
	uint8_t clk_src, temp, ahbp, apb1p;

	clk_src = ((RCC->CFGR >> 2) & 0x3);

	if(clk_src == 0)
	{
		SystemClk = 16000000;
	}else if(clk_src == 1)
	{
		SystemClk = 8000000;
	}else if(clk_src == 2)
	{
		SystemClk = RCC_GetPLLClock();
	}

	temp = ((RCC->CFGR >> 4) & 0xF);
	if(temp < 8)
	{
		ahbp = 1;
	}else
	{
		ahbp = AHB_Prescaler[temp-8];
	}

	temp = ((RCC->CFGR >> 10) & 0x7);
	if(temp < 4)
	{
		apb1p = 1;
	}else
	{
		apb1p = APB1_Prescaler[temp-4];
	}

	pclk1 = ((SystemClk/ahbp)/apb1p);
	return pclk1;
}


uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pclk2, SystemClk;
	uint8_t clk_src, temp, ahbp, apb2p;

	clk_src = ((RCC->CFGR >> 2) & 0x3);

	if(clk_src == 0)
	{
		SystemClk = 16000000;
	}else if(clk_src == 1)
	{
		SystemClk = 8000000;
	}else if(clk_src == 2)
	{
		SystemClk = RCC_GetPLLClock();
	}

	temp = ((RCC->CFGR >> 4) & 0xF);
	if(temp < 8)
	{
		ahbp = 1;
	}else
	{
		ahbp = AHB_Prescaler[temp-8];
	}

	temp = ((RCC->CFGR >> 13) & 0x7);
	if(temp < 4)
	{
		apb2p = 1;
	}else
	{
		apb2p = APB2_Prescaler[temp-4];
	}

	pclk2 = ((SystemClk/ahbp)/apb2p);
	return pclk2;
}
