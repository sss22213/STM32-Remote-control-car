	#include "Delay.h"
	/*********************************************/
	/*NAME:			systick_configature			**/
	/*INPUT:		NULL						**/
	/*RETURN:		NULL						**/
	/*********************************************/
	void systick_configature()
	{
	   SysTick_CounterCmd(SysTick_Counter_Disable);
	   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	   SysTick_CounterCmd(SysTick_Counter_Clear);
	   
	}
	/*********************************************/
	/*NAME:			Delay_Ms					**/
	/*INPUT:		int ms						**/
	/*RETURN:		NULL						**/
	/*********************************************/
	void Delay_Ms(int ms)
	{
		SysTick_SetReload(9000*ms);
		SysTick_CounterCmd(SysTick_Counter_Enable);
		while(SysTick_GetFlagStatus(SysTick_FLAG_COUNT)==0);
		SysTick_CounterCmd(SysTick_Counter_Disable);	
		SysTick_CounterCmd(SysTick_Counter_Clear);	
	}
	void Delay_S(int s)
	{
		
		for(;s>0;s--)
		{
			Delay_Ms(1000);
			
		}
	}
	/*********************************************/
	/*NAME:			Delay_us					**/
	/*INPUT:		int us						**/
	/*RETURN:		NULL						**/
	/*********************************************/
	void Delay_us(int us)
	{
		SysTick_SetReload(9*us);
		SysTick_CounterCmd(SysTick_Counter_Enable);
		while(SysTick_GetFlagStatus(SysTick_FLAG_COUNT)==0);
		SysTick_CounterCmd(SysTick_Counter_Disable);	
		SysTick_CounterCmd(SysTick_Counter_Clear);	
	}
