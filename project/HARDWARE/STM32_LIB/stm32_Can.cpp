#include "stm32f4xx.h"
#include "stm32_Can.h"
#include "stm32_Bsp.h"

#include "TypeDefine.h"

void Stm32_Can_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* CAN Periph clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);

	/* Configure CAN pin: TX RX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;//| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	// Connect TIM3 pins to AF2  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);

	/* CAN register init */
	//CAN_DeInit();
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_7tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler=8;
	CAN_Init(CAN2,&CAN_InitStructure);
	
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=15;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN2,CAN_IT_FMP0, ENABLE);
}


void CAN2_TX0_Send(void)
{
	CanTxMsg TxMessage;
	/* transmit 1 message */
	TxMessage.StdId=0x00;
	TxMessage.ExtId=0x1234;
	TxMessage.IDE=CAN_ID_EXT;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=8;
	TxMessage.Data[0]=0x7d;
	TxMessage.Data[1]=0x7b;
	TxMessage.Data[2]=0x01;
	TxMessage.Data[3]=0x11;
	TxMessage.Data[4]=0x01;
	TxMessage.Data[5]=0x16;
	TxMessage.Data[6]=0x35;
	TxMessage.Data[7]=0xaa;
	CAN_Transmit(CAN2,&TxMessage);
}

void CAN2_TX0_Send_Ext(u8 * str)
{
	CanTxMsg TxMessage;
	/* transmit 1 message */
	TxMessage.StdId=0x00;
	TxMessage.ExtId=0x1234;
	TxMessage.IDE=CAN_ID_EXT;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=8;
	TxMessage.Data[0]=str[0];
	TxMessage.Data[1]=str[1];
	TxMessage.Data[2]=str[2];
	TxMessage.Data[3]=str[3];
	TxMessage.Data[4]=str[4];
	TxMessage.Data[5]=str[5];
	TxMessage.Data[6]=str[6];
	TxMessage.Data[7]=str[7];
	CAN_Transmit(CAN2,&TxMessage);
}

void Can0_PackSend(u8 * str,u16 len)
{
	INT16U uin_Index;
	INT16U uin_Size;
	//INT8U dstAddr;
	//INT8U srcAddr;
	INT8U tmp[8];
	INT8U uch_LastChar = 0;
	INT8U i;


	uin_Index = 0;
	uin_Size = len;
	//dstAddr = pst_Pack->uch_DstAddr;
	//srcAddr = pst_Pack->uch_SrcAddr;

	while (uin_Index < uin_Size)
	{

		for (i=0; i<8; i++)
		{
			if (uin_Index < uin_Size)
			{
				if (uin_Index >2 && uin_Index<uin_Size-1
					&&uch_LastChar == 0x7d)//pst_Pack->uch_Data[uin_Index-1] == 0x7d)
				{

                    uch_LastChar = 0x82;
					tmp[i] = 0x82;
					//pst_Pack->uch_Data[uin_Index-1] = 0;
					
				} 
				else
				{
				    uch_LastChar = str[uin_Index++];
					tmp[i] = uch_LastChar;//pst_Pack->uch_Data[uin_Index++];
				}
			} 
			else
			{
				tmp[i] = 0x00;
			}
		}
		CAN2_TX0_Send_Ext(tmp);
		
		DelayNus(300);
	}
}


void Can0_PackIrqSend(void)
{
#if 0    
	INT8U uch_Tmp[8];
	INT8U i,temp;
	INT8U flag = 0;
   
	CLI();
	for(i=0;i<8;i++)
	{
          //CLI();
		if(m_FpiCan.FifoPopTrnFifo(&temp) == 1)
		{
            //SEI();
			uch_Tmp[i] = temp;
		}
		else
		{
            //SEI();
			uch_Tmp[i] = 0;
			flag++;
		}
	}
	SEI();
	if(flag>=8)
	{
		;
	}
	else
	{
          //DelayNus(300);
		CLI();
                CAN_ITConfig(CAN2,CAN_IT_TME, ENABLE);
		CAN2_TX0_Send_Ext(uch_Tmp);
                
                SEI();
	}
#endif
}


void CAN2_TX_IRQHandler(void)
{
#if 0
	INT8U uch_Tmp[8];
	INT8U i,temp;
	INT8U flag = 0;
	
	for(i=0;i<8;i++)
	{
		if(m_FpiCan.FifoPopTrnFifo(&temp) == 1)
		{
			uch_Tmp[i] = temp;
		}
		else
		{
			uch_Tmp[i] = 0;
			flag++;
		}
	}
	
	if(flag>=8)
	{
		CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
		CAN_ITConfig(CAN2,CAN_IT_TME, DISABLE);
	}
	else
	{
        CAN_ClearITPendingBit(CAN2,CAN_IT_TME);
		CAN2_TX0_Send_Ext(uch_Tmp);
	}
#endif
}


void CAN2_RX0_IRQHandler(void)
{
#if 0
	INT8 i;
	CanRxMsg RxMessage;
	
	RxMessage.StdId=0x00;
	RxMessage.ExtId=0x00;
	RxMessage.IDE=0;
	RxMessage.DLC=0;
	RxMessage.FMI=0;
	RxMessage.Data[0]=0x00;
	RxMessage.Data[1]=0x00;
	
	CAN_Receive(CAN2,CAN_FIFO0,&RxMessage);
	
	//if((RxMessage.ExtId==0x1234) && (RxMessage.IDE==CAN_ID_EXT)
	// && (RxMessage.DLC==2) && ((RxMessage.Data[1]|RxMessage.Data[0]<<8)==0xDECA))
	{
		//ret = 1; 
		
		//can数据接收		
		for(i=0;i<8;i++)
            m_FpiCan.FifoPushRcvFifo(RxMessage.Data[i]);
		//Comm_RcvNByte(COM_CAN,&RxMessage.Data[i],1);
	}
	//else
	//{
		//ret = 0; 
	//}
 #endif   
}
