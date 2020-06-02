#include "stm32f4xx.h"
#include "STM32_BSP.h"
#include <string.h>
#include <stdio.h>
#include "Comm_FpiNew.h"

#include "save.h"
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
extern uint8_t UsbHidReceiveComplete ;
extern INT16U uin_CountSec; 
  
INT16U uin_buff[512];
#define FLASH_SAVE_ADDR  500
INT8U uch_buff[15];

//SR_FRAME *pt;

				
void SendPack(void)
{
    u16 k,i;
    u16 uin_Crc;
static	 INT8U buff[2120];

    
    k = 0;      
    //帧头
    buff[k++] = 0x7D;
	buff[k++] = 0x7B;

		for(i=0;i<64;i++)
		{
			buff[k++] = 0xAA;       
		}
		
		for(i=0;i<512;i++)
		{
		    EncodeUint(0,buff+k); 
		 //   EncodeUint(i*10,buff+k); 
			k+=2;
			EncodeUint(uin_buff[i],buff+k); 
			k+=2;
		}         
   // k += p_frame->uin_len;
    //Crc校验
    uin_Crc = GetCrc16Bit(buff+2,k-2);
    EncodeUint(uin_Crc,buff+k);
    k += 2;
    //帧尾
    buff[k++] = 0x7D;
	buff[k++] = 0x7D;
	
	USART6_Pack_Send(buff,k);
	
}
int main(void)
{ 
    static RCC_ClocksTypeDef RCC_Clocks;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    WDI_TOGGLE();
	//delay_init(160);    //初始化延时函数
    
	BSP_Init();	
    WDI_TOGGLE();
	RCC_GetClocksFreq(&RCC_Clocks);
	WDI_TOGGLE();
	SysClock_Init();
    InitParaFromFlash(INIT_NORMAL);
    WDI_TOGGLE();

	SetDA_Voltage(1.0);  
	SetPWM_Duty(PWM_CHL1,70);
   // LED1_ON();
    LED1_OFF();
	LED2_OFF();
    LED3_OFF();
	while(1)
	{
		WDI_TOGGLE();
		Comm_FpiNew_Precess();
#if 0	
		if (UsbHidReceiveComplete)
		{
		  USBD_HID_SendReport (&USB_OTG_dev, 
								 strings,
								 64);
		  UsbHidReceiveComplete = 0;
		  } 
#endif		
		
#if 1		
		if(uin_CountSec > gst_SysPara.uin_TimeInt) 
        {
		//    LED2_TOGGLE();
            uin_CountSec = 0;
            S10121_ImageCapture((INT16U*)uin_buff);    
	      //  SendImageData(uin_buff);
			//SendPack();
        }	
#endif
	}  
}
