#ifndef COMM_SYSTEM_H
#define COMM_SYSTEM_H

#include "stm32f4xx.h"
#include "Comm_Myfun.h"
#include "Comm_Fifo.h"
#include <string.h>

#define COM_SCI3            0x03   //����
#define COM_SCI6            0x06   //����

#define BUF_MAX            1024   //���ݻ�����1024�ֽ�
#define FIFO_MAX           2046   //����FIFO��2046�ֽ�
#define MAX_ROUT           8	  //�·��
#define RECIEVED     	   0x11   //���յ�����
#define SENDING     	   0x22   //������
#define COM_IDLE 		   0x33   //����

#define CMD_READ           0x55   //������
#define CMD_WRITE          0x66   //д����

#define CMD_W_SUCCESS      0xAA   //д�ɹ�
#define CMD_W_FALSE        0xBB   //дʧ��


//�˿ڽṹ��
typedef struct
{
    u8   uch_status;          //�˵�״̬
    u16  uin_rec_bytes;       //�ѽ�������
	u16  uin_tran_bytes;      //����������
	u8   uch_buf[BUF_MAX];    //���ݻ�����
}SR_COM;
//����֡�ṹ��
typedef struct 
{
	u8   uch_head;           //֡ͷ
	u8   uch_resv[4];        //�����ֽ�
	u8   uch_sumrout;        //��·������
	u8   uch_nowrout;        //��ǰ·������
	u8   uch_rout[MAX_ROUT]; //·���б�
	u8   uch_cmd;            //������
    u8   uch_excmd;          //��������
    u16  uin_len;            //����������
    u8   uch_data[BUF_MAX];  //������
    u16  uin_crc;            //У����
    u8   uch_tail;           //֡β
}SR_FRAME;

class CProtocal:public CFifo
{
public:
	CProtocal(INT8U uch_Com);
	~CProtocal(){;}
    
    //��������
    void Comm_Init(void);
    void Comm_MakePack(SR_FRAME *p_frame);
    char Comm_DecPack(SR_FRAME *p_frame);
    void Comm_DealPack(void);
    //void Comm_RcvNByte(u8 uch_Com,u8 *p_data,u16 uin_num);
    void Comm_SendFrame(INT8U uch_ComType);    
    BOOL Comm_GetFrameFromFifo(void);
    void Comm_PushToFifo(SR_FIFO *p);
    void Comm_PushBuffToFifo(INT8U *p,INT8U len);
    SR_FIFO * GetFifo(INT8U uch_ComType);
public:
	BOOL  b_Enable;//ģ��ʹ��
    
    SR_COM st_com_buff;//�м仺��
    INT8U uch_comThrowFlag;//0x82��¼
    INT8U much_ComType;//�˿ڱ��
};


extern CProtocal m_Uart3;
extern CProtocal m_Uart6;   //�˿�2��PCͨ��

void Comm_Protocal_Precess(void);

#endif
