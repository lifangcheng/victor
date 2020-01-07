//#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Drv_S8377.h"
#include "STM32_BSP.h"

//uint8_t Pixel[138];
/*************************************************************************
*  �������ƣ�CCD_init
*  ����˵����CCD��ʼ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
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
*  �������ƣ�SamplingDelay
*  ����˵����CCD��ʱ���� 2us
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
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

void Dly_ns(void)   //��ԼΪ60ns
{
   INT8U i ;
   for(i=0;i<10;i++) 
   {
       asm("nop");
       asm("nop");
   }          
}


/*************************************************************************
*  �������ƣ�StartIntegration
*  ����˵����CCD��������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
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
*  �������ƣ�ImageCapture
*  ����˵����CCD��������
*  ����˵����* ImageData   ��������
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
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
   for(i = 0; i < 10; i++) {                    //����250����CCD��ͼ����ȥ�Ƚ�ƽ����
       Dly_ns();  //600ns                  //�Ѹ�ֵ�Ĵ���߸�С�ﵽ�Լ�����Ľ����
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
*  �������ƣ�CalculateIntegrationTime
*  ����˵���������ع�ʱ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
*************************************************************************/

INT8U IntegrationTime = 10;
 void S8377_CalculateIntegrationTime(void) {
INT8U Pixel[128];
/* 128�����ص��ƽ��ADֵ */
INT8U PixelAverageValue;
/* 128�����ص��ƽ����ѹֵ��10�� */
/* �ع�ʱ�䣬��λms */
INT8U IntegrationTime = 10;
INT8U PixelAverageVoltage;
/* �趨Ŀ��ƽ����ѹֵ��ʵ�ʵ�ѹ��10�� */
INT16U TargetPixelAverageVoltage = 20;
/* �趨Ŀ��ƽ����ѹֵ��ʵ��ֵ��ƫ�ʵ�ʵ�ѹ��10�� */
INT16U PixelAverageVoltageError = 0;
/* �趨Ŀ��ƽ����ѹֵ������ƫ�ʵ�ʵ�ѹ��10�� */
INT16U TargetPixelAverageVoltageAllowError = 2;

    /* ����128�����ص��ƽ��ADֵ */
    PixelAverageValue = S8377_PixelAverage(128,Pixel);
    /* ����128�����ص��ƽ����ѹֵ,ʵ��ֵ��10�� */
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
*  �������ƣ�PixelAverage
*  ����˵����������ľ�ֵ����
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
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
*  �������ƣ�erzhihua
*  ����˵����ad��ֵ��ֵ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
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
     *data++=150;       //Ĭ�ϰ���Ϊ150������Ϊ0��������λ����ʾ
   //  White++;            //����׵�ĸ���
   }
   else
   {
     *data++=0;
    // Black++;           //����ڵ�ĸ���
   }
  }
}
/*************************************************************************
*  �������ƣ�vImageCalculate
*  ����˵������ֵ���˲�
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
*************************************************************************/
void S8377_vImageCalculate(INT8U * ImageData) //�˲�����
{
  INT8U *pucTemp,i;
  
//*************************************************************************
//�������㣬����һ�У����һ���׵������Ϊ�ڵ�����Ϊ�ĵ�Ϊ�ڵ㡣

    pucTemp=&ImageData[0];
    for(i=0;i<126;i++)
    {
      pucTemp++;                //��һ��������2���㲻Ҫ
      if(*pucTemp>=1)
      {
        if((*(pucTemp-1)==0)&&(*(pucTemp+1)==0))
        {
          *pucTemp=0;
        }
      }
//��ʴ���㣬����һ�У����һ���ڵ������Ϊ�׵�����Ϊ�ĵ�Ϊ�׵㡣
      
      if(*pucTemp==0)
        {
          if((*(pucTemp-1)==150)&&(*(pucTemp+1)==150))          //150Ϊ��λ����ʾֵ
          {
            *pucTemp=150;
          }
        }     
    }  
}

/*************************************************************************
*  �������ƣ�SendHex
*  ����˵�����ɼ���������
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
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
*  �������ƣ�SendImageData
*  ����˵���� �������ݸ���λ��
*  ����˵����* ImageData   ��������
*  �������أ���
*  �޸�ʱ�䣺2019-08-05
*  ��    ע��
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
*  �������ƣ� GetCenter(void)
*  ����˵��������ʶ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2015-2-13
*  ��    ע���Լ�����
*************************************************************************/
void GetCenter(INT8U * ImageData)       //���Է����ж��� 
{                           
      S8377_erzhihua(256,ImageData);         
      S8377_vImageCalculate(ImageData);        //�˲�
}