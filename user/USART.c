	#include "USART.h"
	/*********************************************/
	/*NAME:			USART_Configurataion		**/
	/*INPUT:		NULL						**/
	/*RETURN:		NULL						**/
	/*9600,N81									**/
	/*********************************************/
	void USART_Configurataion(void)
	{
		//USART1_Init
		USART_InitTypeDef UART;


		UART.USART_BaudRate=9600;

		UART.USART_WordLength=USART_WordLength_8b;

		UART.USART_StopBits=USART_StopBits_1;
  		
		UART.USART_Parity=USART_Parity_No;
  		
		UART.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;

		UART.USART_HardwareFlowControl=USART_HardwareFlowControl_None;

  		USART_Init(USART1,&UART);

		USART_Cmd(USART1,ENABLE);

		//USART3_Init

		UART.USART_BaudRate=9600;

		UART.USART_WordLength=USART_WordLength_8b;

		UART.USART_StopBits=USART_StopBits_1;
  		
		UART.USART_Parity=USART_Parity_No;
  		
		UART.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;

		UART.USART_HardwareFlowControl=USART_HardwareFlowControl_None;

		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);

  		USART_Init(USART3,&UART);

		USART_Cmd(USART3,ENABLE);
			
	}
	void USART_Sendstring(USART_TypeDef* USARTx,char* data)
	{
		int i=0;
		while(*(data+i)!='\0')
		{
			USART_SendData(USARTx,*(data+i));
			while (USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);
			i++;
				
		}
			
	}
	char* USART_Readstring(USART_TypeDef* USARTx,int String_Length)
	{
		static char data[50];
		int i=0;
		
		for(i=0;i<String_Length;i++)
		{
			while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE));
			data[i]=USART_ReceiveData(USARTx);
		}
		i++;
		data[i]='\0';
		return data;	
	}

