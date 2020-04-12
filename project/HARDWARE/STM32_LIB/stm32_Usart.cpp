#include "stm32f4xx.h"
#include "stm32_Bsp.h"
#include <stdio.h>


#if 1
#pragma import(__use_no_semihosting)             
//±ê×??aDèòaμ??§3?oˉêy                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//?¨ò?_sys_exit()ò?±ü?aê1ó?°??÷?ú?￡ê?    
int _sys_exit(int x) 
{ 
	x = x; 
} 
//???¨ò?fputcoˉêy 
int fputc(int ch, FILE *f)
{ 	
	while((USART6->SR&0X40)==0);//?-?··￠?í,?±μ?·￠?ííê±?   
	USART6->DR = (u8) ch;      
	return ch;
}
#endif



//==================================================================================================
//| 函数名称 | Bsp_UartInit()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | Uart初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void Bsp_UartInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	//----------USART3 config -----//    有用 的
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
  	
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);  
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	
	GPIO_StructInit(&GPIO_InitStructure);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);   

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	USART_StructInit(&USART_InitStructure);
  	USART_InitStructure.USART_BaudRate = 57600;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART3, &USART_InitStructure);
	
	USART_ClockStructInit(&USART_ClockInitStruct);
	USART_ClockInit(USART3,&USART_ClockInitStruct);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
//    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
  	USART_Cmd(USART3, ENABLE);
  	//USART_ClearFlag(USART3, USART_FLAG_TC);
	
	//----------USART6 config -----//
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
  	
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);  
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	
	GPIO_StructInit(&GPIO_InitStructure);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);   

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	USART_StructInit(&USART_InitStructure);
  	USART_InitStructure.USART_BaudRate = 57600;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART6, &USART_InitStructure);
	
	USART_ClockStructInit(&USART_ClockInitStruct);
	USART_ClockInit(USART6,&USART_ClockInitStruct);

	USART_ITConfig(USART6, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
  	USART_Cmd(USART6, ENABLE);
        
}

void Uart2_Init(u8 uch_baud)
{
	INT32U ul_baud;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	if(uch_baud == 1)
	{
		ul_baud = 4800;
	}
	else if(uch_baud == 2)
	{
		ul_baud = 9600;
	}
	else if(uch_baud == 3)
	{
		ul_baud = 19200;
	}
	else if(uch_baud == 4)
	{
		ul_baud = 57600;
	}else
	{
        ul_baud = 115200;
	}
	
	USART_StructInit(&USART_InitStructure);
  	USART_InitStructure.USART_BaudRate = ul_baud;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART2, &USART_InitStructure);
	
	USART_ClockStructInit(&USART_ClockInitStruct);
	USART_ClockInit(USART2,&USART_ClockInitStruct);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
  	USART_Cmd(USART2, ENABLE);
}

void Uart3_Init(u8 uch_baud)
{
	INT32U ul_baud;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	if(uch_baud == 1)
	{
		ul_baud = 4800;
	}
	else if(uch_baud == 2)
	{
		ul_baud = 9600;
	}
	else if(uch_baud == 3)
	{
		ul_baud = 19200;
	}
	else if(uch_baud == 4)
	{
		ul_baud = 57600;
	}else
	{
        ul_baud = 115200;
	}

	USART_StructInit(&USART_InitStructure);
  	USART_InitStructure.USART_BaudRate = ul_baud;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART3, &USART_InitStructure);
	
	USART_ClockStructInit(&USART_ClockInitStruct);
	USART_ClockInit(USART3,&USART_ClockInitStruct);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
  	USART_Cmd(USART3, ENABLE);
}

void Uart6_Init(u8 uch_baud)
{
	INT32U ul_baud;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	if(uch_baud == 1)
	{
		ul_baud = 4800;
	}
	else if(uch_baud == 2)
	{
		ul_baud = 9600;
	}
	else if(uch_baud == 3)
	{
		ul_baud = 19200;
	}
	else if(uch_baud == 4)
	{
		ul_baud = 57600;
	}else
	{
       ul_baud = 115200;
	}
	
	USART_StructInit(&USART_InitStructure);
  	USART_InitStructure.USART_BaudRate = ul_baud;//波特率设置
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART6, &USART_InitStructure);
	
	USART_ClockStructInit(&USART_ClockInitStruct);
	USART_ClockInit(USART6,&USART_ClockInitStruct);
	
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
  	USART_Cmd(USART6, ENABLE);
}

