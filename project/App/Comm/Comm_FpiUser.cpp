//==================================================================================
//| 文件名称 | 通信应用函数
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
#include "Comm_FpiUser.h"
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


void Comm_Masterpack(SR_FRAME *pr,SR_FRAME *pt)
{
	// INT32U i,j;
	// FP32 f_Tmp;
	// INT16U uin_Tmp;
     switch(pr->uch_cmd)
     {
      case 0x35:
      if(pr->uch_excmd == CMD_R_ANSWER)
      {
      pt->uin_len = 0;
      //gst_Key.uch_Key1 = pr->uch_data[0];
      //gst_Key.uch_Key2 = pr->uch_data[1];
      //gst_Key.uch_Key3 = pr->uch_data[2];
      //gst_Key.uch_Key4 = pr->uch_data[3];
      }
      break;
      case 0x3d:
      if(pr->uch_excmd == CMD_R_ANSWER)
      {
      pt->uin_len = 0;
      //gst_SysPara.f_CurConc = DecodeFloat(pr->uch_data+0);//光散射浓度
      //m_TempCtrl_Scate.f_TarTemp = DecodeFloat(pr->uch_data+4);//光散射目标温度
      //m_Environ.f_ScateTemp = DecodeFloat(pr->uch_data+8);//光散射实际温度
      }
      break;
      case 0x11:
      if(pr->uch_excmd == CMD_R_ANSWER)
      {
          INT8U i;
      pt->uin_len = 0;
      for(i=0;i<32;i++)
      {
      //guc_SoftWarehmi[i]=pr->uch_data[i];
      }
      }
      break;
     } 
}
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
	  case 0x20:
		    if(pr->uch_excmd == CMD_READ && pr->uin_len == 0)//读数据
			{
				pt->uin_len = 1024;                       //从cpu  发送数据到电脑
				for(i=0;i<512;i++)
				{
				   // EncodeUint(uin_buff[i],pt->uch_data+2*i);  
				   EncodeUint(i,pt->uch_data+2*i);          
				}     
			}
#if 0			
            if(pr->uch_excmd == CMD_READ )//
            {
                pt->uin_len = 1088;                       //从cpu  发送数据到电脑   
				for(i=0;i<64;i++)
				{
				    EncodeUint(0xAA,pt->uch_data+i);          
				}
                
				for(i=64;i<1088;i++)
				{
				    EncodeUint(uin_buff[i],pt->uch_data+2*i);          
				}         
				
            } 
#endif
           break;
        case 0x21:
                if(pr->uch_excmd == CMD_READ && pr->uin_len == 0)//读数据
                {
                    pt->uin_len = 2;                       //从cpu  发送数据到电脑
                    EncodeUint(gst_SysPara.uin_TimeInt,pt->uch_data);
                }
                else if(pr->uch_excmd == CMD_WRITE)//写数据
                {
                    pt->uin_len = 1;                             //
                    pt->uch_data[0] = CMD_W_FALSE;
                    if(pr->uin_len == 2)                                  
                    {                                                 
                         if(pr->uch_data[0] >= 0x01 && pr->uch_data[0] >= 1000)
						{ 
						    gst_SysPara.uin_TimeInt = DecodeUint(pr->uch_data);
                            SaveToFlash((INT32U)&gst_SysPara.uin_TimeInt);
	                        pt->uch_data[0] = CMD_W_SUCCESS;   
                        }
                    }
		        }
		   break; 
		   case 0x22:
                if(pr->uch_excmd == CMD_READ && pr->uin_len == 0)//读数据
                {
                    pt->uin_len = 2;                       //从cpu  发送数据到电脑
                    EncodeUint(gst_SysPara.uin_SamAvr,pt->uch_data);
                }
                else if(pr->uch_excmd == CMD_WRITE)//写数据
                {
                    pt->uin_len = 1;                             //
                    pt->uch_data[0] = CMD_W_FALSE;
                    if(pr->uin_len == 2)                                  
                    {                                                 
                         if(pr->uch_data[0] >= 0x01 && pr->uch_data[0] >= 1000)
						{ 
						    gst_SysPara.uin_SamAvr = DecodeUint(pr->uch_data);
                            SaveToFlash((INT32U)&gst_SysPara.uin_SamAvr);
	                        pt->uch_data[0] = CMD_W_SUCCESS;   
                        }
                    }
		        }
		   break; 
     }
 
	 
}		
	





