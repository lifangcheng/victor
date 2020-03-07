//#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Drv_S8377.h"
#include "STM32_BSP.h"

#ifdef S8377
//uint8_t Pixel[138];
/*************************************************************************
*  函数名称：CCD_init
*  功能说明：CCD初始化
*  参数说明：
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*************************************************************************/
void S8377_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Configure PD2 as inputs push-pull, max speed 50 MHz 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    S8377_ST_SET(); 
    S8377_CLK_CLR(); 
}

/*************************************************************************
*  函数名称：SamplingDelay
*  功能说明：CCD延时程序 2us
*  参数说明：
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*************************************************************************/
void SamplingDelay(void)
{
   INT16U i ;
   for(i=0;i<200;i++) 
   {
       asm("nop");
       asm("nop");
   }
}

void Dly_ns(void)   //大约为60ns
{
   INT8U i ;
   for(i=0;i<10;i++) 
   {
       asm("nop");
       asm("nop");
   }          
}


/*************************************************************************
*  函数名称：StartIntegration
*  功能说明：CCD启动程序
*  参数说明：
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*************************************************************************/
void S8377_StartIntegration(void) {

    INT8U i;

    S8377_ST_CLR();            /* ST  = 1 */
    //Dly_ns();
    SamplingDelay();
    S8377_CLK_SET();           /* CLK = 1 */
    //Dly_ns();
    SamplingDelay();
    S8377_CLK_CLR();           /* CLK = 0 */
    SamplingDelay();
    S8377_ST_SET();            /* ST  = 0 */

    for(i=0; i<255; i++) {
        //SamplingDelay();
        SamplingDelay();
        S8377_CLK_SET();        /* CLK = 1 */
        //SamplingDelay();
        SamplingDelay();
        S8377_CLK_CLR();       /* CLK = 0 */
    }
    //SamplingDelay();
    SamplingDelay();
    S8377_CLK_SET();           /* CLK = 1 */
    //SamplingDelay();
    SamplingDelay();
    S8377_CLK_CLR();           /* CLK = 0 */
}

/*************************************************************************
*  函数名称：ImageCapture
*  功能说明：CCD采样程序
*  参数说明：* ImageData   采样数组
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*ImageData =  ad_once(ADC1, AD6a, ADC_8bit);
*************************************************************************/

bool S8377_ImageCapture(INT16U* ImageData) {

    static INT16U i,j;

    S8377_ST_CLR();            /* ST  = 1 */
    //Dly_ns();
    SamplingDelay();
    S8377_CLK_SET();           /* CLK = 1 */
    //Dly_ns();
    SamplingDelay();
    S8377_CLK_CLR();           /* CLK = 0 */
    SamplingDelay();
    S8377_ST_SET();            /* ST  = 0 */
    SamplingDelay();

    //Delay 10us for sample the first pixel
    /**/
   for(i = 0; i < 10; i++) {                    //更改250，让CCD的图像看上去比较平滑，
       Dly_ns();  //600ns                  //把该值改大或者改小达到自己满意的结果。
    }

    //Sampling Pixel 1

    LTC2355_ReadInt16u((INT16U)5,ImageData);
    ImageData ++ ;
    //CLK_ClrVal();           /* CLK = 0 */

    for(i=0; i<255; i++) {
      //  SamplingDelay();

	   #if 1
       // CLK_ClrVal();       /* CLK = 0 *
       if(FALSE == S8377_EOS_READ())
       {
         SamplingDelay();
         S8377_CLK_SET();       /* CLK = 1 */
         SamplingDelay();
         S8377_CLK_CLR();       /* CLK = 0 */
         SamplingDelay();
		 j++;
          break;
       }
#endif 
        SamplingDelay();
        S8377_CLK_SET();       /* CLK = 1 */
        SamplingDelay();
        S8377_CLK_CLR();       /* CLK = 0 */
        SamplingDelay();
        //Sampling Pixel 2~128

       LTC2355_ReadInt16u((INT16U)5,(INT16U*)ImageData);
       ImageData ++ ;

    }
#if 1	
    while(FALSE != S8377_EOS_READ())
    {
        SamplingDelay();
        S8377_CLK_SET();           /* CLK = 1 */
        SamplingDelay();
        S8377_CLK_CLR();           /* CLK = 0 */
    }
#endif	
	j=0;
    SamplingDelay();
    S8377_CLK_SET();           /* CLK = 1 */
    SamplingDelay();
    S8377_CLK_CLR();           /* CLK = 0 */

    return TRUE;
}


