#include "stm32f4xx.h"
#include "stm32_Adc.h"
#include "typedefine.h"

#define ADCVREF 2.5

//==================================================================================================
//| 函数名称 | Bsp_AdcInit()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | Adc初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void Bsp_AdcInit(void)
{
    
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;
    
    /* Enable peripheral clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | 
                                               RCC_APB2Periph_ADC3, ENABLE);	
   
   
    /* Configure ADC Channe4 pin as analog input */ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    /******************************************************************************/
    /*  ADCs configuration: triple interleaved with 5cycles delay to reach 6Msps  */
    /******************************************************************************/
	
    /* ADC Common configuration *************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; 
    ADC_CommonInit(&ADC_CommonInitStructure);	
	
    /* ADC1 regular channel 1 configuration ************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_3Cycles);

	
    /* Enable ADC1 **************************************************************/
    ADC_Cmd(ADC1, ENABLE);
	
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConv(ADC1);
		
}

//==================================================================================================
//| 函数名称 | Get_AdcValue()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | Adc初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================


u16 Get_AdcValue(INT8U ADC_Channel)
{
    u8 uch_status = 0;//0xff;
    u16 uin_Value = 0;  
    for(INT8U i=0;i<3;i++)
    {
        ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_15Cycles);
   
        ADC_SoftwareStartConv(ADC1);	
        
        while(!uch_status)
        {
            uch_status = ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC);	
        }
        //DelayNus(5);
        uin_Value = ADC_GetConversionValue(ADC1);
    }
    
    return uin_Value;	
}

//==================================================================================================
//| 函数名称 | Get_AdcValue()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | Adc初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================


FP32 GetAdcVol(INT8U Channel)
{
    u16 uin_Value;
	FP32 f_Temp;
	switch(Channel)
	{
        case ADC_Channel_14:
			uin_Value = Get_AdcValue(ADC_Channel_14);
			f_Temp =(FP32) (uin_Value / 4096 * ADCVREF * 200.0);  //转换为实际电压
			break;
	}
	return f_Temp;
}

