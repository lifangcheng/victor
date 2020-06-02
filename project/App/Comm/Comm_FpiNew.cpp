//==================================================================================
//| �ļ����� | ͨ��ʵ�ֺ���
//|----------|----------------------------------------------------------------------
//| �ļ����� | 
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------      
//| ���ز��� | ��
//==================================================================================
#include "Comm_FpiNew.h"
#include "Comm_FpiUser.h"
#include "stm32_Bsp.h"


//==================�˵㶨��==================
CFpiNew m_FpiUart3(COM_SCI3);   //�˿�2��PCͨ��
CFpiNew m_FpiUart6(COM_SCI6);   //�˿�2��PCͨ��



//CFpiNew m_FpiUart2(COM_SCI2);
//==================ƽ̨��غ���==================
extern void USART6_Puts(u8* str,u16 len);

//==================================================================================================
//| �������� | CFifo()
//|----------|--------------------------------------------------------------------------------------
//| �������� | ���캯��
//|----------|--------------------------------------------------------------------------------------
//| ������� | ��
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | zhliu,2016.03.31
//==================================================================================================
CFpiNew::CFpiNew(INT8U uch_Com)
{
    b_Enable = TRUE;//Ĭ��ģ��ʹ�ܿ���
    
    much_ComType = uch_Com;
    
    //״̬��ʼ��
    uch_comThrowFlag = 0;
    st_com_buff.uch_status = COM_IDLE;
    st_com_buff.uin_rec_bytes = 0;  
}
//==================================================================================
//| �������� | Comm_Init()
//|----------|----------------------------------------------------------------------
//| �������� | ͨ�ų�ʼ��
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------      
//| ���ز��� | ��
//================================================================================== 
void CFpiNew::Comm_Init(void)
{
    st_com_buff.uch_status  = COM_IDLE;
    st_com_buff.uin_rec_bytes   = 0;    
}
//==================================================================================
//| �������� | Comm_MakePack()
//|----------|----------------------------------------------------------------------
//| �������� | ��ͨ�Ű�
//|----------|----------------------------------------------------------------------
//| ������� | p_com �˿�ָ�� p_frame ������ָ��
//|----------|----------------------------------------------------------------------      
//| ���ز��� | ��
//================================================================================== 
void CFpiNew::Comm_MakePack(SR_FRAME *p_frame)
{
    u16 k;
    u16 uin_Crc;
    SR_COM *p_com;
    
    p_com = &st_com_buff;
    
    k = 0;      
    //֡ͷ
    p_com->uch_buf[k++] = 0x7d;
	p_com->uch_buf[k++] = 0x7b;
	//�����ֽ�
    p_com->uch_buf[k++] = p_frame->uch_resv[0];
	p_com->uch_buf[k++] = p_frame->uch_resv[1];
	p_com->uch_buf[k++] = p_frame->uch_resv[2];
	p_com->uch_buf[k++] = p_frame->uch_resv[3];

    //������
	p_com->uch_buf[k++] = p_frame->uch_cmd;
    //������չ��
    p_com->uch_buf[k++] = p_frame->uch_excmd;
    //���ݳ���
    EncodeUint(p_frame->uin_len,p_com->uch_buf+k);
    k += 2;
    //������
    CopyFromAToB(p_frame->uch_data,p_com->uch_buf+k,p_frame->uin_len);
    k += p_frame->uin_len;
    //CrcУ��
    uin_Crc = GetCrc16Bit(p_com->uch_buf+2,k-2);
    EncodeUint(uin_Crc,p_com->uch_buf+k);
    k += 2;
    //֡β
    p_com->uch_buf[k++] = 0x7D;
	p_com->uch_buf[k++] = 0x7D;
    
    p_com->uin_tran_bytes = k;
     
}
//==================================================================================
//| �������� | Comm_DecPack()
//|----------|----------------------------------------------------------------------
//| �������� | ��ͨ�Ű�
//|----------|----------------------------------------------------------------------
//| ������� | p_com �˿�ָ��p_frame ������ָ�� 
//|----------|----------------------------------------------------------------------
//| ���ز��� | �Ƿ����ɹ�
//==================================================================================
char CFpiNew::Comm_DecPack(SR_FRAME *p_frame)
{
    u16 k;
    u16 uin_Crc1,uin_Crc2;   
    SR_COM *p_com;
    
    p_com = &st_com_buff;

    k = 0;
	//�����ж�
	if(p_com->uin_rec_bytes <= 4)
	{
	    return 0;
	}

    //CrcУ��
    uin_Crc1 = GetCrc16Bit(p_com->uch_buf+2,p_com->uin_rec_bytes-6);    
    uin_Crc2 = DecodeUint(p_com->uch_buf + p_com->uin_rec_bytes-4);
    if(uin_Crc1 != uin_Crc2)
    {
        return 0;
    }    

    //֡ͷ
    //p_frame->uch_head = p_com->uch_buf[k++]; 
	k++;
	k++;
	//�����ֽ�
	p_frame->uch_resv[0] = p_com->uch_buf[k++];
	p_frame->uch_resv[1] = p_com->uch_buf[k++];//��ַ
	p_frame->uch_resv[2] = p_com->uch_buf[k++];
	p_frame->uch_resv[3] = p_com->uch_buf[k++];
	
	//��ַУ��
	//if(p_frame->uch_resv[1] != 0x10 
        //&& p_frame->uch_resv[1] != gst_SysPara.uch_Adr
        //&& p_frame->uch_resv[1] != 0x12
        //&& p_frame->uch_resv[1] != 0xf2)
	//{
	//	return 0;
	//}
	
	//������
    p_frame->uch_cmd = p_com->uch_buf[k++];
    //��������
    p_frame->uch_excmd = p_com->uch_buf[k++]; 

	//·���б�
	//CopyFromAToB(p_frame->uch_resv,p_frame->uch_rout,p_frame->uch_sumrout);

    //����������
    p_frame->uin_len = DecodeUint(p_com->uch_buf +k);
    k += 2;
    //������
    CopyFromAToB(p_com->uch_buf+k,p_frame->uch_data,p_frame->uin_len);
    k += p_frame->uin_len;
    //CrcУ��
    p_frame->uin_crc = DecodeUint(p_com->uch_buf +k);
    k += 2;
    //֡β
    //p_frame->uch_tail = p_com->uch_buf[k++]; 
    
    return 1;
}
//==================================================================================
//| �������� | Comm_DealPack()
//|----------|----------------------------------------------------------------------
//| �������� | ������հ�
//|----------|----------------------------------------------------------------------
//| ������� | pRev���հ�ָ�� pTrans ���Ͱ�ָ�� 
//|----------|----------------------------------------------------------------------      
//| ���ز��� | ��
//==================================================================================
void CFpiNew::Comm_DealPack(void)
{
  //  SR_FIFO *pRevFifo;
 //   SR_FIFO *pTransFifo;

	//��ʱͨ�Ż�����
    SR_FRAME st_rcvpack;
    SR_FRAME st_tranpack;

    //���δ����һ֡,ֱ�ӷ���
    if(Comm_GetFrameFromFifo() != TRUE)
    {
        return;
    }
    //֡���,�����
    if(!Comm_DecPack(&st_rcvpack))
    {
        st_com_buff.uin_rec_bytes = 0;
        st_com_buff.uch_status = COM_IDLE;
        return;
    }
    //����ͨ��֡
	if(st_rcvpack.uch_excmd == CMD_READ || 
	   st_rcvpack.uch_excmd == CMD_WRITE || 
	   st_rcvpack.uch_excmd == 0x77 || 
	   st_rcvpack.uch_excmd == 0x88)
	{
           if(st_rcvpack.uch_resv[1] == 0x10)//��ַƥ��
           {
               Comm_Slavepack(&st_rcvpack,&st_tranpack);
           }
           //��ɢ��ת�����⴦�� ֻ��Դ���4
           else if((st_rcvpack.uch_resv[1] == 0x12
           ||st_rcvpack.uch_resv[1] == 0x11)
           && much_ComType == COM_SCI4)//CAN��ַƥ�� CANת�����⴦��Ŀǰ�üܹ�
           {
               //��֡
               Comm_MakePack(&st_rcvpack);//������ת��
               //ͨ��CAN����
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
           if(st_rcvpack.uch_resv[1] == 0x10)//��ַƥ��
           {
               Comm_Masterpack(&st_rcvpack,&st_tranpack);
               st_com_buff.uin_rec_bytes = 0;
               st_com_buff.uch_status = COM_IDLE;
               return;
           }
           //��ɢ��ת�����⴦��
           else if((st_rcvpack.uch_resv[3] == 0x12 || st_rcvpack.uch_resv[3] == 0x11)
              && much_ComType == COM_CAN)//�й�ɢ�䷢�͵�CAN��ַƥ�� CANת�����⴦��
           {
               //Comm_Tranpack(&st_rcvpack,&st_tranpack);//ת����֡
               //��֡
               Comm_MakePack(&st_rcvpack);
               //ͨ��CAN����
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
    //���ͻظ�֡
    if(st_tranpack.uin_len > 0)
    {
        //����·��
		st_tranpack.uch_resv[0] = st_rcvpack.uch_resv[2];
		st_tranpack.uch_resv[1] = st_rcvpack.uch_resv[3];
		st_tranpack.uch_resv[2] = st_rcvpack.uch_resv[0];
		st_tranpack.uch_resv[3] = st_rcvpack.uch_resv[1];
		//�����뿽��
        st_tranpack.uch_cmd = st_rcvpack.uch_cmd;
        //������չ�뽻��
		if(st_tranpack.uch_excmd != 0xDD && st_tranpack.uch_excmd != 0xCC)
        st_tranpack.uch_excmd = 0xFF - st_rcvpack.uch_excmd;
        //��֡
        Comm_MakePack(&st_tranpack);
        //����
        Comm_SendFrame(much_ComType);
    }
    //��ʱͨ�Ż���������
    st_rcvpack.uin_len    = 0;
    st_tranpack.uin_len   = 0;
	st_tranpack.uch_excmd = 0;
    //�˿�״̬����
    st_com_buff.uin_rec_bytes   = 0;
    st_com_buff.uch_status      = COM_IDLE;
   
}
//==================================================================================
//| �������� | Comm_RcvNByte()
//|----------|----------------------------------------------------------------------
//| �������� | ����N���ֽ�
//|----------|----------------------------------------------------------------------
//| ������� | uch_com �˿ں�, p_data ����, uin_num ���ݳ���
//|----------|----------------------------------------------------------------------      
//| ���ز��� | ��
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
     //���������δ����,�򲻼�������
     if(st_Temp->uch_status == RECIEVED)
     {
         return;
     }
     //ѭ����������
     for(i=0; i<uin_num; i++)
     {
		uch_Temp = *p_data++;
        st_Temp->uch_buf[st_Temp->uin_rec_bytes++] = uch_Temp;
		
		//��־λ�ж�		
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
		else if(st_Temp->uch_buf[st_Temp->uin_rec_bytes-2] == 0x7d && uch_RevThrowFlag == 0) //����2�����
		{
		    if(uch_Temp == 0x7d)
		    {
		        st_Temp->uch_status = RECIEVED;
		    }
        	else if(uch_Temp == 0x82)
            {
        	     //��������0x82
        	     st_Temp->uin_rec_bytes--;  //..
        	     uch_RevThrowFlag = 1;
        	     return;
            }
            else if(uch_Temp == 0x7b)//����֡���¿�ʼ
            {
                st_Temp->uin_rec_bytes=0;
            }	
		}
		
		//�����պ�Ĳ���
		uch_RevThrowFlag = 0; //uch_RevThrowFlag���� 
		if(st_Temp->uin_rec_bytes >= BUF_MAX && st_Temp->uch_status != RECIEVED) //������
		{
		     st_Temp->uin_rec_bytes=0;
		}
     }    
}
**/
//==================================================================================
//| �������� | Comm_GetFrameFromFifo()
//|----------|----------------------------------------------------------------------
//| �������� | ����N���ֽ�
//|----------|----------------------------------------------------------------------
//| ������� | uch_com �˿ں�, p_data ����, uin_num ���ݳ���
//|----------|----------------------------------------------------------------------      
//| ���ز��� | ��
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
	
	//���������δ����,�򲻼�������
	if(st_Temp->uch_status == RECIEVED)
	{
		return TRUE;
	}

    CLI();
	while (FifoPopRcvFifo(&uch_Temp))
	{		
        SEI();
		st_Temp->uch_buf[st_Temp->uin_rec_bytes++] = uch_Temp;
		
		//��־λ�ж�		
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
		else if(st_Temp->uch_buf[st_Temp->uin_rec_bytes-2] == 0x7d && uch_comThrowFlag == 0) //����2�����
		{
			if(uch_Temp == 0x7d)
			{
				st_Temp->uch_status = RECIEVED;
				uch_comThrowFlag = 0; 
				return TRUE;  
			}
			else if(uch_Temp == 0x82)
			{
				 //��������0x82
				 st_Temp->uin_rec_bytes--;  //..
				 uch_comThrowFlag = 1;
			}
			else if(uch_Temp == 0x7b)//����֡���¿�ʼ
			{
				st_Temp->uin_rec_bytes=0;
				uch_comThrowFlag = 0; 
			}	
		}
        else
        {
            uch_comThrowFlag = 0;
        }
		
		//�����պ�Ĳ���		
		if(st_Temp->uin_rec_bytes >= BUF_MAX && st_Temp->uch_status != RECIEVED) //������
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
			&&uch_LastChar == 0x7d)//��������0x7d,���0x82
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
			&&uch_LastChar == 0x7d)//��������0x7d,���0x82
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
//| �������� | Comm_SendFrame()
//|----------|----------------------------------------------------------------------
//| �������� | ����һ֡����
//|----------|----------------------------------------------------------------------
//| ������� | p ���Ͷ˿�
//|----------|----------------------------------------------------------------------      
//| ���ز��� | ��
//==================================================================================
void CFpiNew::Comm_SendFrame(INT8U uch_ComType)
{  
    SR_FIFO *p;
    
    p = GetFifo(uch_ComType);
    
	Comm_PushFpiToFifo(p);
     if(COM_SCI3 == uch_ComType)
     {
        //CAN���ͺ���
        //USART4_Pack_Send(p->uch_buf,p->uin_tran_bytes);
        USART_ITConfig(USART3,USART_IT_TXE,ENABLE);
     } 
     else if(COM_SCI6 == uch_ComType)
     {
        //CAN���ͺ���
        //USART4_Pack_Send(p->uch_buf,p->uin_tran_bytes);
        USART_ITConfig(USART6,USART_IT_TXE,ENABLE);
     }
#if 0
	 else if(COM_CAN == uch_ComType)
     {
        //CAN���ͺ���
        //Can0_PackSend(p->uch_buf,p->uin_tran_bytes);
        Can0_PackIrqSend();
     } 
      else if(COM_SCI5 == uch_ComType)
     {
        //CAN���ͺ���
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
//| �������� | Comm_Precess()
//|----------|----------------------------------------------------------------------
//| �������� | ͨ������,��Ҫ��ʱ����
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------      
//| ���ز��� | ��
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
