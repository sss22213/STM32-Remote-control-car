	#include "STM32f10x_lib.h"
	#include "I2C.h"
	#include "MPU6050.h"
	#include "Delay.h"
	#include "USART.h"
	#include "AT24C02.h"
	#include "anbt_dmp_fun.h"
	#include "anbt_dmp_mpu6050.h"
	#include "anbt_dmp_driver.h"
	#include <math.h>
	#define q30  1073741824.0f
	void delay(int);	
	void  RCC_Configuration(void);
	void  GPIO_Configurataion(void);
	void  TM2_init(void);
	void  NVIC_Configuration(void);
	void L298N_Control(int Command);
	//void niming(int16_t num,int16_t num2);
	int HCSR04_TRIG(void);
	char* HC05_AT(char* command);
	int speed(void);		  
	extern int count; //計數值
	//speed
	float Vxo=0,Vyo=0,Vzo=0,Vx1=0,Vy1=0,Vz1=0;
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	int V=0;
	int main()
	{	


		unsigned long sensor_timestamp;
		short gyro[3], accel[3], sensors;
		unsigned char more;
		long quat[4];
		float Yaw=0.00;
	   	float Roll,Pitch;
		int a=0,b=0,c=0;

		RCC_Configuration();
		GPIO_Configurataion();
		NVIC_Configuration();
		USART_Configurataion();
		systick_configature();
		TM2_init();
		MPU6050_Init();
		Delay_Ms(20);
		//SART_SendData(USART1,AnBT_DMP_MPU6050_Init());

		a=dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more); 
		USART_SendData(USART1,a);	
		  
		if (sensors & INV_WXYZ_QUAT )
		{
		
	 	 q0=quat[0] / q30;
		 q1=quat[1] / q30;
		 q2=quat[2] / q30;
		 q3=quat[3] / q30;
		 Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
 		 Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 
		 Yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;				
		 a=Pitch*100;
		 b=Roll*100;	 
		
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
		 
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_USART3|RCC_APB1Periph_I2C1,ENABLE);

		 
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
	/*********************************************/
	/*NAME:			HC05_AT						**/
	/*INPUT:		char* command				**/
	/*RETURN:		char*						**/
	/*********************************************/
	char* HC05_AT(char* command)
	{
		USART_Sendstring(USART3,command);
		return USART_Readstring(USART3,2);				
	}
	int speed(void)
	{
		//parmeter
		float T=0;
		float t1,t2=0;
		float	Temp=0;
		t2=count;
		T=t2-t1;

		//calue
		Temp=MPU6050_ACCEL_XOUT_M2s();
		Vx1=(Vxo+Temp*T);

		Temp=MPU6050_ACCEL_YOUT_M2s();
		Vy1=(Vyo+Temp*T);

		Temp=MPU6050_ACCEL_ZOUT_M2s();
		Vz1=(Vzo+Temp*T);

		//V=sqrt(Vx1*Vx1+Vy1*Vy1+Vz1*Vz1);
		V=sqrt(Vx1*Vx1)-0xA0;
		//restore
		Vxo=MPU6050_ACCEL_XOUT_M2s();
		Vyo=MPU6050_ACCEL_YOUT_M2s();
		Vzo=MPU6050_ACCEL_ZOUT_M2s();			
		t1=t2;

		return V;
	}  



	