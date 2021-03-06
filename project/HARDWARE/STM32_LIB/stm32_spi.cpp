#include "stm32_spi.h"

const INT16U auin_Baudratediv[8]  =  {
                                      SPI_BaudRatePrescaler_2,SPI_BaudRatePrescaler_4,
                                      SPI_BaudRatePrescaler_8,SPI_BaudRatePrescaler_16,
                                      SPI_BaudRatePrescaler_32,SPI_BaudRatePrescaler_64,
                                      SPI_BaudRatePrescaler_128,SPI_BaudRatePrescaler_256
                                      };
//==================================================================================================
//| 函数名称 | SPI1_Init()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | SPI1初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | uch_Baudrate,波特率
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 是否成功,1,成功,0,失败
//|----------|-------------------------------------------------------------------------------------- 
//==================================================================================================
BOOL SPI1_Init(INT8U uch_Baudrate)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if(uch_Baudrate > 7)
    {
        return FALSE;
    }

    /* Enable SPI1 and GPIOA clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);//使能SPI1、GPIOA外设时钟
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);                     //使能GPIOC外设时钟

    /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;        //选中567三个引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                          //最大输出速度50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                               //复用开漏输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                     //按上述设置初始化引脚
    
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1


    /* SPI1 configuration */ 
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;         //双向双线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                              //主SPI模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                          //8位数据
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                                //时钟极性
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                               //相位
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                  //NSS信号由软件(SSI位)控制
    SPI_InitStructure.SPI_BaudRatePrescaler = auin_Baudratediv[uch_Baudrate];  //波特率
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                         //数据从MSB位开始传输
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);                                        //初始化SPI口
    
    /* Enable SPI1 NSS output for master mode */
   // SPI_SSOutputCmd(SPI1, DISABLE);                                            //关闭SPI的SS输出                                           
    /* Enable SPI1  */
    SPI_Cmd(SPI1, ENABLE);                                                     //使能SPI口

    SPI1_Write(0xff);                                                  //启动传输 
    
    return TRUE;
}

//==================================================================================================
//| 函数名称 | SPI1_Init()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | SPI1初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | uch_Baudrate,波特率
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 是否成功,1,成功,0,失败
//|----------|-------------------------------------------------------------------------------------- 
//==================================================================================================
BOOL SPI3_Init(INT8U uch_Baudrate)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if(uch_Baudrate > 7)
    {
        return FALSE;
    }

    /* Enable SPI1 and GPIOA clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 , ENABLE);//使能SPI3、GPIOB外设时钟
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);                     //使能GPIOB外设时钟

    /* Configure SPI1 pins: NSS, SCK, MISO and MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3;        //选中3457三个引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                          //最大输出速度50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                               //复用开漏输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                     //按上述设置初始化引脚
    
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);//复位SPI1
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);//停止复位SPI1


    /* SPI1 configuration */ 
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;         //双向双线全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                              //主SPI模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                          //8位数据
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                                //时钟极性
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                               //相位
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                                  //NSS信号由软件(SSI位)控制
    SPI_InitStructure.SPI_BaudRatePrescaler = auin_Baudratediv[uch_Baudrate];  //波特率
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                         //数据从MSB位开始传输
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI3, &SPI_InitStructure);                                        //初始化SPI口
    
    /* Enable SPI1 NSS output for master mode */
   // SPI_SSOutputCmd(SPI1, DISABLE);                                            //关闭SPI的SS输出                                           
    /* Enable SPI1  */
    SPI_Cmd(SPI3, ENABLE);                                                     //使能SPI口

    SPI3_Write(0xff);                                                  //启动传输 
    
    return TRUE;
}

//==================================================================================================
//| 函数名称 | SPI_Write()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | SPI写函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 写入的数据
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 读取的数据
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | xjl,13.08.06
//==================================================================================================
INT8U SPI1_Write( INT8U byte )
{
    
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);            //等待发送缓冲空
    SPI_I2S_SendData(SPI1, byte);                                              //发送一字节数据    
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);            //等待接收缓冲接收到数据
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);                                          //接收返回的数据
}
//==================================================================================================
//| 函数名称 | SPI2_Write()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | SPI2写函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 写入的数据
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 读取的数据
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | xjl,13.08.06
//==================================================================================================
INT8U SPI2_Write( INT8U byte )
{
    
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);            //等待发送缓冲空
    SPI_I2S_SendData(SPI2, byte);                                              //发送一字节数据    
    while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);            //等待接收缓冲接收到数据
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);                                          //接收返回的数据
}

//==================================================================================================
//| 函数名称 | SPI3_Write()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | SPI写函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 写入的数据
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 读取的数据
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | xjl,13.08.06
//==================================================================================================
INT8U SPI3_Write( INT8U byte )
{
    
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);            //等待发送缓冲空
    SPI_I2S_SendData(SPI3, byte);                                              //发送一字节数据    
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);            //等待接收缓冲接收到数据
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI3);                                          //接收返回的数据
}

//==================================================================================================
//| 函数名称 | STM32SPI2_SndData16
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | SPI2发送16 bit数据
//|----------|--------------------------------------------------------------------------------------
//| 调用模块 | SPI2_Write
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 待发送的数据send
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | xjl   2014-6-16
//==================================================================================================
void STM32SPI2_SndData16(INT16U send) 
{
	SPI2_Write((INT8U)(send>>8));
	SPI2_Write((INT8U)send);
}
//==================================================================================================
//| 函数名称 | SPI1_Read()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 从SPI读一字节数据
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 读回的数据
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | xjl,13.08.06
//==================================================================================================
INT8U SPI1_Read(void)
{
    return (SPI1_Write(DUMMY_BYTE));
}

//==================================================================================================
//| 函数名称 | SPI3_Read()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 从SPI读一字节数据
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 | 读回的数据
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | xjl,13.08.06
//==================================================================================================
INT8U SPI3_Read(void)
{
    return (SPI3_Write(DUMMY_BYTE));
}

