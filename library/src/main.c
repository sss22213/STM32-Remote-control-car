	#include "STM32f10x_lib.h"

	void delay(int);	
	void  RCC_Configuration(void);
	void  GPIO_Configurataion(void);
	void  USART_Configurataion(void);
	void  TM2_init(void);
	void  NVIC_Configuration(void);
	void systick_configature(void);
	void Delay_Ms(int ms);
	void Delay_us(int us);
	void Delay_S(int s);
	void L298N_Control(int Command);
	void MPU6050_Init(void);
	void I2C_WriteByte(u8 DeviceAddress,u8 MemoryAddress,u8 data);
	u8 I2C_ReadByte(u8 DeviceAddress,u8 MemoryAddress);
	int HCSR04_TRIG(void);
	extern int count; //計數值
	int main()
	{	  
			
		RCC_Configuration();
		GPIO_Configurataion();
		NVIC_Configuration();
		USART_Configurataion();
		systick_configature();
		TM2_init();
		MPU6050_Init();
		while(1)
		{ 
		 	//L298N_Control(1);
			//Delay_Ms(1000);
			USART_SendData(USART1,I2C_ReadByte(0xD0,0x3B));
			//L298N_Control(4);
			Delay_S(1);		
			
			//USART_SendData(USART1,0x02);
				
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
		 
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_I2C1,ENABLE);

		 
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);

		}								  
	
	}
	/*****************************************/
	/*NAME:			GPIO_Configurataion		**/
	/*INPUT:		NULL					**/
	/*RETURN:		NULL					**/
	/*HCSR04:		PA12,PA3,PA0			**/
	/*UART:			PA9,PA10				**/
	/*Moto_Control:	PC1,PC3,PA2,PA4			**/
	/*****************************************/
	void GPIO_Configurataion(void)
	{
	

		GPIO_InitTypeDef g;

		g.GPIO_Pin=GPIO_Pin_4;

		g.GPIO_Mode=GPIO_Mode_Out_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);

		//HCSR04,PA12,PA3,PA0

		g.GPIO_Pin=GPIO_Pin_12;

		g.GPIO_Mode=GPIO_Mode_Out_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);
		  
		g.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_0;

		g.GPIO_Mode=GPIO_Mode_IN_FLOATING;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);

	    //TX,PA9
		g.GPIO_Pin=GPIO_Pin_9;

		g.GPIO_Mode=GPIO_Mode_AF_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);

		//RX,PA10	

		g.GPIO_Pin=GPIO_Pin_10;

		g.GPIO_Mode=GPIO_Mode_IN_FLOATING;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);	  
				  


		//Moto_Control,PC1,PC3,PC7,PC9

		g.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_3;

		g.GPIO_Mode=GPIO_Mode_Out_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOC,&g);

		
		g.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_9;
	  
		g.GPIO_Mode=GPIO_Mode_Out_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOC,&g);
		
		//MPU6050,PB6,PB7

		g.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	  
		g.GPIO_Mode=GPIO_Mode_AF_OD;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOB,&g);

	
		
	
	}
	/*********************************************/
	/*NAME:			NVIC_Configuration			**/
	/*INPUT:		NULL						**/
	/*RETURN:		NULL						**/
	/*TIM2:	Preemption:2,SubPriority:0			**/
	/*********************************************/
	void NVIC_Configuration()
	{
		//TIM2中斷,分時系統用

		NVIC_InitTypeDef NVIC_Struct;

		NVIC_Struct.NVIC_IRQChannel=TIM2_IRQChannel;

		NVIC_Struct.NVIC_IRQChannelPreemptionPriority=2;

		NVIC_Struct.NVIC_IRQChannelSubPriority=0;

		NVIC_Struct.NVIC_IRQChannelCmd=ENABLE;

		NVIC_Init(&NVIC_Struct);
		

	}
	/*********************************************/
	/*NAME:			TM2_init					**/
	/*INPUT:		NULL						**/
	/*RETURN:		NULL						**/
	/*大約1us中斷一次							**/
	/*********************************************/
	void TM2_init()
	{

		//大約1us中斷一次
		TIM_TimeBaseInitTypeDef TIM_Basestruct;

//		TIM_ICInitTypeDef TIM_ICInitStructure;
		
		TIM_Basestruct.TIM_Prescaler=5.500;
		TIM_Basestruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_Basestruct.TIM_Period=9.999;
		TIM_Basestruct.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_Basestruct.TIM_RepetitionCounter=0;
		TIM_TimeBaseInit(TIM2,&TIM_Basestruct);	

		TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
    	
    	TIM_Cmd(TIM2, ENABLE);


	
	}
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
	/*********************************************/
	/*NAME:			HCSR04_TRIG					**/
	/*INPUT:		NULL						**/
	/*RETURN:		int							**/
	/*********************************************/
	int HCSR04_TRIG(void) 
	{
		 long pulseIn_head,pulseIn_tie,pulseIn,n;
		 GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		 Delay_us(2);
		 GPIO_SetBits(GPIOA,GPIO_Pin_12);
		 Delay_us(10);
		 GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		
		 while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0);
		 pulseIn_head=count;
		 while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==1);
		 pulseIn_tie=count;														
		 pulseIn=(pulseIn_tie-pulseIn_head)/58;

		 //校正
		 n=(pulseIn-4)/2+1;

		 pulseIn=pulseIn+3*n-2;

		 return pulseIn; 
		 

	
	}
	/*********************************************/
	/*NAME:			USART_Configurataion		**/
	/*INPUT:		NULL						**/
	/*RETURN:		NULL						**/
	/*9600,N81									**/
	/*********************************************/
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
	/*********************************************/
	/*NAME:			L298N_Control				**/
	/*INPUT:		int Command					**/
	/*RETURN:		NULL						**/
	/*********************************************/
	void L298N_Control(int Command)
	{
		switch(Command)
		{
			case 1: //正轉
				//左
				GPIO_SetBits(GPIOC,GPIO_Pin_1);
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_SetBits(GPIOC,GPIO_Pin_7);
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);
				break;
			case 2:	//反轉
				//左
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				GPIO_SetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);
				GPIO_SetBits(GPIOC,GPIO_Pin_9);
				break;										  
			case 3:	//右轉
				//左
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);
				GPIO_SetBits(GPIOC,GPIO_Pin_9);
				break;
			case 4:	//右轉
				//左
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				GPIO_SetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);
				break;							
	
		}

	}
	void MPU6050_Init(void)
	{
		
		//u8 BUF;
		I2C_InitTypeDef I2C_Con;
		
		I2C_Con.I2C_Mode=I2C_Mode_I2C;
 		I2C_Con.I2C_DutyCycle=I2C_DutyCycle_2;
		I2C_Con.I2C_OwnAddress1=0xA0;
		I2C_Con.I2C_Ack=I2C_Ack_Enable;
		I2C_Con.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
		I2C_Con.I2C_ClockSpeed=50000;

	
		I2C_Init(I2C1,&I2C_Con);
		I2C_Cmd(I2C1,ENABLE);
		I2C_AcknowledgeConfig(I2C1,ENABLE);

		Delay_Ms(1000);

		I2C_WriteByte(0xD0,0x6B,0x80);
		
		Delay_Ms(200);

		I2C_WriteByte(0xD0,0x6B,0x00);

		Delay_Ms(200);
	 
	    I2C_WriteByte(0xD0,0x19,0x07);  //SMPLRT_DIV
		  
		

		I2C_WriteByte(0xD0,0x1A,0x06);  //Config

		 
		I2C_WriteByte(0xD0,0x1B,0x18);  //GYRO_CONFIG
	
	   	I2C_WriteByte(0xD0,0x1C,0x01);  //ACCEL_CONFIG
		
	
	
		
	}
	void I2C_WriteByte(u8 DeviceAddress,u8 MemoryAddress,u8 data)
	{
		
		//等待I2C閒置
		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
		
		//啟動訊號
		I2C_GenerateSTART(I2C1,ENABLE);	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);

		//發送裝置位置
		I2C_Send7bitAddress(I2C1,DeviceAddress,I2C_Direction_Transmitter);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);


		//發送記憶體位置
		I2C_SendData(I2C1,MemoryAddress);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);

		//發送數據
		I2C_SendData(I2C1,data);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);

		//停止訊號
		I2C_GenerateSTOP(I2C1,ENABLE);

		 /* Clear AF flag */
        I2C_ClearFlag(I2C1, I2C_FLAG_AF);
        /* STOP condition */    
        I2C_GenerateSTOP(I2C1, ENABLE);  
	}
	u8 I2C_ReadByte(u8 DeviceAddress,u8 MemoryAddress)
	{
		
		u8 data;

		//等待I2C閒置
		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
		
		//啟動訊號
		I2C_GenerateSTART(I2C1,ENABLE);	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);

		//發送裝置位置
		I2C_Send7bitAddress(I2C1,DeviceAddress,I2C_Direction_Transmitter);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
		I2C_Cmd(I2C1,ENABLE);

		//發送記憶體位置
		I2C_SendData(I2C1,MemoryAddress);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);

		//啟動訊號
		I2C_GenerateSTART(I2C1,ENABLE);	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
		
		//發送裝置位置
		I2C_Send7bitAddress(I2C1,DeviceAddress,I2C_Direction_Receiver);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);
		
		I2C_AcknowledgeConfig(I2C1,DISABLE); //ACK 關閉	
		
		//停止訊號
		I2C_GenerateSTOP(I2C1,ENABLE);

		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);

		data=I2C_ReceiveData(I2C1);			//接收資料
		
		I2C_AcknowledgeConfig(I2C1,ENABLE); 	
	
		return data;		
	
	}	
	
