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
	 FP32 f_Tmp;
	 FP32 f_K,f_B;
	 INT16U uin_Tmp;
     switch(pr->uch_cmd)
     {
		//----------------------------------0x20: 序列号---------------------------------//
		case 0x3D:
			if(pr->uch_excmd == CMD_READ && pr->uin_len == 0)
			{
				pt->uin_len = 12;
				for(i=0;i<12;i++)
				{
					pt->uch_data[i] = gst_SysPara.uch_SerilNum[i];
				}
             }
			else if(pr->uch_excmd == CMD_WRITE )
			{
				pt->uin_len = 1;
				pt->uch_data[0] = CMD_W_FALSE;
				
				if(pr->uin_len == 12)
				{
					for(i=0;i<12;i++)
					{
						if((pr->uch_data[i] >= '0' && pr->uch_data[i] <= '9')//数字
						 ||(pr->uch_data[i] >= 'A' && pr->uch_data[i] <= 'Z')//大写字母
						 ||(pr->uch_data[i] >= 'a' && pr->uch_data[i] <= 'z')//小写字母
						 || pr->uch_data[i] == '.' || pr->uch_data[i] == ' '
						 || pr->uch_data[i] == '-') //横线
						{
					
						}
						else
						{
							break;
						}
					}
                    
                    if(i>=12)
                    {
                        for(j=0;j<12;j++)
                        {
                            gst_SysPara.uch_SerilNum[j] = pr->uch_data[j];
                        }
						SaveToFlashExt((INT32U)gst_SysPara.uch_SerilNum,12);
						pt->uch_data[0] = CMD_W_SUCCESS;
                    }
				}
			}
			break;	
        //------------------------------0x21: 软件版本------------------------------//
        case 0x21:
           if(pr->uch_excmd == CMD_READ)
           {
               pt->uin_len = 32;
              // CopyFromAToB((u8 *)guc_SoftWare,pt->uch_data,32);
           }
           break;
           
        
      //----------------------------------0x22: 通讯信息---------------------------------//
		case 0x22:
			if(pr->uch_excmd == CMD_READ && pr->uin_len == 0)  //cpu接收到读的命令
			{
				pt->uin_len = 2;
				pt->uch_data[0] = gst_SysPara.uch_Adr;      //变量发送给串口电脑数据
				pt->uch_data[1] = gst_SysPara.uch_Baud;
                        }
			else if(pr->uch_excmd == CMD_WRITE)                 //cpu接收到写的命令
			{
				pt->uin_len = 1;                             //如果发送为1 则发送错误        
				pt->uch_data[0] = CMD_W_FALSE;
				
				if(pr->uin_len == 2)                         //cpu接收写两个字节的命令           
				{
					if((pr->uch_data[0] >= 0x20 && pr->uch_data[0] <= 0x3f)
					 &&(pr->uch_data[1] >= 0x01 && pr->uch_data[1] <= 0x05)) //数据范围设定
					{
						gst_SysPara.uch_Adr = pr->uch_data[0];           //把接收到的数据写到eprom中
						gst_SysPara.uch_Baud = pr->uch_data[1];
				
                        Uart2_Init(gst_SysPara.uch_Baud);
			//			Uart1_Init(gst_SysPara.uch_Baud);
				//		 Uart6_Init(gst_SysPara.uch_Baud);
						SaveToFlash((INT32U)&gst_SysPara.uch_Adr);
						SaveToFlash((INT32U)&gst_SysPara.uch_Baud);
						pt->uch_data[0] = CMD_W_SUCCESS;
					}
				}
			}
			break;	                  
                   
           //----------------------------------读取仪表自检状态-------------------------
           	case 0x30:
                if(pr->uch_excmd == CMD_READ && pr->uin_len == 0)//读数据
                {
                    pt->uin_len = 1;                       //从cpu  发送数据到电脑   
                    //状态为2时，自检失败  
                }
                else if(pr->uch_excmd == CMD_WRITE)                 //cpu接收到写的命令
                {
                    //自检状态默认正在自检中
              //      st_MeasurePara.st_WorkMode.uch_SysTest = 0;
                    pt->uin_len = 1;                             //如果发送为1 则发送错误        
                    pt->uch_data[0] = CMD_W_FALSE;
                    
                    if(pr->uin_len == 1)                         //cpu接收写一个字节的命令           
                    {
                        if(pr->uch_data[0] == 0x00 || pr->uch_data[0] == 0x01)
                        {
                           //自检启动或关闭          

                            pt->uch_data[0] = CMD_W_SUCCESS;          
                        }
                    }
                }
                break;

  



	//----------------------------------:升级相关 ---------------------------------//
		//---------------------------------- 0x61: 读取升级配置信息 --------------------//
//		case 0xa1:
           
//			break;
		//---------------------------------- 0x62: 下载代码到临时区 -----------------------//   
//		case 0xa2:
            
            
//			break;
		//-------------------------------- 0x63: 传送代CRC整体校验数据 ---------------------//
//		case 0xa3:
          
//			break;
#if 0
        //----------------------------------0x64 ---------------------------------//
            case 0x64:
			if (pr->uch_excmd == 0x77) //Update Read
            {	    			
                ;	
            }
            else if (pr->uch_excmd == 0x88)  //Update Write
            {
                //Update_Flag = 0;
                static INT16U uin_tmp = 0;
              
                //s_SendCmd_New(pRev->buf[5],0xDD,pCmd[0],pRev,pTrans);
                pt->uch_excmd    = 0xDD; 
                pt->uin_len  = 1;
                pt->uch_data[0] = FALSE;
                WDI_TOGGLE();	
                CLI();
                uch_UpdateFlag = Boot_GetUpdateFlag();
                if (uch_UpdateFlag == UPDATE_FLAG_CHANGE_OK)
                {
                    uin_tmp = Boot_SetUpdateFlag(UPDATE_FLAG_SUCESS);
                    if (uin_tmp == 1)
                    {
                        pt->uch_data[0] = TRUE;
                    }
                }
                SEI();
			}
			break;
        //---------------------------------- 0x65: 程序恢复命令 --------------------------//
        case 0x65:
            if (pr->uch_excmd == 0x77) //Update Read
            {	    			
               ;	
            }
            else if (pr->uch_excmd == 0x88)  //Update Write
            {
                //s_SendCmd_New(pRev->buf[5],0xDD,pCmd[0],pRev,pTrans);
                pt->uch_excmd    = 0xDD; 
                pt->uin_len  = 1;

                pt->uch_data[0] = FALSE;
                WDI_TOGGLE();	
                CLI();
                uch_UpdateFlag = (INT8U)Boot_GetUpdateFlag();
                if (uch_UpdateFlag == UPDATE_FLAG_CHANGE_OK || uch_UpdateFlag == UPDATE_FLAG_SUCESS)
                {
                    if (Boot_SetUpdateFlag(UPDATE_FLAG_REFRESH) == 1)
                    {
                        Boot_Reset();
                        pt->uch_data[0] = TRUE;
                    }
                }
                SEI();
            }
            break;
#endif	
     }
 
	 
}		
	





