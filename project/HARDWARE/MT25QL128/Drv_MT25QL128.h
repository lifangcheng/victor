#ifndef __DRV_MT25QL128_H__
#define __DRV_MT25QL128_H__		

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "stm32f4xx.h"
#include "TypeDefine.h"


#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

extern u16 W25QXX_TYPE;					//定义W25QXX芯片型号		   

#define	MT25QL128_CS 		PCout(5)  		//W25QXX的片选信号

////////////////////////////////////////////////////////////////////////////////// 
//指令表
#define MT25QL128_WriteEnable		0x06 
#define MT25QL128_WriteDisable		0x04 
#define MT25QL128_ReadStatusReg		0x05 
#define MT25QL128_WriteStatusReg	0x01 
#define MT25QL128_ReadDataRegister	0x03 
#define MT25QL128_FastReadData		0x0B 
#define MT25QL128_FastReadDual		0x3B 
#define MT25QL128_PageProgram		0x02 
#define MT25QL128_BlockErase        0xD8 
#define MT25QL128_SectorErase		0x20 
#define MT25QL128_ChipErase			0xC7 
#define MT25QL128_PowerDownRegister	0xB9 
#define MT25QL128_ReleasePowerDown	0xAB 
#define MT25QL128_DeviceID			0x9F 
#define MT25QL128_ManufactDeviceID  0xAF 
#define MT25QL128_JedecDeviceID		0x5A 

void MT25QL128_Init(void);
INT32U  MT25QL128_ReadID(void);  	    		//读取FLASH ID
INT8U	MT25QL128_ReadSR(void);        		//读取状态寄存器 
void MT25QL128_Write_SR(INT8U sr);  			//写状态寄存器
void MT25QL128_Write_Enable(void);  		//写使能 
void MT25QL128_Write_Disable(void);		    //写保护
void MT25QL128_Write_NoCheck(INT8U* pBuffer,INT32U WriteAddr,INT16U NumByteToWrite);
void MT25QL128_Read(INT8U* pBuffer,INT32U ReadAddr,INT16U NumByteToRead);   //读取flash
void MT25QL128_Write(INT8U* pBuffer,INT32U WriteAddr,INT16U NumByteToWrite);//写入flash
void MT25QL128_Erase_Chip(void);    	  	//整片擦除
void MT25QL128_Erase_Sector(INT32U Dst_Addr);	//扇区擦除
void MT25QL128_Wait_Busy(void);           	//等待空闲
void MT25QL128_PowerDown(void);          	//进入掉电模式
void MT25QL128_WAKEUP(void);				//唤醒
void MT25QL128_Write_Page(INT8U* pBuffer,INT32U WriteAddr,INT16U NumByteToWrite);


#ifdef __cplusplus
}
#endif

#endif
















