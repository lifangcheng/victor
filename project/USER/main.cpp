#include "stm32f4xx.h"
#include "STM32_BSP.h"
#include <string.h>
#include <stdio.h>
#include "Comm_Protocal.h"

#include "save.h"
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
extern uint8_t UsbHidReceiveComplete ;
extern INT16U uin_CountSec; 
  
INT16U uin_buff[512];// = {2,45,456,6789,64564};
#define FLASH_SAVE_ADDR  500
INT8U uch_buff[15];

int main(void)
{ 
    static RCC_ClocksTypeDef RCC_Clocks;
	static INT32U data;
	static FP32 f_Temp;
	static INT8U flag;
    u8 strings[15] = "hello word \n";
	int i;
     
	//DelayNms(100);
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
	//gst_SysPara.uch_Adr = 35;
	//gst_SysPara.uch_Baud = 2;
	
	//SaveToFlash((INT32U)&gst_SysPara.uch_Adr);
	//SaveToFlash((INT32U)&gst_SysPara.uch_Baud);
	SetDA_Voltage(1.0);  
	SetPWM_Duty(PWM_CHL1,70);
//	MT25QL128_Erase_Chip();
   // LED1_ON();
    LED1_OFF();
	LED2_OFF();
    LED3_OFF();
	while(1)
	{
	 //   Comm_Protocal_Precess();
//flag=AD4004_ReadRegister();
		WDI_TOGGLE();
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
	        SendImageData(uin_buff);
        }	
#endif
	}  
}
