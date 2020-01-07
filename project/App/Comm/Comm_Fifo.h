//==================================================================================================
//| �ļ����� | Comm_Fifo.h
//|--------- |--------------------------------------------------------------------------------------
//| �ļ����� | Comm_Fifo.cpp����Ӧ��ͷ�ļ�
//|--------- |--------------------------------------------------------------------------------------
//| ��Ȩ���� | Copyright2008, �۹�Ƽ�(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  �汾    |  ʱ��       |  ����     | ����
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2016.03.31  | zhliu    | ����
//==================================================================================================

#ifndef COMM_FIFO_H
#define COMM_FIFO_H

#include "stm32f4xx.h"
#include "Comm_Myfun.h"
#include <string.h>
#include "TypeDefine.h"

#define FIFO_MAX           2046   //����FIFO��2046�ֽ�

//FIFO�ṹ��
typedef struct
{
    u8   uch_status;          //״̬
	u16  uin_Inindex;           //��ǰ�����
	u16  uin_Outindex;           //��ǰ�����
    u16  uin_bytes;           //�Ѵ�������
	u8   uch_fifo[FIFO_MAX];  //���ݻ�����
}SR_FIFO;

//FIFO����ģ���ඨ��
class CFifo
{
public:
	CFifo();
	~CFifo(){;}
    
	void Fifo_Init(void);
    BOOL FifoPush(INT8U data,SR_FIFO *pFifo);
    BOOL FifoPop(INT8U *pdata,SR_FIFO *pFifo);
    BOOL FifoClear(SR_FIFO *pFifo);
    
    BOOL FifoPushRcvFifo(INT8U data);
    BOOL FifoPushTrnFifo(INT8U data);
    
    BOOL FifoPopRcvFifo(INT8U *pdata);
    BOOL FifoPopTrnFifo(INT8U *pdata);
public:
	BOOL  b_Enable;//ģ��ʹ��
    
    SR_FIFO st_RevFifo;//����
    SR_FIFO st_TrnFifo;//����
};

#endif
