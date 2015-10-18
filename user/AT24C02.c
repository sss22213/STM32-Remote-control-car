	#include "AT24C02.h"

	void AT24C02_WriteByte(u8 MemoryAddress,u8 data)
	{
	
		 I2C_WriteByte(0xA0,MemoryAddress,data);
	
	}

	u8	AT24C02_ReadByte(u8 MemoryAddress)
	{
	
		 return I2C_ReadByte(0xA0,MemoryAddress);
	
	}
	void AT24C02_Init(void)
	{
		int i=0;
		for(i=0;i<255;i++)
		{
		  AT24C02_WriteByte(i,0x08);
		  Delay_Ms(50);
		
		}
	
	}
