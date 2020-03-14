//==================================================================================
//| 文件名称 | 通信应用函数
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
#include "Comm_ProtocalUser.h"
#include "TypeDefine.h"
#include "save.h"
#include "Pro_Include.h"

#include "stm32_Bsp.h"
#include <string.h>
#include <stdio.h>

#include "stm32_Usart.h"
extern INT8U guc_SlideNum;
extern INT8U guc_SlideNum2;
extern INT8U guc_SlideNum3;

extern INT8U guc_IO[6];
extern INT32U gul_Count;
extern FP32 gf_P1,gf_I1,gf_IA1,gf_D1,gf_Nerr;
extern FP32 gf_P2,gf_I2,gf_IA2;
extern FP32 gf_P3,gf_I3,gf_IA3;
extern FP32 gf_P4,gf_I4,gf_IA4;
INT8U guc_BoxTemFlag = 0;

char guc_SoftWarehmi[33]={"XXXXXXXXXXXXXXXXXXXX"};

//==================================================================================
//| 函数名称 | Comm_Slavepack()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 从收数据包
//|----------|----------------------------------------------------------------------
//| 输入参数 | pr 接收数据包指针,pt 发送数据包指针
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
void Comm_Slavepack(SR_FRAME *pr,SR_FRAME *pt)
{
	 INT32U i,j;

	 INT16U uin_Tmp;
     switch(pr->uch_cmd)
     {
		//----------------------------------0x20: 序列号---------------------------------//
	  case 0x00:
            if(pr->uch_excmd == CMD_READ )//
            {
                pt->uin_len = 2112;                       //从cpu  发送数据到电脑   
				for(i=0;i<64;i++)
				{
				    EncodeUint(0,pt->uch_data+i);          
				}
                
				for(i=64;i<2112;i+=2)
				{
				    EncodeUint(i,pt->uch_data+i);          
				}         
				
            } 
           break;

     }
 
	 
}		
	





