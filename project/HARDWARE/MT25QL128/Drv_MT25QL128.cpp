#include "Drv_MT25QL128.h" 
#include "STM32_BSP.h"
#include "Pro_TickTime.h"
#include<string.h>

INT16U W25QXX_TYPE=W25Q128;	//Ĭ����MT25QL128

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//MT25QL128
//����Ϊ16M�ֽ�,����128��Block,4096��Sector 
													 
//��ʼ��SPI FLASH��IO��
void MT25QL128_Init(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��

  /* Configure PA.4 PA.8 as Output push-pull, used as Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_4 ;                     //ѡ�е�4����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                          //�������ٶ�50M
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                              //���ģʽ
  GPIO_Init(GPIOA, &GPIO_InitStructure);                                     //���������ó�ʼ������

  /* Configure PC.8 as Output push-pull, used as Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                                  //ѡ�е�4����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                          //�������ٶ�50M
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                              //���ģʽ
  GPIO_Init(GPIOC, &GPIO_InitStructure);                                     //���������ó�ʼ������
  
  GPIO_SetBits(GPIOA, GPIO_Pin_8);

  MT25QL128_CS=1;			    //SPI FLASH��ѡ��
  W25QXX_TYPE=MT25QL128_ReadID();	//��ȡFLASH ID.
}  

//��ȡMT25QL128��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
INT8U MT25QL128_ReadSR(void)   
{  
	INT8U byte=0;   
	MT25QL128_CS=0;                         //ʹ������   
	SPI1_Write(MT25QL128_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_Write(0Xff);                  //��ȡһ���ֽ�  
	MT25QL128_CS=1;                         //ȡ��Ƭѡ     
	return byte;   
} 
//дMT25QL128״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void MT25QL128_Write_SR(INT8U sr)   
{   
	MT25QL128_CS=0;                           //ʹ������   
	SPI1_Write(MT25QL128_WriteStatusReg);     //����дȡ״̬�Ĵ�������    
	SPI1_Write(sr);                           //д��һ���ֽ�  
	MT25QL128_CS=1;                           //ȡ��Ƭѡ     	      
}   
//MT25QL128дʹ��	
//��WEL��λ   
void MT25QL128_Write_Enable(void)   
{
	MT25QL128_CS=0;                            //ʹ������   
    SPI1_Write(MT25QL128_WriteEnable);         //����дʹ��  
	MT25QL128_CS=1;                            //ȡ��Ƭѡ     	      
} 
//MT25QL128д��ֹ	
//��WEL����  
void MT25QL128_Write_Disable(void)   
{  
	MT25QL128_CS=0;                            //ʹ������   
    SPI1_Write(MT25QL128_WriteDisable);        //����д��ָֹ��    
	MT25QL128_CS=1;                            //ȡ��Ƭѡ     	      
} 		
//��ȡоƬID
//����ֵ����:				   
//0X20BA18,��ʾоƬ�ͺ�ΪW25Q128 	  
INT32U MT25QL128_ReadID(void)
{
    INT32U Temp = 0;	  
	MT25QL128_CS=0;				    
	SPI1_Write(MT25QL128_DeviceID);               //���Ͷ�ȡID����
	Temp|=SPI1_Write(0xFF)<<16;  
	Temp|=SPI1_Write(0xFF)<<8;	
	Temp|=SPI1_Write(0xFF);	
	MT25QL128_CS=1;				    
	return Temp;
}   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void MT25QL128_Read(INT8U* pBuffer,INT32U ReadAddr,INT16U NumByteToRead)   
{ 
 	INT16U i;   		
	static INT8U temp;
	MT25QL128_CS=0;                         //ʹ������   
    SPI1_Write(MT25QL128_ReadDataRegister); //���Ͷ�ȡ����   
    SPI1_Write((u8)((ReadAddr)>>16));       //����24bit��ַ    
    SPI1_Write((u8)((ReadAddr)>>8));   
    SPI1_Write((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI1_Write(0XFF);        //ѭ������  
		temp = pBuffer[i];
		
    }
	MT25QL128_CS=1;  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void MT25QL128_Write_Page(INT8U* pBuffer,INT32U WriteAddr,INT16U NumByteToWrite)
{
 	INT16U i;  
    MT25QL128_Write_Enable();                  //SET WEL 
	MT25QL128_CS=0;                            //ʹ������   
   // MT25QL128_Wait_Busy(); 
    SPI1_Write(MT25QL128_PageProgram);              //����дҳ����   
    SPI1_Write((u8)((WriteAddr)>>16));         //����24bit��ַ    
    SPI1_Write((u8)((WriteAddr)>>8));   
    SPI1_Write((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)
        SPI1_Write(pBuffer[i]);                //ѭ��д��  

    MT25QL128_CS=1;                            //ȡ��Ƭѡ 
	MT25QL128_Wait_Busy();					   //�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void MT25QL128_Write_NoCheck(INT8U* pBuffer,INT32U WriteAddr,INT16U NumByteToWrite)   
{ 			 		 
	INT16U pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		MT25QL128_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)  
void MT25QL128_Write(INT8U* pBuffer,INT32U WriteAddr,INT16U NumByteToWrite)   
{ 
	INT32U secpos;
	INT16U secoff;
	INT16U secremain;	   
 	INT16U i;    
    INT8U  MT25QL128_BUF[4096];


 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   

 	if(NumByteToWrite<=secremain)
	  secremain=NumByteToWrite;//������4096���ֽ�

	while(1) 
	{	
		MT25QL128_Read(MT25QL128_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(MT25QL128_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			MT25QL128_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				MT25QL128_BUF[i+secoff]=pBuffer[i];	  
			}
			//memcpy(&MT25QL128_BUF[secoff],pBuffer,NumByteToWrite);
			MT25QL128_Write_NoCheck(MT25QL128_BUF,secpos*4096,4096);//д����������  

		}else MT25QL128_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
		    secpos++;//������ַ��1
		    secoff=0;//ƫ��λ��Ϊ0 	 

		    pBuffer+=secremain;  //ָ��ƫ��
		    WriteAddr+=secremain;//д��ַƫ��	   
		    NumByteToWrite-=secremain;				//�ֽ����ݼ�
		    if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
		    else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void MT25QL128_Erase_Chip(void)   
{                                   
    MT25QL128_Write_Enable();                 //SET WEL 
    MT25QL128_Wait_Busy();   
    MT25QL128_CS=0;                          //ʹ������   
    SPI1_Write(MT25QL128_ChipErase);        //����Ƭ��������  
    MT25QL128_CS=1;                         //ȡ��Ƭѡ     	      
    MT25QL128_Wait_Busy();   		   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void MT25QL128_Erase_Sector(INT32U Dst_Addr)   
{  
    //����falsh�������,������   
    // 	printf("fe:%x\r\n",Dst_Addr);	  
    Dst_Addr*=4096;
    MT25QL128_Write_Enable();                  //SET WEL 	 
    MT25QL128_Wait_Busy();   
    MT25QL128_CS=0;                            //ʹ������   
    SPI1_Write(MT25QL128_SectorErase);         //������������ָ�� 
    SPI1_Write((u8)((Dst_Addr)>>16));          //����24bit��ַ    
    SPI1_Write((u8)((Dst_Addr)>>8));   
    SPI1_Write((u8)Dst_Addr);  
    MT25QL128_CS=1;                            //ȡ��Ƭѡ     	      
    MT25QL128_Wait_Busy();   		       //�ȴ��������
}  
//�ȴ�����
void MT25QL128_Wait_Busy(void)   
{   
    while((MT25QL128_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void MT25QL128_PowerDown(void)   
{ 
    MT25QL128_CS=0;                            //ʹ������   
    SPI1_Write(MT25QL128_PowerDownRegister);   //���͵�������  
    MT25QL128_CS=1;                            //ȡ��Ƭѡ     	      
    TickDelayUs(3);                            //�ȴ�TPD  
}   
//����
void MT25QL128_WAKEUP(void)   
{  
    MT25QL128_CS=0;                            //ʹ������   
    SPI1_Write(MT25QL128_ReleasePowerDown);    //  send W25X_PowerDown command 0xAB    
    MT25QL128_CS=1;                            //ȡ��Ƭѡ     	      
    TickDelayUs(3);                            //�ȴ�TRES1
}
