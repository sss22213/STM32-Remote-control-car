#include "MPU6050.h"
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

		Delay_Ms(200);

		I2C_WriteByte(0xD0,0x6B,0x80);
		
		Delay_Ms(200);

		I2C_WriteByte(0xD0,0x6B,0x00);

		Delay_Ms(200);
	 
	    I2C_WriteByte(0xD0,0x19,0x07);  //SMPLRT_DIV
		  
		I2C_WriteByte(0xD0,0x1A,0x06);  //Config

		 
		I2C_WriteByte(0xD0,0x1B,0xE0);  //GYRO_CONFIG
	
	   	I2C_WriteByte(0xD0,0x1C,0xF8);  //ACCEL_CONFIG
			
		Delay_Ms(200);
	}
	float MPU6050_Temp(void)
	{
		u8	Temp_High;
		u8	Temp_Low;
		s16	Temp;
			
		Temp_High=I2C_ReadByte(0xD0,0x41);
		Delay_Ms(100);
		Temp_Low=I2C_ReadByte(0xD0,0x42);
		Delay_Ms(100);
		Temp=((Temp_High)<<8)+Temp_Low;
		return (float)(Temp/340.0+36.53); 	
	   	
	}
	u16	MPU6050_Get(char address)
	{
		u16 result;	
		
						
		result=I2C_ReadByte(0xD0,address)<<8;
		Delay_Ms(100);
		result|=I2C_ReadByte(0xD0,address+1);
		Delay_Ms(100);

		return result;
			
	
	}
	//A
	u16 MPU6050_ACCEL_XOUT(void)
	{
		  return MPU6050_Get(0x3B); 
	   	
	}
	u16 MPU6050_ACCEL_YOUT(void)
	{
		  return MPU6050_Get(0x3D); 
	   	
	}
	u16 MPU6050_ACCEL_ZOUT(void)
	{
		  return MPU6050_Get(0x3F); 
	   	
	}
	float MPU6050_ACCEL_XOUT_M2s(void)
	{
		return (float)MPU6050_Get(0x3B)/16384.0*9.8;
	
	}
	float MPU6050_ACCEL_YOUT_M2s(void)
	{
		return (float)MPU6050_Get(0x3D)/16384.0*9.86;
	
	}
	float MPU6050_ACCEL_ZOUT_M2s(void)
	{
		return (float)MPU6050_Get(0x3F)/16384.0*9.86;
	
	}

	//G
	u16 MPU6050_GYRO_XOUT(void)
	{
		  return MPU6050_Get(0x43); 
	   	
	}
	u16 MPU6050_GYRO_YOUT(void)
	{
		  return MPU6050_Get(0x45); 
	   	
	}
	u16 MPU6050_GYRO_ZOUT(void)
	{
		  return MPU6050_Get(0x47); 
	   	
	}
	float MPU6050_GYRO_XOUT_M2s(void)
	{
		return (float)MPU6050_Get(0x43)/16.4/54.3;
	
	}
	float MPU6050_GYRO_YOUT_M2s(void)
	{
		return (float)MPU6050_Get(0x45)/16.4/54.3;
	
	}
	float MPU6050_GYRO_ZOUT_M2s(void)
	{
		return (float)MPU6050_Get(0x47)/16.4/54.3;
	
	}
