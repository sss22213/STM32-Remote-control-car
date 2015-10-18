#ifndef __I2C_H
#define __I2C_H


#include "stm32f10x_lib.h"
#include "Delay.h"

/***********************************************************/
/*NAME:			I2C_WriteByte							****/
/*INPUT:		u8 DeviceAddress,u8 MemoryAddress,u8 data***/
/*RETURN:		u16										****/
/***********************************************************/
void I2C_WriteByte(u8 DeviceAddress,u8 MemoryAddress,u8 data);


/***********************************************************/
/*NAME:			I2C_ReadByte							****/
/*INPUT:		u8 DeviceAddress,u8 MemoryAddress		****/
/*RETURN:		u8										****/
/***********************************************************/
u8 I2C_ReadByte(u8 DeviceAddress,u8 MemoryAddress);

void I2C_Configuration(void);

#endif