/*************************************************************************
*  函数名称：CalculateIntegrationTime
*  功能说明：计算曝光时间
*  参数说明：
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*************************************************************************/

INT8U IntegrationTime = 10;
 void S8377_CalculateIntegrationTime(void) {
INT8U Pixel[128];
/* 128个像素点的平均AD值 */
INT8U PixelAverageValue;
/* 128个像素点的平均电压值的10倍 */
/* 曝光时间，单位ms */
INT8U IntegrationTime = 10;
INT8U PixelAverageVoltage;
/* 设定目标平均电压值，实际电压的10倍 */
INT16U TargetPixelAverageVoltage = 20;
/* 设定目标平均电压值与实际值的偏差，实际电压的10倍 */
INT16U PixelAverageVoltageError = 0;
/* 设定目标平均电压值允许的偏差，实际电压的10倍 */
INT16U TargetPixelAverageVoltageAllowError = 2;

    /* 计算128个像素点的平均AD值 */
    PixelAverageValue = S8377_PixelAverage(128,Pixel);
    /* 计算128个像素点的平均电压值,实际值的10倍 */
    PixelAverageVoltage = (uint8_t)((int16_t)PixelAverageValue * 25 / 128);

    PixelAverageVoltageError = TargetPixelAverageVoltage - PixelAverageVoltage;
    if(PixelAverageVoltageError < -TargetPixelAverageVoltageAllowError)
    {
      PixelAverageVoltageError = 0- PixelAverageVoltageError ;
      PixelAverageVoltageError /= 5;
      if(PixelAverageVoltageError > 10 )
         PixelAverageVoltageError = 10 ;
       IntegrationTime -= PixelAverageVoltageError;
    }
    if(PixelAverageVoltageError > TargetPixelAverageVoltageAllowError)
    { 
        PixelAverageVoltageError /= 5;
        if(PixelAverageVoltageError > 10 )
           PixelAverageVoltageError = 10 ;
        IntegrationTime += PixelAverageVoltageError;}
 
/*    
    uart_putchar(UART0,0XAA) ;
    uart_putchar(UART0,PixelAverageVoltage) ;
    uart_putchar(UART0,PixelAverageVoltageError) ;
    uart_putchar(UART0,IntegrationTime) ;
 */   
    if(IntegrationTime <= 1)
        IntegrationTime = 1;
    if(IntegrationTime >= 20)
        IntegrationTime = 20;
   
  
}

/*************************************************************************
*  函数名称：PixelAverage
*  功能说明：求数组的均值程序
*  参数说明：
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*************************************************************************/
INT8U S8377_PixelAverage(INT8U len, INT8U *data) {
  INT8U i;
  INT16U sum = 0;
  for(i = 0; i<len; i++) {
    sum = sum + *data++;
  }
  return ((INT8U)(sum/len));
}

/*************************************************************************
*  函数名称：erzhihua
*  功能说明：ad数值二值化
*  参数说明：
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*************************************************************************/
//unsigned char White,Black;
void S8377_erzhihua(INT8U len, INT8U *data)
{
  INT8U i;
  INT16U Yuzhi;
  Yuzhi=S8377_PixelAverage(128,data);
  for(i = 0; i<len; i++) 
  {
   if(*data>=Yuzhi)
   {
     *data++=150;       //默认白线为150，黑线为0，方便上位机显示
   //  White++;            //计算白点的个数
   }
   else
   {
     *data++=0;
    // Black++;           //计算黑点的个数
   }
  }
}
/*************************************************************************
*  函数名称：vImageCalculate
*  功能说明：二值化滤波
*  参数说明：
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*************************************************************************/
void S8377_vImageCalculate(INT8U * ImageData) //滤波程序
{
  INT8U *pucTemp,i;
  
//*************************************************************************
//膨胀运算，对于一列，如果一个白点的两侧为黑点则认为改点为黑点。

    pucTemp=&ImageData[0];
    for(i=0;i<126;i++)
    {
      pucTemp++;                //第一个点和最后2个点不要
      if(*pucTemp>=1)
      {
        if((*(pucTemp-1)==0)&&(*(pucTemp+1)==0))
        {
          *pucTemp=0;
        }
      }
//腐蚀运算，对于一列，如果一个黑点的两侧为白点则认为改点为白点。
      
      if(*pucTemp==0)
        {
          if((*(pucTemp-1)==150)&&(*(pucTemp+1)==150))          //150为上位机显示值
          {
            *pucTemp=150;
          }
        }     
    }  
}

