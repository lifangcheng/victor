

#ifndef _STM32_CAN_H_
#define _STM32_CAN_H_

void Stm32_Can_Init(void);
void CAN2_TX0_Send(void);

void Can0_PackSend(u8 * str,u16 len);
void Can0_PackIrqSend(void);


#ifdef __cplusplus
 extern "C" {
#endif 
void CAN2_TX_IRQHandler(void);
void CAN2_RX0_IRQHandler(void);
#ifdef __cplusplus
}
#endif

#endif
