	#include "main.h"
	int main()
	{		
	  	 
		RCC_Configuration();
		GPIO_Configurataion();
		NVIC_Configuration();
		USART_Configurataion();
		systick_configature();
		TM2_init();
		Delay_Ms(20);
		ANBT_I2C_Configuration();
	 	Delay_Ms(30); 
		AnBT_DMP_MPU6050_Init();

	
	
		//初始化完成
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);

		Delay_Ms(30); 
		while(1)
		{
			 
			/**********HCSR-04 障礙物偵測************/
		 	distance=HCSR04_TRIG();

			if(distance<60)
			{
				Delay_Ms(10);
				USART_SendData(USART3,'D');
				//control=5;				  
			}
			else if(distance>60)
			{			
				Delay_Ms(10);
				USART_SendData(USART3,'A');	
			}  
		
		 	Delay_Ms(40);
		   /****************上下坡偵測*********/
			Up_Down=Euler_angles();
			
			 switch(Up_Down)
			 {
			 	case Down:	
					Delay_Ms(10);
					USART_SendData(USART3,'O');
					break;
			 	case UP:	
					control=1;	
					Delay_Ms(10);
					USART_SendData(USART3,'U');
					break;
				case Float:
					GPIO_ResetBits(GPIOA,GPIO_Pin_6);
					Delay_Ms(10);
					USART_SendData(USART3,'F');
					break;
				default:break;
			 }
			Delay_Ms(10);

			//溫度
			
		   	USART_SendData(USART3,Soft_I2C_ReadTemp()-3+10);		
			Delay_Ms(3);
			USART_SendData(USART1,temp);

		   	/***************方向控制*************/
			 switch(temp)
			 {
			 	case Go:	control=1;	break;
				case Left:	control=3;	break;
				case Right:	control=4;	break;
				case Stop:	control=5;	break;
				case Back:	control=2;	break;
				default:break;
			 }
		
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
		 
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4|RCC_APB1Periph_USART3|RCC_APB1Periph_I2C1,ENABLE);

		 
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

		g.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;

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

		//USART1
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
		
		//USART3		  
		 //TX,PB10
		g.GPIO_Pin=GPIO_Pin_10;

		g.GPIO_Mode=GPIO_Mode_AF_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOB,&g);

		//RX,PB11	

		g.GPIO_Pin=GPIO_Pin_11;

		g.GPIO_Mode=GPIO_Mode_IN_FLOATING;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOB,&g);	  
		
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
		
		//PA14
		  
		g.GPIO_Pin=GPIO_Pin_14;

		g.GPIO_Mode=GPIO_Mode_Out_PP;

		g.GPIO_Speed=GPIO_Speed_50MHz;

		GPIO_Init(GPIOA,&g);

		
	
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


		NVIC_Struct.NVIC_IRQChannel=USART3_IRQChannel;

		NVIC_Struct.NVIC_IRQChannelPreemptionPriority=3;

		NVIC_Struct.NVIC_IRQChannelSubPriority=0;

		NVIC_Struct.NVIC_IRQChannelCmd=ENABLE;

		NVIC_Init(&NVIC_Struct);
	 

		NVIC_Struct.NVIC_IRQChannel=TIM4_IRQChannel;

		NVIC_Struct.NVIC_IRQChannelPreemptionPriority=4;

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
		TIM_OCInitTypeDef	OC;
		TIM_Basestruct.TIM_Prescaler=5.500;
		TIM_Basestruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_Basestruct.TIM_Period=9.999;
		TIM_Basestruct.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_Basestruct.TIM_RepetitionCounter=0;
		TIM_TimeBaseInit(TIM2,&TIM_Basestruct);	

		TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
    	
    	TIM_Cmd(TIM2, ENABLE);

		//646HZ
		TIM_Basestruct.TIM_Prescaler=500;
		TIM_Basestruct.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_Basestruct.TIM_Period=100;
		TIM_Basestruct.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_Basestruct.TIM_RepetitionCounter=0;
		TIM_TimeBaseInit(TIM4,&TIM_Basestruct);	

		TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE);
    	TIM_Cmd(TIM4, ENABLE);

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
	/*NAME:			L298N_Control				**/
	/*INPUT:		int Command					**/
	/*RETURN:		NULL						**/
	/*********************************************/
	void L298N_Control(int Command)
	{
		switch(Command)
		{
			case LGO: //正轉
				//左
				GPIO_SetBits(GPIOC,GPIO_Pin_1);
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_SetBits(GPIOC,GPIO_Pin_7);
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);
				break;
			case LBAK:	//反轉
				//左
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				GPIO_SetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);
				GPIO_SetBits(GPIOC,GPIO_Pin_9);
				break;										  
			case LLET:	
				//左
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_SetBits(GPIOC,GPIO_Pin_7);
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);
				break;
			case LRIG:	
				//左
				GPIO_SetBits(GPIOC,GPIO_Pin_1);
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);
				break;
			case LSTOP:	
				//左
				GPIO_ResetBits(GPIOC,GPIO_Pin_1);
				GPIO_ResetBits(GPIOC,GPIO_Pin_3);
				//右
				GPIO_ResetBits(GPIOC,GPIO_Pin_9);
				GPIO_ResetBits(GPIOC,GPIO_Pin_7);
				break;
		}
	}
	/*********************************************/
	/*NAME:			Euler angles				**/
	/*INPUT:		void						**/
	/*RETURN:		int							**/
	/*********************************************/
	int Euler_angles(void)
	{
		unsigned long sensor_timestamp;
		short gyro[3], accel[3], sensors;
		unsigned char more;
		long quat[4];
		float Yaw=0.00;
		float Roll,Pitch;
	  	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);	
		 
		if (sensors & INV_WXYZ_QUAT)
		{
	 	 	q0=quat[0] / q30;
		 	q1=quat[1] / q30;
		 	q2=quat[2] / q30;
		 	q3=quat[3] / q30;
		 	Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 
 		 	Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 
			Yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;		
	  
			if(Pitch>1)return 1;
			else if(Pitch<-9)return 0;
			else	return 2; 	 
		}
		return -1;
	}
