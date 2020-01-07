
#ifndef TYPESDEFINE_H
#define TYPESDEFINE_H

#include "stm32f4xx.h" 
	 
//??′?2ù×÷,êμ??51àà??μ?GPIO????1|?ü
//??ì?êμ??????,2???<<CM3è¨ít????>>μú????(87ò3~92ò3).M4í?M3àà??,??ê???′??÷μ??·±?á?.
//IO?ú2ù×÷oê?¨ò?
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO?úμ??·ó3é?
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO?ú2ù×÷,????μ￥ò?μ?IO?ú!
//è·±￡nμ??μD?óú16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //ê?3? 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //ê?è? 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //ê?3? 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //ê?è? 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //ê?3? 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //ê?è? 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //ê?3? 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //ê?è? 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //ê?3? 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //ê?è?

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //ê?3? 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //ê?è?

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //ê?3? 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //ê?è?

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //ê?3? 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //ê?è?

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //ê?3? 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //ê?è?

//标准数据类型定义
typedef unsigned char       BOOL;     //布尔类型          
typedef unsigned char       INT8U;    //8位无符合整型           
typedef signed   char       INT8S;    //8位有符合整型                              
typedef unsigned short      INT16U;   //16位无符合整型                  
typedef signed   short      INT16S;   //16位有符合整型              
typedef unsigned long       INT32U;   //32位无符合整型                   
typedef signed   long       INT32S;   //32位有符合整型
typedef unsigned long long  INT64U;   //64位无符合整型                   
typedef signed   long long  INT64S;   //64位有符合整型                
typedef float               FP32;     //32位浮点型                
typedef double              FP64;     //64位浮点型   

typedef signed   short      INT16;   //16位有符合整型  
typedef signed   char       INT8;

//标准宏定义
#ifndef NULL
#define NULL   (void *)0
#endif

#define FALSE   0
#define TRUE    1

#define m_MIN(x,y)    	((x<y) ? (x):(y))
#define m_MAX(x,y)		((x>y) ? (x):(y))

#endif
