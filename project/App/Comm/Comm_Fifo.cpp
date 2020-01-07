#include "Comm_Fifo.h"
#include "TypeDefine.h"

CFifo::CFifo(void)
{
    INT16U i;
    b_Enable = TRUE;//Ĭ��ģ��ʹ�ܿ���
    
    //״̬��ʼ��
    st_RevFifo.uch_status  = 0;
	st_TrnFifo.uch_status  = 0;
	
	st_RevFifo.uin_Outindex = 0;
	st_RevFifo.uin_Inindex = 0;
	st_RevFifo.uin_bytes = 0;
	
	st_TrnFifo.uin_Outindex = 0;
	st_TrnFifo.uin_Inindex = 0;
	st_TrnFifo.uin_bytes = 0;
    //�建����
    for(i=0;i<FIFO_MAX;i++)
    {
        st_RevFifo.uch_fifo[i]  = 0;
        st_TrnFifo.uch_fifo[i] = 0;
    }
}


void CFifo::Fifo_Init(void)
{
   // u16 i;
}


BOOL CFifo::FifoPush(INT8U data,SR_FIFO *pFifo)
{	
	if(pFifo->uin_bytes >= FIFO_MAX)
	{
		return FALSE;
	}
	
	pFifo->uch_fifo[pFifo->uin_Inindex++] = data;
    if (pFifo->uin_Inindex >= FIFO_MAX)
    {
        pFifo->uin_Inindex = 0;
    }
	pFifo->uin_bytes++;
    return TRUE;	
}

BOOL CFifo::FifoPushRcvFifo(INT8U data)
{	
    return (FifoPush(data,&st_RevFifo));
}

BOOL CFifo::FifoPushTrnFifo(INT8U data)
{	
    return (FifoPush(data,&st_TrnFifo));
}

BOOL CFifo::FifoPop(INT8U *pdata,SR_FIFO *pFifo)
{
	if(pFifo->uin_bytes <= 0)
	{
		return FALSE;	
	}
	*pdata = pFifo->uch_fifo[pFifo->uin_Outindex++];
	
	if (pFifo->uin_Outindex >= FIFO_MAX)
	{
		pFifo->uin_Outindex = 0;
	}
	pFifo->uin_bytes--;
	return TRUE;
}

BOOL CFifo::FifoPopRcvFifo(INT8U *pdata)
{	
    return (FifoPop(pdata,&st_RevFifo));
}

BOOL CFifo::FifoPopTrnFifo(INT8U *pdata)
{	
    return (FifoPop(pdata,&st_TrnFifo));
}

BOOL CFifo::FifoClear(SR_FIFO *pFifo)
{
	pFifo->uin_Outindex = 0;
	pFifo->uin_Inindex = 0;
	pFifo->uin_bytes = 0;
	return TRUE;
}