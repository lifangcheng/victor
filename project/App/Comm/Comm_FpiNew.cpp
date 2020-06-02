//==================================================================================
//| 文件名称 | 通信实现函数
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
#include "Comm_FpiNew.h"
#include "Comm_FpiUser.h"
#include "stm32_Bsp.h"


//==================端点定义==================
CFpiNew m_FpiUart3(COM_SCI3);   //端口2与PC通信
CFpiNew m_FpiUart6(COM_SCI6);   //端口2与PC通信



//CFpiNew m_FpiUart2(COM_SCI2);
//==================平台相关函数==================
extern void USART6_Puts(u8* str,u16 len);

//==================================================================================================
//| 函数名称 | CFifo()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 构造函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | zhliu,2016.03.31
//==================================================================================================
CFpiNew::CFpiNew(INT8U uch_Com)
{
    b_Enable = TRUE;//默认模块使能开启
    
    much_ComType = uch_Com;
    
    //状态初始化
    uch_comThrowFlag = 0;
    st_com_buff.uch_status = COM_IDLE;
    st_com_buff.uin_rec_bytes = 0;  
}
//==================================================================================
//| 函数名称 | Comm_Init()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 通信初始化
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//================================================================================== 
void CFpiNew::Comm_Init(void)
{
    st_com_buff.uch_status  = COM_IDLE;
    st_com_buff.uin_rec_bytes   = 0;    
}
//==================================================================================
//| 函数名称 | Comm_MakePack()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 组通信包
//|----------|----------------------------------------------------------------------
//| 输入参数 | p_com 端口指针 p_frame 缓冲区指针
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//================================================================================== 
void CFpiNew::Comm_MakePack(SR_FRAME *p_frame)
{
    u16 k;
    u16 uin_Crc;
    SR_COM *p_com;
    
    p_com = &st_com_buff;
    
    k = 0;      
    //帧头
    p_com->uch_buf[k++] = 0x7d;
	p_com->uch_buf[k++] = 0x7b;
	//保留字节
    p_com->uch_buf[k++] = p_frame->uch_resv[0];
	p_com->uch_buf[k++] = p_frame->uch_resv[1];
	p_com->uch_buf[k++] = p_frame->uch_resv[2];
	p_com->uch_buf[k++] = p_frame->uch_resv[3];

    //命令码
	p_com->uch_buf[k++] = p_frame->uch_cmd;
    //命令扩展码
    p_com->uch_buf[k++] = p_frame->uch_excmd;
    //数据长度
    EncodeUint(p_frame->uin_len,p_com->uch_buf+k);
    k += 2;
    //数据区
    CopyFromAToB(p_frame->uch_data,p_com->uch_buf+k,p_frame->uin_len);
    k += p_frame->uin_len;
    //Crc校验
    uin_Crc = GetCrc16Bit(p_com->uch_buf+2,k-2);
    EncodeUint(uin_Crc,p_com->uch_buf+k);
    k += 2;
    //帧尾
    p_com->uch_buf[k++] = 0x7D;
	p_com->uch_buf[k++] = 0x7D;
    
    p_com->uin_tran_bytes = k;
     
}
//==================================================================================
//| 函数名称 | Comm_DecPack()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 解通信包
//|----------|----------------------------------------------------------------------
//| 输入参数 | p_com 端口指针p_frame 缓冲区指针 
//|----------|----------------------------------------------------------------------
//| 返回参数 | 是否解包成功
//==================================================================================
char CFpiNew::Comm_DecPack(SR_FRAME *p_frame)
{
    u16 k;
    u16 uin_Crc1,uin_Crc2;   
    SR_COM *p_com;
    
    p_com = &st_com_buff;

    k = 0;
	//长度判断
	if(p_com->uin_rec_bytes <= 4)
	{
	    return 0;
	}

    //Crc校验
    uin_Crc1 = GetCrc16Bit(p_com->uch_buf+2,p_com->uin_rec_bytes-6);    
    uin_Crc2 = DecodeUint(p_com->uch_buf + p_com->uin_rec_bytes-4);
    if(uin_Crc1 != uin_Crc2)
    {
        return 0;
    }    

    //帧头
    //p_frame->uch_head = p_com->uch_buf[k++]; 
	k++;
	k++;
	//保留字节
	p_frame->uch_resv[0] = p_com->uch_buf[k++];
	p_frame->uch_resv[1] = p_com->uch_buf[k++];//地址
	p_frame->uch_resv[2] = p_com->uch_buf[k++];
	p_frame->uch_resv[3] = p_com->uch_buf[k++];
	
	//地址校验
	//if(p_frame->uch_resv[1] != 0x10 
        //&& p_frame->uch_resv[1] != gst_SysPara.uch_Adr
        //&& p_frame->uch_resv[1] != 0x12
        //&& p_frame->uch_resv[1] != 0xf2)
	//{
	//	return 0;
	//}
	
	//命令码
    p_frame->uch_cmd = p_com->uch_buf[k++];
    //命令类型
    p_frame->uch_excmd = p_com->uch_buf[k++]; 

	//路径列表
	//CopyFromAToB(p_frame->uch_resv,p_frame->uch_rout,p_frame->uch_sumrout);

    //数据区长度
    p_frame->uin_len = DecodeUint(p_com->uch_buf +k);
    k += 2;
    //数据区
    CopyFromAToB(p_com->uch_buf+k,p_frame->uch_data,p_frame->uin_len);
    k += p_frame->uin_len;
    //Crc校验
    p_frame->uin_crc = DecodeUint(p_com->uch_buf +k);
    k += 2;
    //帧尾
    //p_frame->uch_tail = p_com->uch_buf[k++]; 
    
    return 1;
}
//==================================================================================
//| 函数名称 | Comm_DealPack()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 处理接收包
//|----------|----------------------------------------------------------------------
//| 输入参数 | pRev接收包指针 pTrans 发送包指针 
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
void CFpiNew::Comm_DealPack(void)
{
  //  SR_FIFO *pRevFifo;
 //   SR_FIFO *pTransFifo;

	//临时通信缓冲区
    SR_FRAME st_rcvpack;
    SR_FRAME st_tranpack;

    //如果未收满一帧,直接返回
    if(Comm_GetFrameFromFifo() != TRUE)
    {
        return;
    }
    //帧检查,并解包
    if(!Comm_DecPack(&st_rcvpack))
    {
        st_com_buff.uin_rec_bytes = 0;
        st_com_buff.uch_status = COM_IDLE;
        return;
    }
    //处理通信帧
	if(st_rcvpack.uch_excmd == CMD_READ || 
	   st_rcvpack.uch_excmd == CMD_WRITE || 
	   st_rcvpack.uch_excmd == 0x77 || 
	   st_rcvpack.uch_excmd == 0x88)
	{
           if(st_rcvpack.uch_resv[1] == 0x10)//地址匹配
           {
               Comm_Slavepack(&st_rcvpack,&st_tranpack);
           }
           //光散射转发特殊处理 只针对串口4
           else if((st_rcvpack.uch_resv[1] == 0x12
           ||st_rcvpack.uch_resv[1] == 0x11)
           && much_ComType == COM_SCI4)//CAN地址匹配 CAN转发特殊处理，目前该架构
           {
               //组帧
               Comm_MakePack(&st_rcvpack);//将接收转发
               //通过CAN发送
               Comm_SendFrame(COM_CAN);
               st_com_buff.uin_rec_bytes = 0;
               st_com_buff.uch_status = COM_IDLE;
               return;
           }
           else
           {
               st_com_buff.uin_rec_bytes = 0;
               st_com_buff.uch_status = COM_IDLE;
               return;
           }
	}
        else if(st_rcvpack.uch_excmd == CMD_R_ANSWER || 
	   st_rcvpack.uch_excmd == CMD_W_ANSWER || 
	   st_rcvpack.uch_excmd == 0xCC || 
	   st_rcvpack.uch_excmd == 0xDD)
	{
           if(st_rcvpack.uch_resv[1] == 0x10)//地址匹配
           {
               Comm_Masterpack(&st_rcvpack,&st_tranpack);
               st_com_buff.uin_rec_bytes = 0;
               st_com_buff.uch_status = COM_IDLE;
               return;
           }
           //光散射转发特殊处理
           else if((st_rcvpack.uch_resv[3] == 0x12 || st_rcvpack.uch_resv[3] == 0x11)
              && much_ComType == COM_CAN)//有光散射发送的CAN地址匹配 CAN转发特殊处理
           {
               //Comm_Tranpack(&st_rcvpack,&st_tranpack);//转发组帧
               //组帧
               Comm_MakePack(&st_rcvpack);
               //通过CAN发送
               Comm_SendFrame(COM_SCI4);
               st_com_buff.uin_rec_bytes = 0;
               st_com_buff.uch_status = COM_IDLE;
               return;
           }
           else
           {
               st_com_buff.uin_rec_bytes = 0;
               st_com_buff.uch_status = COM_IDLE;
               return;
           }
	}
	else
	{
	    st_com_buff.uin_rec_bytes = 0;
		st_com_buff.uch_status = COM_IDLE;
		return;
	}
    //发送回复帧
    if(st_tranpack.uin_len > 0)
    {
        //处理路径
		st_tranpack.uch_resv[0] = st_rcvpack.uch_resv[2];
		st_tranpack.uch_resv[1] = st_rcvpack.uch_resv[3];
		st_tranpack.uch_resv[2] = st_rcvpack.uch_resv[0];
		st_tranpack.uch_resv[3] = st_rcvpack.uch_resv[1];
		//命令码拷贝
        st_tranpack.uch_cmd = st_rcvpack.uch_cmd;
        //命令扩展码交换
		if(st_tranpack.uch_excmd != 0xDD && st_tranpack.uch_excmd != 0xCC)
        st_tranpack.uch_excmd = 0xFF - st_rcvpack.uch_excmd;
        //组帧
        Comm_MakePack(&st_tranpack);
        //发送
        Comm_SendFrame(much_ComType);
    }
    //临时通信缓冲区清零
    st_rcvpack.uin_len    = 0;
    st_tranpack.uin_len   = 0;
	st_tranpack.uch_excmd = 0;
    //端口状态清零
    st_com_buff.uin_rec_bytes   = 0;
    st_com_buff.uch_status      = COM_IDLE;
   
}
//==================================================================================
//| 函数名称 | Comm_RcvNByte()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 接收N个字节
//|----------|----------------------------------------------------------------------
//| 输入参数 | uch_com 端口号, p_data 数据, uin_num 数据长度
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
/**
void Comm_RcvNByte(u8 uch_com,u8 *p_data,u16 uin_num)
{
	 static u8 uch_RevThrowFlag=0;
     SR_COM *st_Temp;
     u8 uch_Temp;
     u16 i;
 
	 
     if(uch_com == COM_SCI6)
     {
        st_Temp = &st_uart_rev;
     }
	 else if(uch_com == COM_CAN)
     {
        st_Temp = &st_can_rev;
     }
	 else if(uch_com == COM_SCI4)
     {
        st_Temp = &st_uart4_rev;
     }
	 else if(uch_com == COM_SCI1)
     {
        st_Temp = &st_uart1_rev;
     }
	 else if(uch_com == COM_SCI2)
     {
        st_Temp = &st_uart2_rev;
     }
	 else
	 {
	    return;
	 }
     //如果接收完未处理,则不继续接收
     if(st_Temp->uch_status == RECIEVED)
     {
         return;
     }
     //循环处理数据
     for(i=0; i<uin_num; i++)
     {
		uch_Temp = *p_data++;
        st_Temp->uch_buf[st_Temp->uin_rec_bytes++] = uch_Temp;
		
		//标志位判断		
		if(st_Temp->uin_rec_bytes == 1)
		{
		    if(uch_Temp != 0x7d)
		    {
		        st_Temp->uin_rec_bytes = 0;
		    }
		}
		else if(st_Temp->uin_rec_bytes == 2)
		{
		    if(uch_Temp != 0x7b)
		    {
		        st_Temp->uin_rec_bytes=0;
		    }
		}
		else if(st_Temp->uch_buf[st_Temp->uin_rec_bytes-2] == 0x7d && uch_RevThrowFlag == 0) //大于2的情况
		{
		    if(uch_Temp == 0x7d)
		    {
		        st_Temp->uch_status = RECIEVED;
		    }
        	else if(uch_Temp == 0x82)
            {
        	     //丢掉数据0x82
        	     st_Temp->uin_rec_bytes--;  //..
        	     uch_RevThrowFlag = 1;
        	     return;
            }
            else if(uch_Temp == 0x7b)//数据帧重新开始
            {
                st_Temp->uin_rec_bytes=0;
            }	
		}
		
		//正接收后的操作
		uch_RevThrowFlag = 0; //uch_RevThrowFlag清零 
		if(st_Temp->uin_rec_bytes >= BUF_MAX && st_Temp->uch_status != RECIEVED) //溢出检查
		{
		     st_Temp->uin_rec_bytes=0;
		}
     }    
}
**/
//==================================================================================
//| 函数名称 | Comm_GetFrameFromFifo()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 接收N个字节
//|----------|----------------------------------------------------------------------
//| 输入参数 | uch_com 端口号, p_data 数据, uin_num 数据长度
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
BOOL CFpiNew::Comm_GetFrameFromFifo(void)
{
	SR_COM *st_Temp;
    u8 uch_Temp;

	//if (pfifo == NULL || pCom == NULL)
	//{
	//	return FALSE;
	//}
	
	st_Temp = &st_com_buff;
	
	//如果接收完未处理,则不继续接收
	if(st_Temp->uch_status == RECIEVED)
	{
		return TRUE;
	}

    CLI();
	while (FifoPopRcvFifo(&uch_Temp))
	{		
        SEI();
		st_Temp->uch_buf[st_Temp->uin_rec_bytes++] = uch_Temp;
		
		//标志位判断		
		if(st_Temp->uin_rec_bytes == 1)
		{
			if(uch_Temp != 0x7d)
			{
				st_Temp->uin_rec_bytes = 0;
			}
			uch_comThrowFlag = 0; 
		}
		else if(st_Temp->uin_rec_bytes == 2)
		{
			if(uch_Temp != 0x7b)
			{
				st_Temp->uin_rec_bytes=0;
			}
			uch_comThrowFlag = 0; 
		}
		else if(st_Temp->uch_buf[st_Temp->uin_rec_bytes-2] == 0x7d && uch_comThrowFlag == 0) //大于2的情况
		{
			if(uch_Temp == 0x7d)
			{
				st_Temp->uch_status = RECIEVED;
				uch_comThrowFlag = 0; 
				return TRUE;  
			}
			else if(uch_Temp == 0x82)
			{
				 //丢掉数据0x82
				 st_Temp->uin_rec_bytes--;  //..
				 uch_comThrowFlag = 1;
			}
			else if(uch_Temp == 0x7b)//数据帧重新开始
			{
				st_Temp->uin_rec_bytes=0;
				uch_comThrowFlag = 0; 
			}	
		}
        else
        {
            uch_comThrowFlag = 0;
        }
		
		//正接收后的操作		
		if(st_Temp->uin_rec_bytes >= BUF_MAX && st_Temp->uch_status != RECIEVED) //溢出检查
		{
			 st_Temp->uin_rec_bytes=0;
		}
        CLI();
   
	}     
    SEI();

	return FALSE;  
}

