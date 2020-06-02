#ifndef SAVE_H
#define SAVE_H

#include "stm32f4xx.h"
#include "TypeDefine.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define  START_ADD    (INT32U)0x10 //��ʼ��ַ
//========================= Macro,Typestruct, & Class =====================//
#define N_MAX_EEPROM_ARR  1024 //���EEPROM������洢����

#define MW1  1  //INT8U ռ��1���ֽڵ���С�洢��Ԫ
#define MW2  2  //INT16Uռ��2���ֽڵ���С�洢��Ԫ
#define MW4  4  //INT16Uռ��4���ֽڵ���С�洢��Ԫ

//12�ֻ�������������
#define  KIND_INT8U   0x01
#define  KIND_INT8S   0x02
#define  KIND_INT16U  0x03
#define  KIND_INT16S  0x04
#define  KIND_INT32U  0x05
#define  KIND_INT32S  0x06
#define  KIND_FP32    0x07
#define  KIND_FP64    0x08
#define  KIND_STRING  0x09
#define  KIND_COMBOX  0x0A
#define  KIND_BOOL    0x0B
#define  KIND_STRUCT  0x0C

//�޶���ʽ����
#define  LIMIT_NULL   0x00 
#define  LIMIT_RANGE  0x01 
//��ʼ����ʽ
#define  INIT_NORMAL  0x00
#define  INIT_DEFAULT 0x01

#define N_MAX_SAVE   200   //����������

  
//Eeprom�洢����
typedef struct
{
    INT32U  ul_SaveAddr;         //EEprom�洢��ַ
	INT32U  ul_VariableAddr;     //������ַ
	INT8U   uch_TypeFlag;        //��������
	INT16U  uin_Len;             //���ݳ���
    INT8U   uch_LimitFlag;       //�޶���ʽ
    FP32    f_Min;               //�޶���Сֵ
    FP32    f_Max;               //�޶����ֵ
	FP32    f_Default;           //Ĭ��ֵ
}MYSAVEPARA;      

typedef struct
{
	INT8U uch_SoftWareVer[32];//????
	INT8U uch_SerilNum[13];//?????
	INT8U uch_FirstInit;//????????
	INT8U uch_MacType;//????
	
	INT8U uch_Adr;//????
	INT8U uch_Baud;//???    
    INT16U uin_TimeInt;
	INT16U uin_SamAvr;
        
}ST_SYSPARA;

extern ST_SYSPARA gst_SysPara;//ϵͳ����

//---------------------------------������ַ---------------------------------//
static const MYSAVEPARA st_Save[N_MAX_SAVE]=
{
//�洢��ַ|-----------------������ַ---------------|-----����------|-����-|-----�޶���-----|---��Сֵ--|---���ֵ---|--Ĭ��ֵ---//
//START_ADD, (INT32U)&gst_SysPara.uch_FirstInit,KIND_INT8U,    1,    LIMIT_RANGE,       '0',         '9',      '1',	
START_ADD+36,(INT32U)&gst_SysPara.uch_SerilNum[0],   KIND_INT8U,    12,    LIMIT_RANGE,       0,         'z',      'X',
START_ADD+48, (INT32U)&gst_SysPara.uch_Adr,KIND_INT8U,    1,    LIMIT_RANGE,       0x20,          0x3f,      0x20,	
START_ADD+49, (INT32U)&gst_SysPara.uch_Baud,KIND_INT8U,    1,    LIMIT_RANGE,       0x01,          0x05,      0x04,	
START_ADD+50, (INT32U)&gst_SysPara.uin_TimeInt,KIND_INT16U,    2,    LIMIT_RANGE,       1,          1000,      100,	
START_ADD+52, (INT32U)&gst_SysPara.uin_SamAvr ,KIND_INT16U,    2,    LIMIT_RANGE,       1,          1000,      1,	
};

//========================= Variable,Object & Fuction =====================//
void InitParaFromFlash(INT8U uch_InitFlag);
BOOL SaveToFlash(INT32U ul_VariableAddr);
BOOL SaveToFlashExt(INT32U ul_VariableAddr,INT16U uin_Len);

void   WriteCharToFlash(INT32U uin_Addr,INT8U uch_Data);
void   WriteIntToFlash(INT32U uin_Addr,INT16U uin_Data);
void   WriteLongToFlash(INT32U uin_Addr,INT32U ul_Data);
void   WriteFloatToFlash(INT32U uin_Addr,FP32 f_Data);
INT8U  ReadCharFromFlash(INT32U uin_Addr);
INT16U ReadIntFromFlash(INT32U uin_Addr);
INT32U ReadLongFromFlash(INT32U uin_Addr);
FP32   ReadFloatFromFlash(INT32U uin_Addr);



#ifdef __cplusplus
}
#endif

#endif
