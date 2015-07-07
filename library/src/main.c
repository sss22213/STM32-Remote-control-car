	#include "STM32f10x_lib.h"

	void delay(int);	
	void  RCC_Configuration(void);
	void  GPIO_Configurataion(void);
	void  USART_Configurataion(void);
	void  TM2_init(void);
	void  NVIC_Configuration(void);
	void systick_configature(void);
	void Delay_Ms(int ms);
	void L298N_Control(int Command);
	//void EXTI_Configurature(void);
	int HCSR04_TRIG(void);
	extern int count; //計數值
	int main()
	{	  
			
		RCC_Configuration();
		GPIO_Configurataion();
		NVIC_Configuration();
		//EXTI_Configurature();
		USART_Configurataion();
		systick_configature();
		TM2_init();
		
		while(1)
		{ 
		 
			USART_SendData(USART1,HCSR04_TRIG());
			Delay_Ms(1000);	
		} 
		

	}
	void  RCC_Configuration(void)
	{
		ErrorStatus HSEStartUpStatus;

		RCC_DeInit();
					  
		RCC_HSEConfig(RCC_HSE_ON);

		HSEStartUpStatus=RCC_WaitForHSEStartUp();
	
		if(HSEStartUpStatus==SUCCESS)
		{
			//系統時鐘設定
			RCC_HCLKConfig(RCC_SYSCLK_Div1);

			RCC_PCLK2Config(RCC_HCLK_Div1);

			RCC_PCLK1Config(RCC_HCLK_Div2);
		   //FLASH設定
			FLASH_SetLatency(FLASH_Latency_2);

			FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		   //PLL設定
			RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);

			RCC_PLLCmd(ENABLE);

			while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);

			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

			while(RCC_GetSYSCLKSource()!=0x08);
		 
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

		}								  
	
	}
	void GPIO_Configurataion(void)
	{
	

		GPIO_InitTypeDef g;

		
		g.GPIO_Pin=GPIO_Pin_12;

		g.GPIO_Mode=GPIO_Mode_Out_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);  
		//HCSR04

		g.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_0;

		g.GPIO_Mode=GPIO_Mode_IN_FLOATING;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);

	    //TX
		g.GPIO_Pin=GPIO_Pin_9;

		g.GPIO_Mode=GPIO_Mode_AF_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);

		//RX	

		g.GPIO_Pin=GPIO_Pin_10;

		g.GPIO_Mode=GPIO_Mode_IN_FLOATING;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);	  
				  


		//Moto_Control

		g.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_3;

		g.GPIO_Mode=GPIO_Mode_Out_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOC,&g);
		
		g.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_4;
	  
		g.GPIO_Mode=GPIO_Mode_Out_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);	  	  
		
	
	}
	void NVIC_Configuration()
	{
		//TIM2中斷,分時系統用

		NVIC_InitTypeDef NVIC_Struct;

		NVIC_Struct.NVIC_IRQChannel=TIM2_IRQChannel;

		NVIC_Struct.NVIC_IRQChannelPreemptionPriority=2;

		NVIC_Struct.NVIC_IRQChannelSubPriority=0;

		NVIC_Struct.NVIC_IRQChannelCmd=ENABLE;

		NVIC_Init(&NVIC_Struct);

		/*外部中斷

		NVIC_Struct.NVIC_IRQChannel=EXTI3_IRQChannel;

		NVIC_Struct.NVIC_IRQChannelPreemptionPriority=1;

		NVIC_Struct.NVIC_IRQChannelSubPriority=0;

		NVIC_Struct.NVIC_IRQChannelCmd=ENABLE;

		NVIC_Init(&NVIC_Struct);			  */

	}
	void TM2_init()
	{

		//大約1us中斷一次
		TIM_TimeBaseInitTypeDef TIM_Basestruct;

		TIM_Basestruct.TIM_Prescaler=5.500;
		TIM_Basestruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_Basestruct.TIM_Period=9.999;
		TIM_Basestruct.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_Basestruct.TIM_RepetitionCounter=0;
		TIM_TimeBaseInit(TIM2,&TIM_Basestruct);	

		TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
    	
    	TIM_Cmd(TIM2, ENABLE);
	
	}
	void EXTI_Configurature()
	{
		EXTI_InitTypeDef BaseEXTI;	
		BaseEXTI.EXTI_Line=EXTI_Line3;
		BaseEXTI.EXTI_Mode=EXTI_Mode_Interrupt;
 		BaseEXTI.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
 		BaseEXTI.EXTI_LineCmd=ENABLE;

		EXTI_Init(&BaseEXTI);
	
	}	 
	void systick_configature()
	{
	   SysTick_CounterCmd(SysTick_Counter_Disable);
	   SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	   SysTick_CounterCmd(SysTick_Counter_Clear);
	   
	}
	void Delay_Ms(int ms)
	{
		SysTick_SetReload(9000*ms);
		SysTick_CounterCmd(SysTick_Counter_Enable);
		while(SysTick_GetFlagStatus(SysTick_FLAG_COUNT)==0);
		SysTick_CounterCmd(SysTick_Counter_Disable);	
		SysTick_CounterCmd(SysTick_Counter_Clear);	
	}
	int HCSR04_TRIG(void) 
	{
		 int pulseIn_head,pulseIn_tie,pulseIn;
		 GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		 Delay_Ms(2);
		 GPIO_SetBits(GPIOA,GPIO_Pin_12);
		 Delay_Ms(10);
		 GPIO_ResetBits(GPIOA,GPIO_Pin_12);

		 while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0);
		 pulseIn_head=count;
		 while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1);
		 pulseIn_tie=count;
		 pulseIn=(pulseIn_tie-pulseIn_head)/58;
		 return pulseIn; 
		 

	
	}
	void USART_Configurataion(void)
	{
		//USART_Init
		USART_InitTypeDef UART;

		//USART_ClockInitTypeDef UART_Clock;

		UART.USART_BaudRate=9600;

		UART.USART_WordLength=USART_WordLength_8b;

		UART.USART_StopBits=USART_StopBits_1;
  		
		UART.USART_Parity=USART_Parity_No;
  		
		UART.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;

		UART.USART_HardwareFlowControl=USART_HardwareFlowControl_None;

  		USART_Init(USART1,&UART);

		USART_Cmd(USART1,ENABLE);
			
	}
	void L298N_Control(int Command)
	{
		switch(Command)
		{
			case 1: //正轉
				//左
				GPIO_SetBits(GPIOC,GPIO_Pin_1);
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_SetBits(GPIOA,GPIO_Pin_2);
				GPIO_ResetBits(GPIOA,GPIO_Pin_4);
				break;
			case 2:	//反轉
				//左
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				GPIO_SetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
				GPIO_SetBits(GPIOA,GPIO_Pin_4);
				break;
			case 3:	//右轉
				//左
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
				GPIO_SetBits(GPIOA,GPIO_Pin_4);
				break;
			case 4:	//右轉
				//左
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				GPIO_SetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
				GPIO_ResetBits(GPIOA,GPIO_Pin_4);
				break;							
	
		}
	
	
	
	
	}
