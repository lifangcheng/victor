#include "Comm_Protocal.h"
#include "Comm_ProtocalUser.h"
#include "STM32_BSP.h"


//==================�˵㶨��==================
CProtocal m_Uart3(COM_SCI3);   //�˿�2��PCͨ��
CProtocal m_Uart6(COM_SCI6);   //�˿�2��PCͨ��

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
CProtocal::CProtocal(INT8U uch_Com)
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
void CProtocal::Comm_Init(void)
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
void CProtocal::Comm_MakePack(SR_FRAME *p_frame)
{
    u16 k;
    u16 uin_Crc;
    SR_COM *p_com;
    
    p_com = &st_com_buff;
    
    k = 0;      
    //֡ͷ
    p_com->uch_buf[k++] = 0x8A;
	p_com->uch_buf[k++] = 0x8B;
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
    p_com->uch_buf[k++] = 0x8B;
	p_com->uch_buf[k++] = 0x8B;
    
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
char CProtocal::Comm_DecPack(SR_FRAME *p_frame)
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
void CProtocal::Comm_DealPack(void)
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
//| �������� | Comm_GetFrameFromFifo()
//|----------|----------------------------------------------------------------------
//| �������� | ����N���ֽ�
//|----------|----------------------------------------------------------------------
//| ������� | uch_com �˿ں�, p_data ����, uin_num ���ݳ���
//|----------|----------------------------------------------------------------------      
//| ���ز��� | ��
//==================================================================================
BOOL CProtocal::Comm_GetFrameFromFifo(void)
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
			if(uch_Temp != 0x8A)
			{
				st_Temp->uin_rec_bytes = 0;
			}
			uch_comThrowFlag = 0; 
		}
		else if(st_Temp->uin_rec_bytes == 2)
		{
			if(uch_Temp != 0x8B)
			{
				st_Temp->uin_rec_bytes=0;
			}
			uch_comThrowFlag = 0; 
		}
		else if(st_Temp->uch_buf[st_Temp->uin_rec_bytes-2] == 0x8B && uch_comThrowFlag == 0) //����2�����
		{
			if(uch_Temp == 0x8B)
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
			else if(uch_Temp == 0x8B)//����֡���¿�ʼ
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

void CProtocal::Comm_PushToFifo(SR_FIFO *p)
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
			&&uch_LastChar == 0x8A)//��������0x7d,���0x82
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


void CProtocal::Comm_PushBuffToFifo(INT8U *p,INT8U len)
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
			&&uch_LastChar == 0x8A)//��������0x7d,���0x82
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
void CProtocal::Comm_SendFrame(INT8U uch_ComType)
{  
    SR_FIFO *p;
    
    p = GetFifo(uch_ComType);
    
	Comm_PushToFifo(p);

	 if(COM_SCI3 == uch_ComType)
     {
        USART_ITConfig(USART3,USART_IT_TXE,ENABLE);
     } 
     else if(COM_SCI6 == uch_ComType)
     {
        USART_ITConfig(USART6,USART_IT_TXE,ENABLE);
     } 
}

SR_FIFO * CProtocal::GetFifo(INT8U uch_ComType)
{    
    SR_FIFO *p;    
    switch(uch_ComType)
    {
    case COM_SCI3:
        p = &m_Uart3.st_TrnFifo;
        break;
    case COM_SCI6:
        p = &m_Uart6.st_TrnFifo;
        break;
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
void Comm_Protocal_Precess(void)
{
    m_Uart3.Comm_DealPack();
//	m_Uart6.Comm_DealPack();
}
