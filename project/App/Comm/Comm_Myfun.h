//==================================================================================
//| 函数名称 | 自定义函数()
//|----------|----------------------------------------------------------------------
//| 函数功能 | 
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------      
//| 返回参数 | 无
//================================================================================== 
#ifndef  COMM_MYFUN_H
#define  COMM_MYFUN_H 


//=========================== Macro,Typestruct, & Class ===================//
#define MAIN_ASPECTS_EN  0   //1：大端模式, 0:小端模式

#if MAIN_ASPECTS_EN > 0    //大端模式
	typedef struct 
	{      
	   unsigned int byte0:8;    
	   unsigned int byte1:8;          
	}TYPE_INT16U;

	typedef struct 
	{      
	   signed int byte0:8;    
	   signed int byte1:8;          
	}TYPE_INT16S;
	
	typedef struct 
	{      
	   unsigned long byte0:8;    
	   unsigned long byte1:8; 
	   unsigned long byte2:8;    
	   unsigned long byte3:8;   
	}TYPE_INT32U;

	typedef struct 
	{      
	   signed long byte0:8;    
	   signed long byte1:8; 
	   signed long byte2:8;    
	   signed long byte3:8;           
	}TYPE_INT32S;
#else                       //小端模式
    typedef struct 
    {      
	   unsigned int byte1:8;    
	   unsigned int byte0:8;          
	}TYPE_INT16U;

	typedef struct 
	{      
	   signed int byte1:8;    
	   signed int byte0:8;          
	}TYPE_INT16S;
	
	typedef struct 
	{      
	   unsigned long byte3:8;    
	   unsigned long byte2:8; 
	   unsigned long byte1:8;    
	   unsigned long byte0:8;   
	}TYPE_INT32U;

	typedef struct 
	{      
	   signed long byte3:8;    
	   signed long byte2:8; 
	   signed long byte1:8;    
	   signed long byte0:8;           
	}TYPE_INT32S;
#endif

typedef union 
{
	float          f_FloatData;	
	signed long    lg_LongData; 
	unsigned long  ul_UlongData;
	TYPE_INT32U   bit;
}TYPECHANGE;


typedef union 
{
    signed int        in_IntData;
    unsigned int      uin_UintData;
	TYPE_INT16U   bit;	
}TYPECHANGE2;

//========================= Variable,Object & Fuction =====================//
unsigned int  GetCrc16Bit(unsigned char *p_uch_Data,unsigned int uin_CrcDataLen);
void          EncodeFloat(float f1, unsigned char *p);
float         DecodeFloat(unsigned char *p);
void          EncodeUlong(unsigned long ul_Data, unsigned char *p);
unsigned long DecodeUlong(unsigned char *p);
void          EncodeLong(signed long lg_Data, unsigned char *p);
signed long   DecodeLong(unsigned char *p);
void          EncodeUint(unsigned int uin_Data, unsigned char *p);
unsigned int  DecodeUint(unsigned char *p);
void          EncodeInt(signed int in_Data, unsigned char *p);
signed int    DecodeInt(unsigned char *p);
void          CopyFromAToB(unsigned char *pAbuf,unsigned char *pBbuf,unsigned int uin_Len);


#endif

