//==================================================================================================
//| 文件名称 | Comm_Fifo.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Comm_Fifo.cpp所对应的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright2008, 聚光科技(FPI)
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2016.03.31  | zhliu    | 初版
//==================================================================================================

#ifndef COMM_FIFO_H
#define COMM_FIFO_H

#include "stm32f4xx.h"
#include "Comm_Myfun.h"
#include <string.h>
#include "TypeDefine.h"

#define FIFO_MAX           2046   //数据FIFO区2046字节

//FIFO结构体
typedef struct
{
    u8   uch_status;          //状态
	u16  uin_Inindex;           //当前入序号
	u16  uin_Outindex;           //当前出序号
    u16  uin_bytes;           //已存数据量
	u8   uch_fifo[FIFO_MAX];  //数据缓冲区
}SR_FIFO;

//FIFO处理模块类定义
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
	BOOL  b_Enable;//模块使能
    
    SR_FIFO st_RevFifo;//接收
    SR_FIFO st_TrnFifo;//发送
};

#endif
