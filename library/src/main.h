#ifndef __MAIN__H_
#define __MAIN__H_


#include "STM32f10x_lib.h"
#include "I2C.h"
#include "MPU6050.h"
#include "Delay.h"
#include "USART.h"
#include "AT24C02.h"
#include "IMU.h"
#include "anbt_dmp_fun.h"
#include "anbt_i2c.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"
#include <math.h>

	
//UP_Down enum
typedef enum{
	Down,
	UP,
	Float

}Look_UP_Down;

/**************************************/
//Moto_Control
typedef enum
{
	Go='G',
	Left='L',
	Right='R',
	Stop='S',
	Back='B'

}Moto_Control;
 
/**************************************/
//L298N_Control
typedef enum
{
	LNO,
	LGO,	//前進
	LBAK,	//後退
	LLET,	//左轉
	LRIG,	//右轉
	LSTOP,	//停止
}L298N_Control_par;
/*****************************************/
//Delay
void delay(int);
//Set Clock	
void  RCC_Configuration(void);
//GPIO_Init
void  GPIO_Configurataion(void);
//Timer_Init 
void  TM2_init(void);
//NVIC
void  NVIC_Configuration(void);
//Moto
void L298N_Control(int Command);

/**************************************/
//Variable
int Euler_angles(void);
extern u16 temp;
int HCSR04_TRIG(void);	  

//HCSR-04
extern int count; 

//Angle Const
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
#define q30  1073741824.0f

//safe parmenter
int dangerous=0;
int distance=0;
int Up_Down=0;

//direction Control
int control;

#endif