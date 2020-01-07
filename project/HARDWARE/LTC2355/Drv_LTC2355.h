#ifndef DRV_LTC2355_H
#define DRV_LTC2355_H

#include "stm32f4xx.h"
#include "TypeDefine.h"


#define LTC2355_CONV_SET()		(GPIOB->BSRRL=GPIO_Pin_6)
#define LTC2355_CONV_CLR()		(GPIOB->BSRRH=GPIO_Pin_6)

#define LTC2355_CLK_SET()	(GPIOB->BSRRL=GPIO_Pin_3)
#define LTC2355_CLK_CLR()	(GPIOB->BSRRH=GPIO_Pin_3)

#define LTC2355_DOUT_SET()	(GPIOB->BSRRL=GPIO_Pin_5)
#define LTC2355_DOUT_CLR()	(GPIOB->BSRRH=GPIO_Pin_5)

#define LTC2355_DIN_READ()	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)


#define LTC2355_REF1  4.096

void LTC2355Init(void);
BOOL LTC2355_ReadInt16u_One(INT16U &uin_RevData);
BOOL LTC2355_ReadInt16u(INT16U uin_AveTimes,INT16U *uin_RevData);
BOOL LTC2355_ReadFp32(FP32 &ul_RevData);    
BOOL LTC2355_ReadFp32(INT16U uin_AveTimes,FP32 &ul_RevData);
void LTC2355_Selftest(FP32 *uch_RevData);
BOOL LTC2355_SPI_SendInt16u(INT16U &uin_RevData);


#endif
