//==================================================================================
//| 文件名称 | 自定义函数
//|----------|----------------------------------------------------------------------
//| 文件功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//================================================================================== 

#include "Comm_Myfun.h"


const unsigned char auch_CRCHi[256] = 
{
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;

const unsigned char auch_CRCLo[256] = 
{
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;
//==================================================================================
//| 函数名称 | GetCrc16Bit
//|----------|----------------------------------------------------------------------
//| 函数功能 | 获取16位的CRC校验码
//|----------|----------------------------------------------------------------------
//| 输入参数 | p_uch_Data:      需要进行crc计算的数据帧  
//|          | uin_CrcDataLen:  该数据帧的长度
//|----------|---------------------------------------------------------------------- 
//| 返回参数 | 16位的CRC校验码
//================================================================================== 
unsigned int GetCrc16Bit(unsigned char *p_uch_Data, unsigned int uin_CrcDataLen )
{
	unsigned int  uin_Temp;
	unsigned char uch_CRCHi = 0xFF ;
   	unsigned char uch_CRCLo = 0xFF ;
   	unsigned char uch_Index=0;

    while (uin_CrcDataLen--)
    {   
         uch_Index = uch_CRCHi ^ (*p_uch_Data++)&0xff ;  
         uch_CRCHi = uch_CRCLo ^ auch_CRCHi[uch_Index];
         uch_CRCLo = auch_CRCLo[uch_Index] ;
    }
	uin_Temp = ((unsigned int)uch_CRCHi << 8) | ((unsigned int)uch_CRCLo);
  	return (uin_Temp);
}


//==================================================================================
//| 函数名称 | EncodeFloat()
//|----------|----------------------------------------------------------------------
//| 函数功能 | float加码
//|----------|----------------------------------------------------------------------
//| 输入参数 | f1: 要加码的数据
//|          | p:  加码数组指针
//|----------|---------------------------------------------------------------------       
//| 返回参数 | 无
//=================================================================================
void EncodeFloat(float f1, unsigned char *p)
{
   	TYPECHANGE u_Temp;
   	u_Temp.f_FloatData = f1;
   	p[0] = u_Temp.bit.byte0;
   	p[1] = u_Temp.bit.byte1;
   	p[2] = u_Temp.bit.byte2;
   	p[3] = u_Temp.bit.byte3;
} 


//=================================================================================
//| 函数名称 | DecodeFloat()
//|----------|---------------------------------------------------------------------
//| 函数功能 | float解码
//|----------|---------------------------------------------------------------------
//| 输入参数 | p:  解码数组指针
//|----------|---------------------------------------------------------------------      
//| 返回参数 | 解码数据值
//=================================================================================
float DecodeFloat(unsigned char *p)
{
   	TYPECHANGE u_Temp;
   	u_Temp.bit.byte0 = p[0];
   	u_Temp.bit.byte1 = p[1];
   	u_Temp.bit.byte2 = p[2];
   	u_Temp.bit.byte3 = p[3];
   	return(u_Temp.f_FloatData);
}

//=================================================================================
//| 函数名称 | EncodeUlong()
//|----------|---------------------------------------------------------------------
//| 函数功能 | unsigned long加码
//|----------|---------------------------------------------------------------------
//| 输入参数 | ul_Data: 要加码的数据
//|          | p:  加码数组指针
//|----------|---------------------------------------------------------------------     
//| 返回参数 | 无
//=================================================================================
void EncodeUlong(unsigned long ul_Data, unsigned char *p)
{
   	TYPECHANGE u_Temp;
   	u_Temp.ul_UlongData = ul_Data;
   	p[0] = u_Temp.bit.byte0;
   	p[1] = u_Temp.bit.byte1;
   	p[2] = u_Temp.bit.byte2;
   	p[3] = u_Temp.bit.byte3;
} 

//=================================================================================
//| 函数名称 | DecodeUlong()
//|----------|---------------------------------------------------------------------
//| 函数功能 | unsigned long解码
//|----------|---------------------------------------------------------------------
//| 输入参数 | p:  解码数组指针
//|----------|---------------------------------------------------------------------      
//| 返回参数 | 解码数据值
//=================================================================================
unsigned long DecodeUlong(unsigned char *p)
{
   	TYPECHANGE u_Temp;
   	u_Temp.bit.byte0 = p[0];
   	u_Temp.bit.byte1 = p[1];
   	u_Temp.bit.byte2 = p[2];
   	u_Temp.bit.byte3 = p[3];
   	return(u_Temp.ul_UlongData);
}

//==================================================================================
//| 函数名称 | EncodeLong()
//|----------|----------------------------------------------------------------------
//| 函数功能 | signed long加码
//|----------|----------------------------------------------------------------------
//| 输入参数 | lg_Data: 要加码的数据
//|          | p:  加码数组指针
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
void EncodeLong(signed long lg_Data, unsigned char *p)
{
   	TYPECHANGE u_Temp;
   	u_Temp.lg_LongData =  lg_Data;
   	p[0] = u_Temp.bit.byte0;
   	p[1] = u_Temp.bit.byte1;
   	p[2] = u_Temp.bit.byte2;
   	p[3] = u_Temp.bit.byte3;
} 

//==================================================================================
//| 函数名称 | DecodeLong()
//|----------|----------------------------------------------------------------------
//| 函数功能 | signed long解码
//|----------|----------------------------------------------------------------------
//| 输入参数 | p:  解码数组指针
//|----------|---------------------------------------------------------------------- 
//| 返回参数 | 解码数据值
//==================================================================================
signed long DecodeLong(unsigned char *p)
{
    TYPECHANGE u_Temp;
   	u_Temp.bit.byte0 = p[0];
   	u_Temp.bit.byte1 = p[1];
   	u_Temp.bit.byte2 = p[2];
   	u_Temp.bit.byte3 = p[3];
   	return(u_Temp.lg_LongData);
}

//==================================================================================
//| 函数名称 | EncodeUint()
//|----------|----------------------------------------------------------------------
//| 函数功能 | unsigned int加码
//|----------|----------------------------------------------------------------------
//| 输入参数 | uin_Data: 要加码的数据
//|          | p:  加码数组指针
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//==================================================================================
void EncodeUint(unsigned int uin_Data, unsigned char *p)
{
   	TYPECHANGE2 u_Temp;
   	u_Temp.uin_UintData =  uin_Data;
   	p[0] = u_Temp.bit.byte0;
   	p[1] = u_Temp.bit.byte1;
} 


//==================================================================================
//| 函数名称 | DecodeUint()
//|----------|----------------------------------------------------------------------
//| 函数功能 | unsigned int解码
//|----------|----------------------------------------------------------------------
//| 输入参数 | p:  解码数组指针
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 解码数据值
//==================================================================================
unsigned int DecodeUint(unsigned char *p)
{
   	TYPECHANGE2 u_Temp;
	u_Temp.uin_UintData= 0;
   	u_Temp.bit.byte0 = p[0];
   	u_Temp.bit.byte1 = p[1];
   	return(u_Temp.uin_UintData);
}

//==================================================================================
//| 函数名称 | EncodeInt()
//|----------|----------------------------------------------------------------------
//| 函数功能 | signed int加码
//|----------|----------------------------------------------------------------------
//| 输入参数 | in_Data: 要加码的数据
//|          | p:  加码数组指针
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
void EncodeInt(signed int in_Data, unsigned char *p)
{
   	TYPECHANGE2 u_Temp;
   	u_Temp.in_IntData =  in_Data;
   	p[0] = u_Temp.bit.byte0;
   	p[1] = u_Temp.bit.byte1;
} 


//==================================================================================
//| 函数名称 | DecodeInt()
//|----------|----------------------------------------------------------------------
//| 函数功能 | signed int解码
//|----------|----------------------------------------------------------------------
//| 输入参数 | p:  解码数组指针
//|----------|----------------------------------------------------------------------       
//| 返回参数 | 解码数据值
//==================================================================================
signed int DecodeInt(unsigned char *p)
{
   	TYPECHANGE2 u_Temp;
   	u_Temp.bit.byte0 = p[0];
   	u_Temp.bit.byte1 = p[1];
   	return(u_Temp.in_IntData);
}
//==================================================================================
//| 函数名称 | CopyDataFromAToB()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 把数据从A拷贝到B(8位无符号整形)
//|----------|----------------------------------------------------------------------
//| 输入参数 | pAbuf:  数据组A指针
//|          | pBbuf:  数据组B指针
//|          | uin_Len:  要拷贝的数据长度
//|----------|----------------------------------------------------------------------       
//| 返回参数 | 解码数据值
//==================================================================================
void CopyFromAToB(unsigned char *pAbuf,unsigned char *pBbuf,unsigned int uin_Len)
{
   unsigned int i;
   for(i=0; i<uin_Len; i++)
   {
      pBbuf[i] = pAbuf[i]; 
   } 
}
