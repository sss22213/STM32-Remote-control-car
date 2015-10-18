#ifndef __IMU_H
#define __IMU_H


#include "stm32f10x_lib.h"
#include "USART.h"
#include "Delay.h"


void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

#endif
