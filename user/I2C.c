	#include "I2C.h"
	void I2C_Configuration(void)
	{
		
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
	}
	/*************************************************************/
	/*NAME:			I2C_WriteByte								**/
	/*INPUT:		u8 DeviceAddress,u8 MemoryAddress,u8 data	**/
	/*RETURN:		NULL										**/
	/*************************************************************/
	void I2C_WriteByte(u8 DeviceAddress,u8 MemoryAddress,u8 data)
	{
		
		//等待I2C閒置
		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
		
		//啟動訊號
		I2C_GenerateSTART(I2C1,ENABLE);	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);

		//發送裝置位置
		I2C_Send7bitAddress(I2C1,DeviceAddress,I2C_Direction_Transmitter);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);


		//發送記憶體位置
		I2C_SendData(I2C1,MemoryAddress);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);

		//發送數據
		I2C_SendData(I2C1,data);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);

		//停止訊號
		I2C_GenerateSTOP(I2C1,ENABLE);

		 /* Clear AF flag */
        I2C_ClearFlag(I2C1, I2C_FLAG_AF);
        /* STOP condition */    
        I2C_GenerateSTOP(I2C1, ENABLE);  
	}
	/*************************************************************/
	/*NAME:			I2C_ReadByte								**/
	/*INPUT:		u8 DeviceAddress,u8 MemoryAddress			**/
	/*RETURN:		u8											**/
	/*************************************************************/
	u8 I2C_ReadByte(u8 DeviceAddress,u8 MemoryAddress)
	{
		
		u8 data;

		//等待I2C閒置
		while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
		
		//啟動訊號
		I2C_GenerateSTART(I2C1,ENABLE);	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);

		//發送裝置位置
		I2C_Send7bitAddress(I2C1,DeviceAddress,I2C_Direction_Transmitter);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
		I2C_Cmd(I2C1,ENABLE);

		//發送記憶體位置
		I2C_SendData(I2C1,MemoryAddress);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);

		//啟動訊號
		I2C_GenerateSTART(I2C1,ENABLE);	
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
		
		//發送裝置位置
		I2C_Send7bitAddress(I2C1,DeviceAddress,I2C_Direction_Receiver);
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);
		
		I2C_AcknowledgeConfig(I2C1,DISABLE); //ACK 關閉	
		
		//停止訊號
		I2C_GenerateSTOP(I2C1,ENABLE);

		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);

		data=I2C_ReceiveData(I2C1);			//接收資料
		
		I2C_AcknowledgeConfig(I2C1,ENABLE); 	
	
		return data;		
	
	}

