#ifndef __DAC121_H__
#define __DAC121_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f4xx.h"
#include "STM32_BSP.h"

#define VOLTAGE_REF 4.096
#define RATIO 4095
   
#define VOLTAGE_GAIN 1.2*166

#define DAC121_CS_H           GPIOC->BSRRL=GPIO_Pin_9;
#define DAC121_CS_L           GPIOC->BSRRH=GPIO_Pin_9;

#define DAC121_CLK_H          GPIOC->BSRRL=GPIO_Pin_10;
#define DAC121_CLK_L          GPIOC->BSRRH=GPIO_Pin_10; 

#define DAC121_DIN_H          GPIOC->BSRRL=GPIO_Pin_12;
#define DAC121_DIN_L          GPIOC->BSRRH=GPIO_Pin_12;

void DAC121S101_Init(void);
void DAC121S101_Write(INT16U uin_Data);
void SetDA_Voltage(FP32 f_Voltage);
void SetDA_HighVoltage(FP32 f_Voltage);

#ifdef __cplusplus
}
#endif

#endif
