#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32_Bsp.h"

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

static void Bsp_PllInit(void);
static void BSP_GpioInit(void); 
static void Bsp_NVICInit(void);


//==================================================================================================
//| 函数名称 | BSP_Init()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 系统初始化函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void BSP_Init(void)
{
    Bsp_PllInit();
    BSP_GpioInit();
    Bsp_NVICInit();
    Bsp_UartInit();
	Bsp_AdcInit();
	SPI1_Init(SPI0_BAUDRATE_1);
	SPI3_Init(SPI0_BAUDRATE_1);
	SysClock_Init();
	
    MT25QL128_Init();
	STM32_Tim_PWM_Init();
	//S8377_init();
    //LTC2355Init();
    AD4004Init();
    S10121_init();
	DAC121S101_Init();
#if 0
	  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc, 
            &USBD_HID_cb, 
            &USR_cb);
#endif
}

//==================================================================================================
//| 函数名称 | BSP_PllInit()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | PLL倍频设置
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void Bsp_PllInit(void)
{
	//使能各模块时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
	                       RCC_AHB1Periph_GPIOB |
						   RCC_AHB1Periph_GPIOC |
						   RCC_AHB1Periph_GPIOD |
						   RCC_AHB1Periph_GPIOE |
						   RCC_AHB1Periph_GPIOF |
						   RCC_AHB1Periph_GPIOG ,
						   ENABLE);

}


//==================================================================================================
//| 函数名称 | SysClock_Init
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 |SysClock初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================  
void SysClock_Init(void)
{
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,
	                       ENABLE);	
	SysTick_Config(SystemCoreClock / 100);
}



INT32U ul_SystermTimerUs = 0;
INT32U ul_SystermTimerMs = 0;

INT32U GetSystermTimerUs(void)
{
    volatile INT32U ul_Temp,ul_Temp1;
    
    ul_Temp = SysTick->LOAD;
	ul_Temp1 = SysTick->VAL;

    //DisableInterrupts;
    ul_Temp =  ul_SystermTimerUs + (ul_Temp/168);         //168MHz,8分频
    //EnableInterrupts;
    
    return(ul_Temp);
}
extern INT32U gul_Tick1,gul_Tick2,gul_Tick3,gul_Tick4;
INT32U GetSystermTimerMs(void)
{
    volatile INT32U ul_Temp,ul_Temp1;
	INT32U ul_Temp2;
    
    ul_Temp = SysTick->LOAD;
	ul_Temp1 = SysTick->VAL;
	
	gul_Tick1 = ul_Temp;
	gul_Tick2 = ul_Temp1;
	
	ul_Temp2 = (ul_Temp - ul_Temp1);//zhliu
    //DisableInterrupts;
    ul_Temp =  ul_SystermTimerMs + ((ul_Temp2/168))/1000; //168MHz,8分频
    //EnableInterrupts;
	
	gul_Tick3 = ul_Temp2;
	gul_Tick4 = ul_Temp;
    
    return(ul_Temp);
}



//==================================================================================================
//| 函数名称 | DelayNus
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 延时n微秒
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//==================================================================================================  
void DelayNus(u32 Nus)
{
	u32 i,j;
	for(j = 0;j < Nus;j++)
	{ 
    	for(i=0;i<28;i++); 
	}
}
//==================================================================================================
//| 函数名称 | DelayNms
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 延时n毫秒
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================  
void DelayNms(u32 Nms)
{
	do
	{
		DelayNus(1000);
		//WDI_Clear();
        WDI_TOGGLE();
	}
	while(Nms--);
}

//==================================================================================================
//| 函数名称 | BSP_GpioInit()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | Gpio初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void BSP_GpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 

    // Configure SW1 2 3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// Configure LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// Configure WDI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
    // Configure test
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
}


//==================================================================================================
//| 函数名称 | Bsp_NVICInit()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 中断初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void Bsp_NVICInit(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    // Enable the USART2 Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
    // Enable the USART3 Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);

    // Enable the USART6 Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}

//==================================================================================================
//| 函数名称 | DelayNms
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 延时n毫秒
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================  
void WDI_Clear(void)
{
	WDI_OFF;
	DelayNus(5);
	WDI_ON;
	DelayNus(5);
	WDI_OFF;
}
