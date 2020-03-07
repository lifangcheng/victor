#include "Drv_MT25QL128.h" 
#include "STM32_BSP.h"
#include "Pro_TickTime.h"
#include<string.h>

INT16U W25QXX_TYPE=W25Q128;	//默认是MT25QL128

//4Kbytes为一个Sector
//16个扇区为1个Block
//MT25QL128
//容量为16M字节,共有128个Block,4096个Sector 
													 
//初始化SPI FLASH的IO口
void MT25QL128_Init(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟

  /* Configure PA.4 PA.8 as Output push-pull, used as Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_4 ;                     //选中第4引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                          //最大输出速度50M
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                              //输出模式
  GPIO_Init(GPIOA, &GPIO_InitStructure);                                     //按上述设置初始化引脚

  /* Configure PC.8 as Output push-pull, used as Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                  //选中第4引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                          //最大输出速度50M
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                              //输出模式
  GPIO_Init(GPIOC, &GPIO_InitStructure);                                     //按上述设置初始化引脚
  
  GPIO_SetBits(GPIOA, GPIO_Pin_8);

  MT25QL128_CS=1;			    //SPI FLASH不选中
  W25QXX_TYPE=MT25QL128_ReadID();	//读取FLASH ID.
}  

//读取MT25QL128的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
INT8U MT25QL128_ReadSR(void)   
{  
	INT8U byte=0;   
	MT25QL128_CS=0;                         //使能器件   
	SPI1_Write(MT25QL128_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=SPI1_Write(0Xff);                  //读取一个字节  
	MT25QL128_CS=1;                         //取消片选     
	return byte;   
} 
//写MT25QL128状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void MT25QL128_Write_SR(INT8U sr)   
{   
	MT25QL128_CS=0;                           //使能器件   
	SPI1_Write(MT25QL128_WriteStatusReg);     //发送写取状态寄存器命令    
	SPI1_Write(sr);                           //写入一个字节  
	MT25QL128_CS=1;                           //取消片选     	      
}   
//MT25QL128写使能	
//将WEL置位   
void MT25QL128_Write_Enable(void)   
{
	MT25QL128_CS=0;                            //使能器件   
    SPI1_Write(MT25QL128_WriteEnable);         //发送写使能  
	MT25QL128_CS=1;                            //取消片选     	      
} 
//MT25QL128写禁止	
//将WEL清零  
void MT25QL128_Write_Disable(void)   
{  
	MT25QL128_CS=0;                            //使能器件   
    SPI1_Write(MT25QL128_WriteDisable);        //发送写禁止指令    
	MT25QL128_CS=1;                            //取消片选     	      
} 		
//读取芯片ID
//返回值如下:				   
//0X20BA18,表示芯片型号为W25Q128 	  
INT32U MT25QL128_ReadID(void)
{
    INT32U Temp = 0;	  
	MT25QL128_CS=0;				    
	SPI1_Write(MT25QL128_DeviceID);               //发送读取ID命令
	Temp|=SPI1_Write(0xFF)<<16;  
	Temp|=SPI1_Write(0xFF)<<8;	
	Temp|=SPI1_Write(0xFF);	
	MT25QL128_CS=1;				    
	return Temp;
}   		    
//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void MT25QL128_Read(INT8U* pBuffer,INT32U ReadAddr,INT16U NumByteToRead)   
{ 
 	INT16U i;   		
	static INT8U temp;
	MT25QL128_CS=0;                         //使能器件   
    SPI1_Write(MT25QL128_ReadDataRegister); //发送读取命令   
    SPI1_Write((u8)((ReadAddr)>>16));       //发送24bit地址    
    SPI1_Write((u8)((ReadAddr)>>8));   
    SPI1_Write((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI1_Write(0XFF);        //循环读数  
		temp = pBuffer[i];
		
    }
	MT25QL128_CS=1;  				    	      
}  
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void MT25QL128_Write_Page(INT8U* pBuffer,INT32U WriteAddr,INT16U NumByteToWrite)
{
 	INT16U i;  
    MT25QL128_Write_Enable();                  //SET WEL 
	MT25QL128_CS=0;                            //使能器件   
   // MT25QL128_Wait_Busy(); 
    SPI1_Write(MT25QL128_PageProgram);              //发送写页命令   
    SPI1_Write((u8)((WriteAddr)>>16));         //发送24bit地址    
    SPI1_Write((u8)((WriteAddr)>>8));   
    SPI1_Write((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)
        SPI1_Write(pBuffer[i]);                //循环写数  

    MT25QL128_CS=1;                            //取消片选 
	MT25QL128_Wait_Busy();					   //等待写入结束
} 
//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void MT25QL128_Write_NoCheck(INT8U* pBuffer,INT32U WriteAddr,INT16U NumByteToWrite)   
{ 			 		 
	INT16U pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		MT25QL128_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)  
void MT25QL128_Write(INT8U* pBuffer,INT32U WriteAddr,INT16U NumByteToWrite)   
{ 
	INT32U secpos;
	INT16U secoff;
	INT16U secremain;	   
 	INT16U i;    
    INT8U  MT25QL128_BUF[4096];


 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   

 	if(NumByteToWrite<=secremain)
	  secremain=NumByteToWrite;//不大于4096个字节

	while(1) 
	{	
		MT25QL128_Read(MT25QL128_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(MT25QL128_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			MT25QL128_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				MT25QL128_BUF[i+secoff]=pBuffer[i];	  
			}
			//memcpy(&MT25QL128_BUF[secoff],pBuffer,NumByteToWrite);
			MT25QL128_Write_NoCheck(MT25QL128_BUF,secpos*4096,4096);//写入整个扇区  

		}else MT25QL128_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
		    secpos++;//扇区地址增1
		    secoff=0;//偏移位置为0 	 

		    pBuffer+=secremain;  //指针偏移
		    WriteAddr+=secremain;//写地址偏移	   
		    NumByteToWrite-=secremain;				//字节数递减
		    if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
		    else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 
}
//擦除整个芯片		  
//等待时间超长...
void MT25QL128_Erase_Chip(void)   
{                                   
    MT25QL128_Write_Enable();                 //SET WEL 
    MT25QL128_Wait_Busy();   
    MT25QL128_CS=0;                          //使能器件   
    SPI1_Write(MT25QL128_ChipErase);        //发送片擦除命令  
    MT25QL128_CS=1;                         //取消片选     	      
    MT25QL128_Wait_Busy();   		   //等待芯片擦除结束
}   
//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void MT25QL128_Erase_Sector(INT32U Dst_Addr)   
{  
    //监视falsh擦除情况,测试用   
    // 	printf("fe:%x\r\n",Dst_Addr);	  
    Dst_Addr*=4096;
    MT25QL128_Write_Enable();                  //SET WEL 	 
    MT25QL128_Wait_Busy();   
    MT25QL128_CS=0;                            //使能器件   
    SPI1_Write(MT25QL128_SectorErase);         //发送扇区擦除指令 
    SPI1_Write((u8)((Dst_Addr)>>16));          //发送24bit地址    
    SPI1_Write((u8)((Dst_Addr)>>8));   
    SPI1_Write((u8)Dst_Addr);  
    MT25QL128_CS=1;                            //取消片选     	      
    MT25QL128_Wait_Busy();   		       //等待擦除完成
}  
//等待空闲
void MT25QL128_Wait_Busy(void)   
{   
    while((MT25QL128_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  
//进入掉电模式
void MT25QL128_PowerDown(void)   
{ 
    MT25QL128_CS=0;                            //使能器件   
    SPI1_Write(MT25QL128_PowerDownRegister);   //发送掉电命令  
    MT25QL128_CS=1;                            //取消片选     	      
    TickDelayUs(3);                            //等待TPD  
}   
//唤醒
void MT25QL128_WAKEUP(void)   
{  
    MT25QL128_CS=0;                            //使能器件   
    SPI1_Write(MT25QL128_ReleasePowerDown);    //  send W25X_PowerDown command 0xAB    
    MT25QL128_CS=1;                            //取消片选     	      
    TickDelayUs(3);                            //等待TRES1
}