/*************************************************************************
*  函数名称：SendHex
*  功能说明：采集发数程序
*  参数说明：
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*************************************************************************/
void SendHex(INT8U hex) {
  INT8U temp;
  temp = hex >> 4;
  if(temp < 10) {
    USART6_Puts((u8*)(temp + '0'),1);
  } else {
    USART6_Puts((u8*)(temp - 10 + 'A'),1);
  }
  temp = hex & 0x0F;
  if(temp < 10) {
    USART6_Puts((u8*)(temp + '0'),1);
  } else {
    USART6_Puts((u8*)(temp - 10 + 'A'),1);
  }
  
}
#if 1
/*************************************************************************
*  函数名称：SendImageData
*  功能说明： 发送数据给上位机
*  参数说明：* ImageData   发送数组
*  函数返回：无
*  修改时间：2019-08-05
*  备    注：
*************************************************************************/
void SendImageData(INT16U * ImageData) {

    INT16U i = 0;
    INT8U crc = 0;
    INT8U uin_buff[5];
    /* Send Data */
    //USART6_Puts((u8*)'*',1);
    //USART6_Puts((u8*)'L',1);
    //USART6_Puts((u8*)'D',1);
//USART6_Puts((u8 *)"0",1);
//	printf("$,");
    for(i=0; i<256; i++) {
	//  sprintf((char *)uin_buff,"%d",*ImageData++);

	  
	  printf("$,%u\r\n",*ImageData++);
	 // Dly_ns();
	//  USART_SendData(USART6, 0xA5);
	//  printf("%u ",*ImageData++);
	//  USART6_Puts(uin_buff,5);
//	  USART6_Puts((u8 *)" ",1);
	 
     // SendHex(*ImageData++);

//	    memset(uin_buff,0,sizeof (uin_buff));
    }
//	printf("00#");
   // SendHex(crc);
   // USART6_Puts((u8 *)"\r\n",2);
 
}
#endif

#if 0
void SendImageData(INT16U * ImageData) {

    INT16U i = 0;
    INT8U crc = 0;
    INT8U uin_buff[5];
    /* Send Data */
	//USART_SendData(USART6, '*'); 
	//USART_SendData(USART6, 'L'); 
    USART6_Puts((u8*)'*',1);
    USART6_Puts((u8*)'L',1);
    USART6_Puts((u8*)'D',1);

	SendHex(0);
    SendHex(0);
    SendHex(0);
    SendHex(0);
	
	*ImageData =(*ImageData)/255;
#if 1	
    for(i=0; i<128; i++) {
	 // sprintf((char *)uin_buff,"%d",*ImageData++);
	  SendHex(*ImageData++);
	//  USART6_Puts((u8 *)" ",1);
	//  memset(uin_buff,0,sizeof (uin_buff));
     // SendHex(*ImageData++);
    }
#endif
    SendHex(crc);
    //USART6_Puts((u8 *)"\n",1);
    USART6_Puts((u8 *)'#',1);
}
#endif
/*************************************************************************
*
*  函数名称： GetCenter(void)
*  功能说明：中线识别
*  参数说明：
*  函数返回：无
*  修改时间：2015-2-13
*  备    注：自己添加
*************************************************************************/
void GetCenter(INT8U * ImageData)       //可以放在中断中 
{                           
      S8377_erzhihua(256,ImageData);         
      S8377_vImageCalculate(ImageData);        //滤波
}
#endif