#include "HTU21.h"

	u8 Temp_Read(void)
	{
		
		 u8 MSB,LSB;
		 
		//等待I2C閒置
		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
		I2C_AcknowledgeConfig(I2C1,ENABLE); 
		//啟動訊號
		I2C_GenerateSTART(I2C1,ENABLE);	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);

		//發送裝置位置
		I2C_Send7bitAddress(I2C1,0xF0,I2C_Direction_Transmitter);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
		I2C_Cmd(I2C1,ENABLE);

		//發送記憶體位置
		I2C_SendData(I2C1,0xE5);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);

		//啟動訊號
		I2C_GenerateSTART(I2C1,ENABLE);	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
		
		//發送裝置位置
		I2C_Send7bitAddress(I2C1,0xF0,I2C_Direction_Receiver);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);

		//I2C_AcknowledgeConfig(I2C1,DISABLE); //ACK 關閉	
		MSB=I2C_ReceiveData(I2C1);
	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);
		
		LSB=I2C_ReceiveData(I2C1);
	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);
		
		 	//停止訊號
		I2C_GenerateSTOP(I2C1,ENABLE);
			
		USART_SendData(USART1,MSB);
	
	}
