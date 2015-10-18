#include <math.h>
#include "IMU.h"

#define Kp 2.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer

#define Ki 0.005f                // integral gain governs rate of convergence of gyroscope biases

#define halfT 0.5f                // half the sample period

//---------------------------------------------------------------------------------------------------

// Variable definitions


		float q00 = 1, q11 = 0, q22 = 0, q33 = 0;        // quaternion elements representing the estimated orientation

		float exInt = 0, eyInt = 0, ezInt = 0;        // scaled integral error


void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) {

      float norm;

      float vx, vy, vz;

      float ex, ey, ez;         

	  float	Pitch,Rool,Yaw;
     
       // normalise the measurements

        norm = sqrt(ax*ax + ay*ay + az*az);      


	

        ax = ax / norm;

        ay = ay / norm;

        az = az / norm;
		 
	   	
	   

	   // estimated direction of gravity

      	vx = 2*(q11*q33 - q00*q22);

        vy = 2*(q00*q11 + q22*q33);

        vz = q00*q00 - q11*q11 - q22*q22 + q33*q33;


	

		// error is sum of cross product between reference direction of field and direction measured by sensor

        ex = (ay*vz - az*vy);

        ey = (az*vx - ax*vz);

        ez = (ax*vy - ay*vx);

		// integral error scaled integral gain

        exInt = exInt + ex*Ki;

        eyInt = eyInt + ey*Ki;

        ezInt = ezInt + ez*Ki;



       // adjusted gyroscope measurements

        gx = gx + Kp*ex + exInt;

        gy = gy + Kp*ey + eyInt;

        gz = gz + Kp*ez + ezInt;

	  	
		
		 // integrate quaternion rate and normalise

        q00 = q00 + (-q11*gx - q22*gy - q33*gz)*halfT;

        q11 = q11 + (q00*gx + q22*gz - q33*gy)*halfT;

        q22 = q22 + (q00*gy - q11*gz + q33*gx)*halfT;

        q33 = q33 + (q00*gz + q11*gy - q22*gx)*halfT;
		
		// normalise quaternion


	

        norm = sqrt(q00*q00 + q11*q11 + q22*q22 + q33*q33);

        q00 = q00/ norm;

        q11 = q11 / norm;

        q22 = q22 / norm;

        q33 = q33 / norm;
  
	
		Pitch  = (float)asin(-2 * q11 * q33 + 2 * q00* q22)* 57.3; // pitch
        Rool = (float)atan2(2 * q22 * q33 + 2 * q00 * q11, -2 * q11 * q11 - 2 * q22* q22 + 1)* 57.3; // rollv
		//Yaw = (float)(atan2(2 * q11 * q22 + 2 * q00 * q33, (-2) * q22 * q22 + (-2) * q33 * q33 + 1) * 57.3);
		

	/*	if(Pitch-0x03>0)
		{
			USART_SendData(USART1,0x00);
		}
		else if(Pitch-0x03<0)
		{
			USART_SendData(USART1,0x01);
		
		}

		Delay_S(1);

		if(Rool-0xB0>0)
		{
			USART_SendData(USART1,0x00);
		}
		else if(Rool-0xB0<0)
		{
			USART_SendData(USART1,0x01);
		
		}

		Delay_S(1);

		if(Yaw-0xB1>0)
		{
			USART_SendData(USART1,0x00);
		}
		else if(Yaw-0xB1<0)
		{
			USART_SendData(USART1,0x01);
		
		} */
	
		USART_SendData(USART1,Pitch);
		Delay_S(1);
		USART_SendData(USART1,Rool);
		Delay_S(1);
		USART_SendData(USART1,Yaw);
	
		
}
     