//==================================================================================================
//| 函数名称 | USART3_Puts
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 发送字符串
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void USART2_Puts(u8 * str,u16 len) 
{ 
    u16 i;
	for(i = 0;i< len ;i++) 
    { 
	  
	   USART_SendData(USART2, *str++); 
       while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 

    }
}

void USART3_Puts(u8 * str,u16 len) 
{ 
    u16 i;
	for(i = 0;i< len ;i++) 
    { 
	  
	   USART_SendData(USART3, *str++); 
       while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 

    }
}

//==================================================================================================
//| 函数名称 | USART6_Puts
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 发送字符串
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void USART6_Puts(u8 * str,u16 len) 
{ 
    u16 i;
	for(i = 0;i< len ;i++) 
    {
	  
	   USART_SendData(USART6, *str++); 
       while(USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET); 

    }
}


void USART2_Pack_Send(u8 * str,u16 len) 
{ 

	INT16U uin_Index;
	INT16U uin_Size;

	INT8U tmp;
	INT8U uch_LastChar = 0;

	uin_Index = 0;
	uin_Size = len;

	while (uin_Index < uin_Size)
	{
		if (uin_Index >2 && uin_Index<uin_Size-1
			&&uch_LastChar == 0x7d)//pst_Pack->uch_Data[uin_Index-1] == 0x7d)
		{

			uch_LastChar = 0x82;
			tmp = 0x82;
			//pst_Pack->uch_Data[uin_Index-1] = 0;
		}
		else
		{
			uch_LastChar = str[uin_Index++];
			tmp = uch_LastChar;//pst_Pack->uch_Data[uin_Index++];
		}
		USART_SendData(USART2, tmp); 
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
	}
}


void USART3_Pack_Send(u8 * str,u16 len) 
{ 

	INT16U uin_Index;
	INT16U uin_Size;

	INT8U tmp;
	INT8U uch_LastChar = 0;

	uin_Index = 0;
	uin_Size = len;

	while (uin_Index < uin_Size)
	{
		if (uin_Index >2 && uin_Index<uin_Size-1
			&&uch_LastChar == 0x7d)//pst_Pack->uch_Data[uin_Index-1] == 0x7d)
		{

			uch_LastChar = 0x82;
			tmp = 0x82;
			//pst_Pack->uch_Data[uin_Index-1] = 0;
		}
		else
		{
			uch_LastChar = str[uin_Index++];
			tmp = uch_LastChar;//pst_Pack->uch_Data[uin_Index++];
		}
		USART_SendData(USART3, tmp); 
        while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
	}
}

void USART6_Pack_Send(u8 * str,u16 len) 
{ 
	INT16U uin_Index;
	INT16U uin_Size;

	INT8U tmp;
	INT8U uch_LastChar = 0;

	uin_Index = 0;
	uin_Size = len;

	while (uin_Index < uin_Size)
	{
	  if (uin_Index >2 && uin_Index<uin_Size-1
			&&uch_LastChar == 0x7d)//pst_Pack->uch_Data[uin_Index-1] == 0x7d)
		{

			uch_LastChar = 0x82;
			tmp = 0x82;
			//pst_Pack->uch_Data[uin_Index-1] = 0;
		}
		else
		{
			uch_LastChar = str[uin_Index++];
			tmp = uch_LastChar;//pst_Pack->uch_Data[uin_Index++];
		}

		USART_SendData(USART6, tmp); 
        while(USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET); 
	}
}



//==================================================================================================
//| 函数名称 | USART6_IRQHandler()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | Usart6中断函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void USART6_IRQHandler(void)
{
  u8 temp; 
  if(USART_GetITStatus(USART6, USART_IT_RXNE) == SET) 
  { 
     temp = USART_ReceiveData(USART6);
     USART_ClearITPendingBit(USART6, USART_IT_RXNE); 
  }   
  if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
  {   	
	 USART_ClearITPendingBit(USART6, USART_IT_TXE); 
	 USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
  }
  return;
}


void USART2_IRQHandler(void)
{
  u8 temp; 
  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET) 
  { 
     temp = USART_ReceiveData(USART2);
     USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  }     
  return;
}
//UART3
void USART3_IRQHandler(void)
{
  u8 temp; 
  if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET) 
  { 
     temp = USART_ReceiveData(USART3);

     USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  }     
  if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
  {   	
	 USART_ClearITPendingBit(USART3, USART_IT_TXE); 
	 USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
  }
  return;
}
