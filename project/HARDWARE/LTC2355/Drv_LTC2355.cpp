#include "STM32_BSP.h"
#include "Drv_LTC2355.h"

BOOL LTC2355_SPI_SendInt16uOne(INT16U &uin_RevData);


void LTC2355Init(void)
{
    INT16U uin_Temp;
    GPIO_InitTypeDef GPIO_InitStructure;
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Configure PA5 as inputs push-pull, max speed 50 MHz 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    LTC2355_CONV_SET();
	LTC2355_CLK_SET();
  //  LTC2355_SPI_SendInt16uOne(uin_Temp);
}

void DelayHalfus(void)
{
	u32 i;
   	for(i=0;i<25;i++)
    {
        ;    
    }
}

BOOL LTC2355_ReadInt16u_One(INT16U &uin_RevData)
{
    BOOL   b_Flag;
    INT16U   uin_Temp;
    b_Flag = LTC2355_SPI_SendInt16u(uin_Temp);
    if(b_Flag == FALSE)
    {
        return (FALSE);
    }
    b_Flag = LTC2355_SPI_SendInt16u(uin_Temp);
    if(b_Flag == FALSE)
    {
        return (FALSE);
    }
    b_Flag = LTC2355_SPI_SendInt16u(uin_Temp);
    if(b_Flag == FALSE)
    {
        return (FALSE);
    }
    DelayNus(5);

    uin_RevData = uin_Temp;

    return(TRUE);         
}

BOOL LTC2355_ReadInt16u(INT16U uin_AveTimes,INT16U *uin_RevData)
{
    INT32U ul_Temp = 0;
    INT16U uin_Temp;
    INT16U j;
    BOOL   b_Flag;
    
    b_Flag = LTC2355_SPI_SendInt16u(uin_Temp);
    if(b_Flag == FALSE)
    {
        return (FALSE);
    }
	DelayNus(5);
	b_Flag = LTC2355_SPI_SendInt16u(uin_Temp);
	if(b_Flag == FALSE)
	{
		return (FALSE);
	}

    for(j = 0; j < uin_AveTimes; j++)
    {
		
		DelayNus(5);
        b_Flag = LTC2355_SPI_SendInt16u(uin_Temp);
        if(b_Flag == FALSE)
        {
            return (FALSE);
        }
		DelayHalfus();
 
        ul_Temp += uin_Temp;
    }
    *uin_RevData = (INT16U)(ul_Temp/uin_AveTimes);
       
    return (TRUE);    
}

BOOL LTC2355_ReadFp32(FP32 &ul_RevData)
{
    INT16U uin_Temp;
    BOOL b_Flag;
	FP32 f_Ref;
    b_Flag = LTC2355_ReadInt16u_One(uin_Temp);
    if(b_Flag == FALSE)
    {
        return (FALSE);
    }
	
    ul_RevData = (FP32)uin_Temp * LTC2355_REF1 / 65535.0; //返回数据     
    
    return(TRUE);         
}


BOOL LTC2355_ReadFp32(INT16U uin_AveTimes,FP32 &ul_RevData)
{
    INT16U uin_Temp;
    BOOL b_Flag;
	FP32 f_Ref;
    b_Flag = LTC2355_ReadInt16u(uin_AveTimes,&uin_Temp);
    if(b_Flag == FALSE)
    {
        return (FALSE);
    }    

    ul_RevData = (FP32)uin_Temp * LTC2355_REF1 / 65535.0;
     
    return(TRUE);         
}

