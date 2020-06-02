#ifndef COMM_SYSTEM_H
#define COMM_SYSTEM_H

#include "stm32f4xx.h"
#include "Comm_Myfun.h"
#include "Comm_Fifo.h"
#include <string.h>

#define COM_SCI6            0x01   //串口
#define COM_CAN             0x02   //can
#define COM_SCI4            0x03   //串口
#define COM_SCI1            0x04   //串口
#define COM_SCI3            0x05   //串口
#define COM_SCI5            0x06   //串口

#define BUF_MAX            1300   //数据缓冲区1024字节
#define FIFO_MAX           2600   //数据FIFO区2046字节
#define MAX_ROUT           8	  //最长路径
#define RECIEVED     	   0x11   //接收到数据
#define SENDING     	   0x22   //发送中
#define COM_IDLE 		   0x33   //空闲

#define CMD_READ           0x55   //读命令
#define CMD_WRITE          0x66   //写命令
#define CMD_R_ANSWER       0xAA   //读回应
#define CMD_W_ANSWER       0x99   //写回应

#define CMD_W_SUCCESS      0x88   //写成功
#define CMD_W_FALSE        0xAA   //写失败


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

class CFpiNew:public CFifo
{
public:
	CFpiNew(INT8U uch_Com);
	~CFpiNew(){;}
    
    //函数声明
    void Comm_Init(void);
    void Comm_MakePack(SR_FRAME *p_frame);
    char Comm_DecPack(SR_FRAME *p_frame);
    void Comm_DealPack(void);
    //void Comm_RcvNByte(u8 uch_Com,u8 *p_data,u16 uin_num);
    void Comm_SendFrame(INT8U uch_ComType);    
    BOOL Comm_GetFrameFromFifo(void);
    void Comm_PushFpiToFifo(SR_FIFO *p);
    void Comm_PushFpiBuffToFifo(INT8U *p,INT8U len);
    SR_FIFO * GetFifo(INT8U uch_ComType);
public:
	BOOL  b_Enable;//模块使能
    
    SR_COM st_com_buff;//中间缓存
    INT8U uch_comThrowFlag;//0x82记录
    INT8U much_ComType;//端口编号
};

extern INT16U uin_buff[512];


extern CFpiNew m_FpiUart3;
extern CFpiNew m_FpiUart6;


extern void Comm_FpiNew_Precess(void);

#endif
