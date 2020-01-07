#ifndef COMM_SYSTEM_H
#define COMM_SYSTEM_H

#include "stm32f4xx.h"
#include "Comm_Myfun.h"
#include "Comm_Fifo.h"
#include <string.h>

#define COM_SCI3            0x03   //串口
#define COM_SCI6            0x06   //串口

#define BUF_MAX            1024   //数据缓冲区1024字节
#define FIFO_MAX           2046   //数据FIFO区2046字节
#define MAX_ROUT           8	  //最长路径
#define RECIEVED     	   0x11   //接收到数据
#define SENDING     	   0x22   //发送中
#define COM_IDLE 		   0x33   //空闲

#define CMD_READ           0x55   //读命令
#define CMD_WRITE          0x66   //写命令

#define CMD_W_SUCCESS      0xAA   //写成功
#define CMD_W_FALSE        0xBB   //写失败


//端口结构体
typedef struct
{
    u8   uch_status;          //端点状态
    u16  uin_rec_bytes;       //已接受数据
	u16  uin_tran_bytes;      //待发送数据
	u8   uch_buf[BUF_MAX];    //数据缓冲区
}SR_COM;
//数据帧结构体
typedef struct 
{
	u8   uch_head;           //帧头
	u8   uch_resv[4];        //保留字节
	u8   uch_sumrout;        //总路径长度
	u8   uch_nowrout;        //当前路径长度
	u8   uch_rout[MAX_ROUT]; //路径列表
	u8   uch_cmd;            //命令码
    u8   uch_excmd;          //命令类型
    u16  uin_len;            //数据区长度
    u8   uch_data[BUF_MAX];  //数据区
    u16  uin_crc;            //校验码
    u8   uch_tail;           //帧尾
}SR_FRAME;

class CProtocal:public CFifo
{
public:
	CProtocal(INT8U uch_Com);
	~CProtocal(){;}
    
    //函数声明
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
	BOOL  b_Enable;//模块使能
    
    SR_COM st_com_buff;//中间缓存
    INT8U uch_comThrowFlag;//0x82记录
    INT8U much_ComType;//端口编号
};


extern CProtocal m_Uart3;
extern CProtocal m_Uart6;   //端口2与PC通信

void Comm_Protocal_Precess(void);

#endif
