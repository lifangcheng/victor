#ifndef STM32_USART_H
#define STM32_USART_H

#include "stm32f4xx.h"
//------------------------------------------用户配置------------------------------------------------

//------------------------------------------接口声明------------------------------------------------
void Bsp_UartInit(void);
void Uart2_Init(u8 uch_baud);
void Uart3_Init(u8 uch_baud);
void Uart6_Init(u8 uch_baud);

void USART2_Puts(u8 * str,u16 len);
void USART3_Puts(u8 * str,u16 len);
void USART6_Puts(u8 * str,u16 len);

void USART2_Pack_Send(u8 * str,u16 len);
void USART3_Pack_Send(u8 * str,u16 len);
void USART6_Pack_Send(u8 * str,u16 len);

#ifdef __cplusplus
 extern "C" {
#endif 

void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void USART6_IRQHandler(void);
	 
#ifdef __cplusplus
}
#endif

#endif
