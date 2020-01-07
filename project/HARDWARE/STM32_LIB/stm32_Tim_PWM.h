
#ifndef STM32_TIM_H
#define STM32_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "TypeDefine.h"

#define PWM_CHL1 1
#define PWM_CHL2 2
#define PWM_CHL3 3
#define PWM_CHL4 4
#define PWM_CHL5 5
#define PWM_CHL6 6
#define PWM_CHL7 7
#define PWM_CHL8 8
#define PWM_CHL9 9
#define PWM_CHL10 10
#define PWM_CHL11 11
#define PWM_CHL12 12
#define PWM_CHL13 13
#define PWM_CHL14 14




void STM32_Tim_PWM_Init(void);
void SetPWM_Duty(INT8U chl,FP32 duty);
void SetTimPWM_Period(INT8U chl,INT32U period);

#ifdef __cplusplus
}
#endif
	 
#endif
