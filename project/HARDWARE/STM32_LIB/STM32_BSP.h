#ifndef STM32_BSP_H
#define STM32_BSP_H
     
#define CLI() __set_PRIMASK(1)
#define SEI() __set_PRIMASK(0)

#include "stm32f4xx.h"

#include "stm32_Usart.h"
#include "stm32_Adc.h"
#include "stm32_Tim_PWM.h"
#include "stm32_Can.h"
#include "stm32_Exti.h"
#include "stm32_spi.h"
#include "usbd_core.h"
#include "usbd_hid_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "usb_dcd_int.h"


#include "DAC121.h"
#include "Drv_LTC2355.h"
//#include "Drv_S8377.h"
#include "Drv_AD4004.h"
#include "Drv_S10121.h"
#include "Drv_MT25QL128.h" 

//------------------------------------------用户配置------------------------------------------------
#define LED1_ON()          GPIO_ResetBits(GPIOB,GPIO_Pin_2)       
#define LED1_OFF()         GPIO_SetBits(GPIOB,GPIO_Pin_2)
#define LED1_TOGGLE()      GPIO_ToggleBits(GPIOB,GPIO_Pin_2)

#define LED2_ON()          GPIO_ResetBits(GPIOB,GPIO_Pin_0)       
#define LED2_OFF()         GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define LED2_TOGGLE()      GPIO_ToggleBits(GPIOB,GPIO_Pin_0)

#define LED3_ON()          GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LED3_OFF()         GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define LED3_TOGGLE()      GPIO_ToggleBits(GPIOB,GPIO_Pin_1)


//看门狗
#define WDI_OFF      	GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define WDI_ON       	GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define WDI_TOGGLE()    GPIO_ToggleBits(GPIOC,GPIO_Pin_1)


#define SW1_ON()          GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define SW1_OFF()         GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define SW1_TOGGLE()      GPIO_ToggleBits(GPIOC,GPIO_Pin_3)

#define SW2_ON()          GPIO_SetBits(GPIOC,GPIO_Pin_5)
#define SW2_OFF()         GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#define SW2_TOGGLE()      GPIO_ToggleBits(GPIOC,GPIO_Pin_5)

//------------------------------------------接口声明------------------------------------------------
void BSP_Init(void);
void WDI_Clear(void);
void SysClock_Init(void);

void DelayNms(u32 Nms);

void DelayNus(u32 Nus);

void USART1_Frame(u8 * str,u16 len);

void USART3_Puts(u8 * str,u16 len);
void USART6_Puts(u8 * str,u16 len);


void USART3_IRQHandler(void);

INT32U GetSystermTimerUs(void);
INT32U GetSystermTimerMs(void);

#endif
