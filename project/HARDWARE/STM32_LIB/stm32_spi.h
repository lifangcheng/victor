#ifndef STM32_SPI_H
#define STM32_SPI_H

#include "stm32f4xx.h"
#include "TypeDefine.h"

/* SPI总线波特率设置*/
#define SPI0_BAUDRATE_1   0U
#define SPI0_BAUDRATE_2   1U
#define SPI0_BAUDRATE_3   2U
#define SPI0_BAUDRATE_4  3U
#define SPI0_BAUDRATE_5   4U
#define SPI0_BAUDRATE_6   5U
#define SPI0_BAUDRATE_7   6U
#define SPI0_BAUDRATE_8   7U
//	


#define DUMMY_BYTE  0XFF

extern BOOL  SPI1_Init(INT8U uch_Baudrate);
extern INT8U SPI1_Write(INT8U byte);
extern INT8U SPI1_Read(void);


extern BOOL SPI3_Init(INT8U uch_Baudrate);
extern INT8U SPI3_Write(INT8U byte);
extern void  STM32SPI2_SndData16(INT16U send);

extern INT8U SPI3_Read(void);


#endif
