#include "stm32f4xx.h"
#include "stm32_Tim_PWM.h"
#include "TypeDefine.h"

INT32U gl_Tim8Period;

INT32U gl_Tim3Period;
INT32U gl_Tim4Period;
INT32U gl_Tim5Period;

INT32U gl_Tim9Period;
INT32U gl_Tim1Period;

//==================================================================================================
//| 函数名称 | STM32_Tim_PWM_Init()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | Adc初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void STM32_Tim_PWM_Init(void)
{
	INT32U PrescalerValue,Period;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	
#if 0
	/////////////////////////  TIM3   //////////////////////
	// TIM3 clock enable 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//PB4 PB5->TIM3_CH1 CH3 CH4 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	// Connect TIM3 pins to AF2  
	//GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	
	//要TIM1_CLK为28MHZ，则TIM1_PSC分频值如下计算
	//Prescaler = ((SystemCoreClock /2) /TIM1_CLK) - 1
	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 20000000) - 1;

	//周期(频率)计算,PWM_CLK(PWM输出)为10KHZ
	//period(ARR) = (TIM1_CLK / PWM_CLK) - 1
	Period = (20000000/30000) - 1;
	gl_Tim3Period = Period+1;
	
	//TIM配置
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//TIM1_CH1->PWM1 Mode configuration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OCInitStructure.TIM_Pulse = 0;//20;//0;//Period/2;  //初始占空比为50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

//	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
//	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_Cmd(TIM3, ENABLE);


	/////////////////////////  TIM4   //////////////////////
	// TIM4 clock enable 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//PB6 PB7 PB8 PB9->TIM4_CH1 CH2 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

	// Connect TIM4 pins to AF2  
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	
	//要TIM1_CLK为28MHZ，则TIM1_PSC分频值如下计算
	//Prescaler = ((SystemCoreClock /2) /TIM1_CLK) - 1
//	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 20000000) - 1;
    PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 20000) - 1;
	//周期(频率)计算,PWM_CLK(PWM输出)为10KHZ
	//period(ARR) = (TIM1_CLK / PWM_CLK) - 1
//	Period = (20000000/10) - 1;//Period = (28000000/1200) - 1;
    Period = (20000/10) - 1;
	gl_Tim4Period = Period+1;
	
	//TIM配置
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	//TIM1_CH1->PWM1 Mode configuration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OCInitStructure.TIM_Pulse = 0;  //初始占空比为50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
//	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
//	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	// TIM1 enable counter 
	TIM_Cmd(TIM4, ENABLE);



/////////////////////////  TIM8   //////////////////////
	// TIM8 clock enable 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	//PB6 PB7 PB8 PB9->TIM4_CH1 CH2 CH3 CH4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	// Connect TIM4 pins to AF2  
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);
        
	//要TIM1_CLK为28MHZ，则TIM1_PSC分频值如下计算
	//Prescaler = ((SystemCoreClock /2) /TIM1_CLK) - 1
	PrescalerValue = (uint16_t) ((SystemCoreClock ) / 20000000) - 1;

	//周期(频率)计算,PWM_CLK(PWM输出)为10KHZ
	//period(ARR) = (TIM1_CLK / PWM_CLK) - 1
	Period = (20000000/1000) - 1;//Period = (28000000/1200) - 1;
	gl_Tim8Period = Period+1;
	
	//TIM配置
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	//TIM1_CH1->PWM1 Mode configuration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OCInitStructure.TIM_Pulse = 0;  //初始占空比为50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC4Init(TIM8, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM8, ENABLE);

	// TIM1 enable counter 
	TIM_Cmd(TIM8, ENABLE);   
    TIM_CtrlPWMOutputs(TIM8, ENABLE);
