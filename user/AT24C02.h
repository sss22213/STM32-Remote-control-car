#ifndef __AT24C02_H
#define __AT24C02_H
#include "stm32f10x_lib.h"
#include "I2C.h"
#include "Delay.h"


/***********************************************************/
/*NAME:			AT24C02_WriteByte						****/
/*INPUT:		u8 MemoryAddress,u8 data				***/
/*RETURN:		void									****/
/***********************************************************/
void AT24C02_WriteByte(u8 MemoryAddress,u8 data);

/***********************************************************/
/*NAME:			AT24C02_ReadByte						****/
/*INPUT:		u8 MemoryAddress						***/
/*RETURN:		u8										****/
/***********************************************************/
u8	AT24C02_ReadByte(u8 MemoryAddress);

/***********************************************************/
/*NAME:			AT24C02_Init							****/
/*INPUT:		void									***/
/*RETURN:		void									****/
/***********************************************************/
void AT24C02_Init(void);

#endif