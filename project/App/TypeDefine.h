
#ifndef TYPESDEFINE_H
#define TYPESDEFINE_H

#include "stm32f4xx.h" 
	 
//??��?2������,����??51����??��?GPIO????1|?��
//??��?����??????,2???<<CM3������t????>>�̨�????(87��3~92��3).M4��?M3����??,??��???��??�¦�??����?��?.
//IO?��2������o��?����?
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO?����??����3��?
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
 
//IO?��2������,????�̣���?��?IO?��!
//��������n��??��D?����16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��?3? 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //��?��? 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��?3? 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //��?��? 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��?3? 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //��?��? 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��?3? 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //��?��? 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��?3? 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //��?��?

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��?3? 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //��?��?

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��?3? 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //��?��?

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //��?3? 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //��?��?

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //��?3? 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //��?��?

//��׼�������Ͷ���
typedef unsigned char       BOOL;     //��������          
typedef unsigned char       INT8U;    //8λ�޷�������           
typedef signed   char       INT8S;    //8λ�з�������                              
typedef unsigned short      INT16U;   //16λ�޷�������                  
typedef signed   short      INT16S;   //16λ�з�������              
typedef unsigned long       INT32U;   //32λ�޷�������                   
typedef signed   long       INT32S;   //32λ�з�������
typedef unsigned long long  INT64U;   //64λ�޷�������                   
typedef signed   long long  INT64S;   //64λ�з�������                
typedef float               FP32;     //32λ������                
typedef double              FP64;     //64λ������   

typedef signed   short      INT16;   //16λ�з�������  
typedef signed   char       INT8;

//��׼�궨��
#ifndef NULL
#define NULL   (void *)0
#endif

#define FALSE   0
#define TRUE    1

#define m_MIN(x,y)    	((x<y) ? (x):(y))
#define m_MAX(x,y)		((x>y) ? (x):(y))

#endif
