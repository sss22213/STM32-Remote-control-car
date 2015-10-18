#ifndef __MPU6050_H
#define __MPU6050_H


#include "stm32f10x_lib.h"
#include "I2C.h"
#include "Delay.h"
#include <math.h>
   

		
float MPU6050_ACCEL_ZOUT_M2s(void);
/*********************************************************/
/*NAME:			MPU6050_Init							**/
/*INPUT:		NULL									**/
/*RETURN:		NULL									**/
/*********************************************************/
void MPU6050_Init(void);

/*********************************************************/
/*NAME:			MPU6050_Temp							**/
/*INPUT:		NULL									**/
/*RETURN:		float									**/
/*Temperature in degrees C								**/
/*********************************************************/
float MPU6050_Temp(void);

u16	MPU6050_Get(char address);

/*********************************************************/
/*NAME:			MPU6050_ACCEL_XOUT						**/
/*INPUT:		NULL									**/
/*RETURN:		u16										**/
/*********************************************************/
u16	MPU6050_ACCEL_XOUT(void);

/*********************************************************/
/*NAME:			MPU6050_ACCEL_YOUT						**/
/*INPUT:		NULL									**/
/*RETURN:		u16										**/
/*********************************************************/
u16 MPU6050_ACCEL_YOUT(void);


/*********************************************************/
/*NAME:			MPU6050_ACCEL_XOUT						**/
/*INPUT:		NULL									**/
/*RETURN:		u16										**/
/*********************************************************/
u16 MPU6050_ACCEL_ZOUT(void);

/*********************************************************/
/*NAME:			MPU6050_ACCEL_XOUT_M2s					**/
/*INPUT:		NULL									**/
/*RETURN:		float									**/
/*M/s^2													**/
/*********************************************************/
float MPU6050_ACCEL_XOUT_M2s(void);

/*********************************************************/
/*NAME:			MPU6050_ACCEL_YOUT_M2s					**/
/*INPUT:		NULL									**/
/*RETURN:		float									**/
/*M/s^2													**/
/*********************************************************/
float MPU6050_ACCEL_YOUT_M2s(void);

/*********************************************************/
/*NAME:			MPU6050_ACCEL_ZOUT_M2s					**/
/*INPUT:		NULL									**/
/*RETURN:		float									**/
/*M/s^2													**/
/*********************************************************/
	u16 MPU6050_GYRO_XOUT(void);
	
	u16 MPU6050_GYRO_YOUT(void);

	u16 MPU6050_GYRO_ZOUT(void);
	
	float MPU6050_GYRO_XOUT_M2s(void);

	float MPU6050_GYRO_YOUT_M2s(void);

	float MPU6050_GYRO_ZOUT_M2s(void);


#endif