void CFpiNew::Comm_PushFpiToFifo(SR_FIFO *p)
{
	INT16U uin_Index;
	INT16U uin_Size;

	INT8U tmp;
	INT8U uch_LastChar = 0;

	uin_Index = 0;
	uin_Size = st_com_buff.uin_tran_bytes;

	while (uin_Index < uin_Size)
	{
		if (uin_Index >2 && uin_Index<uin_Size-1
			&&uch_LastChar == 0x7d)//数据中有0x7d,后跟0x82
		{
			uch_LastChar = 0x82;
			tmp = 0x82;
		}
		else
		{
			uch_LastChar = st_com_buff.uch_buf[uin_Index++];
			tmp = uch_LastChar;
		}
        CLI();
		FifoPush(tmp, p);  
        SEI();
	}
}


void CFpiNew::Comm_PushFpiBuffToFifo(INT8U *p,INT8U len)
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
			&&uch_LastChar == 0x7d)//数据中有0x7d,后跟0x82
		{
			uch_LastChar = 0x82;
			tmp = 0x82;
		}
		else
		{
			uch_LastChar = p[uin_Index++];
			tmp = uch_LastChar;
		}
        CLI();
		FifoPush(tmp, &st_TrnFifo);  
        SEI();
	}
}
//==================================================================================
//| 函数名称 | Comm_SendFrame()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 发送一帧数据
//|----------|----------------------------------------------------------------------
//| 输入参数 | p 发送端口
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
void CFpiNew::Comm_SendFrame(INT8U uch_ComType)
{  
    SR_FIFO *p;
    
    p = GetFifo(uch_ComType);
    
	Comm_PushFpiToFifo(p);
     if(COM_SCI3 == uch_ComType)
     {
        //CAN发送函数
        //USART4_Pack_Send(p->uch_buf,p->uin_tran_bytes);
        USART_ITConfig(USART3,USART_IT_TXE,ENABLE);
     } 
     else if(COM_SCI6 == uch_ComType)
     {
        //CAN发送函数
        //USART4_Pack_Send(p->uch_buf,p->uin_tran_bytes);
        USART_ITConfig(USART6,USART_IT_TXE,ENABLE);
     }
#if 0
	 else if(COM_CAN == uch_ComType)
     {
        //CAN发送函数
        //Can0_PackSend(p->uch_buf,p->uin_tran_bytes);
        Can0_PackIrqSend();
     } 
      else if(COM_SCI5 == uch_ComType)
     {
        //CAN发送函数
        //USART4_Pack_Send(p->uch_buf,p->uin_tran_bytes);
        USART_ITConfig(UART5,USART_IT_TXE,ENABLE);
     }
#endif
}

SR_FIFO * CFpiNew::GetFifo(INT8U uch_ComType)
{    
    SR_FIFO *p;    
    switch(uch_ComType)
    {
    case COM_SCI3:
        p = &m_FpiUart3.st_TrnFifo;
        break;
    case COM_SCI6:
        p = &m_FpiUart6.st_TrnFifo;
        break;
#if 0
	case COM_SCI5:
        p = &m_FpiUart5.st_TrnFifo;
        break;
    case COM_CAN:
        p = &m_FpiCan.st_TrnFifo;
        break;
#endif
    default:
        p = NULL;
        break;
    }    
    return p;
}
//==================================================================================
//| 函数名称 | Comm_Precess()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 通信流程,需要定时调用
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
void Comm_FpiNew_Precess(void)
{
    //Comm_DealPack(&st_uart_rev,&st_uart_tran);
    m_FpiUart3.Comm_DealPack();
	m_FpiUart6.Comm_DealPack();
//    m_FpiUart5.Comm_DealPack();
//	m_FpiCan.Comm_DealPack();
	//Comm_DealPack(&st_uart1_rev,&st_uart1_tran);
	//m_FpiUart2.Comm_DealPack();
}