#endif    
/////////////////////////  TIM12   //////////////////////
	// TIM12 clock enable 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

	//PB14->TIM12_CH1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	// Connect TIM4 pins to AF2  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);

        
	//要TIM1_CLK为28MHZ，则TIM1_PSC分频值如下计算
	//Prescaler = ((SystemCoreClock /2) /TIM1_CLK) - 1
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 20000000)-1;//(uint16_t) ((SystemCoreClock /2) / 2800000/2) - 1;

	//周期(频率)计算,PWM_CLK(PWM输出)为10KHZ
	//period(ARR) = (TIM1_CLK / PWM_CLK) - 1
	Period = 20000000 / 10000;//(26666666/600) - 1;//Period = (28000000/1200) - 1;
        //Period = (28000000/1200) - 1;
	gl_Tim9Period = Period+1;
	
	//TIM配置
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);

	//TIM12_CH1->PWM1 Mode configuration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OCInitStructure.TIM_Pulse = Period;  //初始占空比为50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	TIM_OC1Init(TIM12, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM12, ENABLE);

	// TIM12 enable counter 
	TIM_Cmd(TIM12, ENABLE);

#if 0
/////////////////////////  TIM1   //////////////////////
	// TIM1 clock enable 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	//PB6 PB7 PB8 PB9->TIM4_CH1 CH2 CH3 CH4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	// Connect TIM1 pins to B14  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM1);

	//要TIM1_CLK为32MHZ，则TIM1_PSC分频值如下计算
	//Prescaler = ((SystemCoreClock /2) /TIM1_CLK) - 1
	PrescalerValue = (uint16_t) ((SystemCoreClock ) / 168000000) - 1;

	//周期(频率)计算,PWM_CLK(PWM输出)为10KHZ
	//period(ARR) = (TIM1_CLK / PWM_CLK) - 1
	Period = (168000000/10000) - 1;
	gl_Tim1Period = Period+1;
	
	//TIM配置
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	//TIM1_CH2N->PWM1 Mode configuration
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;  //初始占空比为50%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, ENABLE);

	// TIM1 enable counter 
	TIM_Cmd(TIM1, ENABLE);	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);	
#endif
}

//==================================================================================================
//| 函数名称 | SetPWM_Duty()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 设置占空比
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 通道号:chl，占空比:duty
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================
void SetPWM_Duty(INT8U chl,FP32 f_duty)
{
	INT32U ul_CMP = 0;
    FP32 duty = 0;

	duty = f_duty; 
	switch(chl)
	{
	case PWM_CHL1:                     //pwm1 Time12 ch1
     
		ul_CMP = (INT32U)(gl_Tim1Period*duty);
		TIM_SetCompare1(TIM12,ul_CMP);break;
	case PWM_CHL2:                    //pwm2 Time1 ch4
      
		ul_CMP = (INT32U)(gl_Tim1Period*duty);
		TIM_SetCompare4(TIM1,ul_CMP);break;
	case PWM_CHL3:                    //pwm3 Time1 chi
      
		ul_CMP = (INT32U)(gl_Tim1Period*duty);//配置为IO管脚 控制电源
		TIM_SetCompare1(TIM1,ul_CMP);
                break;
	case PWM_CHL4:                    //pwm4 Time1 ch2
       
		ul_CMP = (INT32U)(gl_Tim1Period*duty);
		TIM_SetCompare2(TIM1,ul_CMP);
		break;
	case PWM_CHL5:                   //pwm5 Time3 ch1
       
		ul_CMP = (INT32U)(gl_Tim3Period*duty);
		TIM_SetCompare1(TIM3,ul_CMP);break;
	case PWM_CHL6:                   //pwm6 Time8 ch4
       
		ul_CMP = (INT32U)(gl_Tim8Period*duty);  //功率限制
		TIM_SetCompare4(TIM8,ul_CMP);break;
	case PWM_CHL7:                   //pwm7 Time4 ch1
       
		ul_CMP = (INT32U)(gl_Tim4Period*duty);  //功率限制
		TIM_SetCompare1(TIM4,ul_CMP);break;
	case PWM_CHL8:                   //pwm8 Time4 ch2
        //氧化炉
		ul_CMP = (INT32U)(gl_Tim4Period*duty);  //功率限制
		TIM_SetCompare2(TIM4,ul_CMP);break;
	default:
		break;
	}
}