#if 0
BOOL LTC2355_SPI_SendInt16u(INT16U &uin_RevData)
{
    INT8U i;
    INT8U  uch_TempBit;
    INT16U uin_TempByte = 0;
	
    LTC2355_CLK_CLR();
    DelayHalfus();
	LTC2355_CLK_SET();
    LTC2355_CONV_SET();
    DelayHalfus();
    LTC2355_CONV_CLR();
	
	do
	{
	   LTC2355_CLK_CLR();
       DelayHalfus();
       LTC2355_CLK_SET();
	   DelayHalfus();
	}while(LTC2355_DIN_READ());
	
	LTC2355_CLK_CLR();
    DelayHalfus();
    LTC2355_CLK_SET();
    DelayHalfus();
	LTC2355_CLK_CLR();
    DelayHalfus();
    LTC2355_CLK_SET();

    for(i=0;i<16;i++)
    {
        DelayHalfus();
        LTC2355_CLK_CLR();
	    DelayHalfus();
        LTC2355_CLK_SET();
        if(LTC2355_DIN_READ() )//无应答
        {
            uch_TempBit = 0;
        }
        else//有应答
        {
            uch_TempBit = 1;
        }
		
        uin_TempByte = (uin_TempByte << 1) +uch_TempBit;  
    }
	uin_RevData = uin_TempByte;

	return (TRUE);            
}
#endif
#if 1
BOOL LTC2355_SPI_SendInt16u(INT16U &uin_RevData)
{
    INT8U i;
    INT8U  uch_TempBit;
    INT16U uin_TempByte = 0;

    LTC2355_CONV_CLR();
	
	//for(i=0;i<6;i++)
	do
	{
	   LTC2355_CLK_CLR();
       DelayHalfus();
       LTC2355_CLK_SET();
	   DelayHalfus();
#if 0	   
	   i++;
	   if(i >12 )
	   {
	      return (FALSE); ;
	   }
#endif	   
	}while(LTC2355_DIN_READ());

    for(i=0;i<16;i++)
    {
        LTC2355_CLK_CLR();
	    DelayHalfus();
        LTC2355_CLK_SET();
        if(LTC2355_DIN_READ() )//无应答
        {
            uch_TempBit = 1;
        }
        else//有应答
        {
            uch_TempBit = 0;
        }
	    DelayHalfus();

        uin_TempByte = (uin_TempByte << 1) +uch_TempBit;
    }
	uin_RevData = uin_TempByte;
    LTC2355_CONV_SET();
	return (TRUE);            
}

#endif
# if 0
//BOOL LTC2355_SPI_SendInt16uContinue(INT16U &uin_RevData)
BOOL LTC2355_SPI_SendInt16u(INT16U &uin_RevData)

{
    INT8U i;
    INT8U  uch_TempBit;
    INT16U uin_TempByte = 0x00;
    LTC2355_CONV_CLR();

    for(i=0;i<16;i++)
    {
        DelayHalfus();
        LTC2355_CLK_CLR();
	    DelayHalfus();
        LTC2355_CLK_SET();
        if(LTC2355_DIN_READ() )//无应答
            uin_TempByte |=(0x01 << i);        
    }
	uin_RevData = uin_TempByte;
 //DelayHalfus();
    return (TRUE);            
}
#endif
BOOL LTC2355_SPI_SendInt16uOne(INT16U &uin_RevData)
{
    INT8U i;
    INT8U  uch_TempBit;
    INT16U uin_TempByte = 0;
	
    LTC2355_CLK_CLR();
    DelayHalfus();
	LTC2355_CLK_SET();
    LTC2355_CONV_SET();
    DelayHalfus();
    LTC2355_CONV_CLR();
	
	while(LTC2355_DIN_READ())
	{
	   LTC2355_CLK_CLR();
       DelayHalfus();
       LTC2355_CLK_SET();
	   i++;
	   if(i> 200)
		 return FALSE;
	}
	
	LTC2355_CLK_CLR();
    DelayHalfus();
    LTC2355_CLK_SET();
    DelayHalfus();
	LTC2355_CLK_CLR();
    DelayHalfus();
    LTC2355_CLK_SET();

    for(i=0;i<16;i++)
    {
        DelayHalfus();
        LTC2355_CLK_CLR();
	    DelayHalfus();
        LTC2355_CLK_SET();
        if(LTC2355_DIN_READ() )//无应答
        {
            uch_TempBit = 0;
        }
        else//有应答
        {
            uch_TempBit = 1;
        }
		
        uin_TempByte = (uin_TempByte << 1) +uch_TempBit;        
    }
	uin_RevData = uin_TempByte;
 //DelayHalfus();
    return (TRUE);            
}

void LTC2355_Selftest(FP32 *uch_RevData)
{
   INT8U i;
   FP32 f_Temp;
   {
       LTC2355_ReadFp32(10,f_Temp);
       //1-6通道电压值
       if(i<6)
       {
          uch_RevData[i] = f_Temp;
       }
       else if(i<7)//7通道电池电压值
       {
          //电池电压值
           uch_RevData[i] = 11*f_Temp;           
       }
       else//8通道热敏电阻温度值
       {
          //温度
           //uch_RevData[i] = f_Temp*8.647;
           //uch_RevData[i] = s_GetTFromSignal(f_Temp);  
       }
   }
   
}
