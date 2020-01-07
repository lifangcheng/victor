#include "save.h"
#include "STM32_BSP.h"

ST_SYSPARA gst_SysPara;//ϵͳ����


//==================================================================================================
//| �������� | InitParaFromFlash()
//|----------|--------------------------------------------------------------------------------------
//| �������� | ��Flash��ʼ������
//|----------|--------------------------------------------------------------------------------------
//| ������� | uch_InitFlag: ��ʼ����ʽ
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | 
//==================================================================================================
void InitParaFromFlash(INT8U uch_InitFlag)
{
	INT16U i,k;
	INT8U  uch_Temp=0; 
	INT16U uin_Temp;
	INT32U ul_Temp;
	FP32   f_Temp;
	INT32U ul_SaveAddr;
	MYSAVEPARA *p;
	
	//�ҳ���Ӧ��ַ
	for(i=0; i<N_MAX_SAVE; i++)
	{
	    p = (MYSAVEPARA*)&st_Save[i];
	    //�洢��ַΪ���������ʼ��
	    if(st_Save[i].ul_SaveAddr == 0)
	    {
	        break;
	    }
	    
	    //�洢������ַΪ��ʱ�����в���
	    if(st_Save[i].ul_VariableAddr == 0 || p->uin_Len > N_MAX_EEPROM_ARR)//��֧�ֳ��ȴ���
	    {
	        continue;
	    }
	    
	    //���и�ֵ�Ϸ��Բ��洢���洢��Ԫ
    	ul_SaveAddr = p->ul_SaveAddr;
    	switch(p->uch_TypeFlag)
    	{
    	  //INT8U����
    	  case KIND_BOOL:
    	  case KIND_INT8U:
    	  case KIND_INT8S:
    	    for(k=0; k < p->uin_Len; k++)
        	{
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        uch_Temp = (INT8U)p->f_Default;
        	        WriteCharToFlash(ul_SaveAddr+k*MW1,uch_Temp);
        	    }
        	    else     
        	    {
        	        uch_Temp = ReadCharFromFlash(ul_SaveAddr+k*MW1);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(uch_Temp >= (INT8U)p->f_Min && uch_Temp <= (INT8U)p->f_Max))
        	        {
        	            uch_Temp = (INT8U)p->f_Default;
        	            WriteCharToFlash(ul_SaveAddr+k*MW1,uch_Temp);
        	        } 
        	    }
        	    *(INT8U*)(p->ul_VariableAddr+k*MW1) = uch_Temp;
    	    }
    	    break;
    	    
    	    
    	  //INT16U����
    	  case KIND_INT16U:
    	  case KIND_INT16S:
    	    for(k=0; k < p->uin_Len; k++)
        	{
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        uin_Temp = (INT16U)p->f_Default;
        	        WriteIntToFlash(ul_SaveAddr+k*MW2,uin_Temp);
        	    }
        	    else
        	    {
        	        uin_Temp = ReadIntFromFlash(ul_SaveAddr+k*MW2);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(uin_Temp >= (INT16U)p->f_Min && uin_Temp <= (INT16U)p->f_Max))
        	        {
        	            uin_Temp = (INT16U)p->f_Default;
        	            WriteIntToFlash(ul_SaveAddr+k*MW2,uin_Temp);
        	        }
        	    }
        	    *(INT16U*)(p->ul_VariableAddr+k*MW2) = uin_Temp;
        	}
    	    break;
    	  
    	  //INT32U����
    	  case KIND_INT32U:
    	  case KIND_INT32S:
    	    for(k=0; k < p->uin_Len; k++)
            {
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        ul_Temp = (INT32U)p->f_Default;
        	        WriteLongToFlash(ul_SaveAddr+k*MW4,uch_Temp);
        	    }
        	    else
        	    {
        	        ul_Temp = ReadLongFromFlash(ul_SaveAddr+k*MW4);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(ul_Temp >= (INT32U)p->f_Min && ul_Temp <= (INT32U)p->f_Max))
        	        {
        	            ul_Temp = (INT32U)p->f_Default;
        	            WriteLongToFlash(ul_SaveAddr+k*MW4,ul_Temp);
        	        }
        	    }
        	    *(INT32U*)(p->ul_VariableAddr+k*MW4) = ul_Temp;
            }
    	    break;
    	  
    	    
    	  //FP32����
    	  case KIND_FP32:
    	  case KIND_FP64:
    	    for(k=0; k < p->uin_Len; k++)
            {
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        f_Temp = (FP32)p->f_Default;
        	        WriteFloatToFlash(ul_SaveAddr+k*MW4,f_Temp);
        	    }
        	    else
        	    {
        	        f_Temp = ReadFloatFromFlash(ul_SaveAddr+k*MW4);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(f_Temp >= (FP32)p->f_Min && f_Temp <= (FP32)p->f_Max))
        	        {
        	            f_Temp = (FP32)p->f_Default;
        	            WriteFloatToFlash(ul_SaveAddr+k*MW4,f_Temp);
        	        }
        	    }
        	    *(FP32*)(p->ul_VariableAddr+k*MW4) = f_Temp;
            }
    	    break;
    	 
    	  default:
    	    break;
    	}
		WDI_TOGGLE();
	}	
}
//==================================================================================================
//| �������� | SaveToFlash()
//|----------|--------------------------------------------------------------------------------------
//| �������� | ���ݴ洢����(����������)
//|----------|--------------------------------------------------------------------------------------
//| ������� | ul_VariableAddr: Ҫ�洢�������׵�ַ
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� | 
//==================================================================================================
BOOL SaveToFlash(INT32U ul_VariableAddr)
{
    INT16U i;
	INT8U  uch_Temp;
	INT16U uin_Temp;
	INT32U ul_Temp;
	FP32   f_Temp;
	INT32U ul_SaveAddr;
	MYSAVEPARA *p=0;
	
	//�ҳ���Ӧ��ַ
	for(i=0; i<N_MAX_SAVE; i++)
	{
	    if(st_Save[i].ul_VariableAddr == ul_VariableAddr)
	    {
	        p = (MYSAVEPARA*)&st_Save[i];
	        break;
	    }
	    else if(st_Save[i].ul_SaveAddr == 0)
	    {
	        return(FALSE);
	    }
	}
	
	//���и�ֵ�Ϸ��Բ��洢���洢��Ԫ
	ul_SaveAddr = p->ul_SaveAddr;
	switch(p->uch_TypeFlag)
	{
	  //INT8U����
	  case KIND_BOOL:
	  case KIND_INT8U:
	  case KIND_INT8S:
	    uch_Temp = *(INT8U*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(uch_Temp >= (INT8U)p->f_Min && uch_Temp <= (INT8U)p->f_Max)
	        {
	            WriteCharToFlash(ul_SaveAddr,uch_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteCharToFlash(ul_SaveAddr,uch_Temp);
	    }
	    break;
	    
	  //INT16U����
	  case KIND_INT16U:
	  case KIND_INT16S:
	    uin_Temp = *(INT16U*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(uin_Temp >= (INT16U)p->f_Min && uin_Temp <= (INT16U)p->f_Max)
	        {
	            WriteIntToFlash(ul_SaveAddr,uin_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {					 
	         WriteIntToFlash(ul_SaveAddr,uin_Temp);
	    }
	    break;
	
	  //INT32U����
	  case KIND_INT32U:
	  case KIND_INT32S:
	    ul_Temp = *(INT32U*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(ul_Temp >= (INT32U)p->f_Min && ul_Temp <= (INT32U)p->f_Max)
	        {
	            WriteLongToFlash(ul_SaveAddr,ul_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteLongToFlash(ul_SaveAddr,ul_Temp);
	    }
	    break;
	    
	  //FP32����
	  case KIND_FP32:
	  case KIND_FP64:
	    f_Temp = *(FP32*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(f_Temp >= (FP32)p->f_Min && f_Temp <= (FP32)p->f_Max)
	        {
	            WriteFloatToFlash(ul_SaveAddr,f_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteFloatToFlash(ul_SaveAddr,f_Temp);
	    }
	    break;
	    
	  default:
	    break;
	}
	return(TRUE);
}



//==================================================================================================
//| �������� | SaveToEeprom()
//|----------|--------------------------------------------------------------------------------------
//| �������� | ���ݴ洢����(����������)
//|----------|--------------------------------------------------------------------------------------
//| ������� | ul_VariableAddr: Ҫ�洢�������׵�ַ
//|          | uin_Len: ���ݳ���
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//|----------|-------------------------------------------------------------------------------------- 
//| ������� |  
//==================================================================================================
BOOL SaveToFlashExt(INT32U ul_VariableAddr,INT16U uin_Len)
{
	INT16U i,k;
	INT8U  uch_Temp;
	INT16U uin_Temp;
	INT32U ul_Temp;
	FP32   f_Temp;
	INT32U ul_SaveAddr;
	MYSAVEPARA *p=0;
	
	//�ҳ���Ӧ��ַ
	for(i=0; i<N_MAX_SAVE; i++)
	{
	    if(st_Save[i].ul_VariableAddr == ul_VariableAddr)
	    {
	        p = (MYSAVEPARA*)&st_Save[i];
	        break;
	    }
	    else if(st_Save[i].ul_SaveAddr == 0)
	    {
	        return(FALSE);
	    }
	}
	
	//���ȺϷ����ж�
	if(uin_Len > p->uin_Len || uin_Len > N_MAX_EEPROM_ARR)
	{
	    return(FALSE);
	}
	
	
	//���и�ֵ�Ϸ��Բ��洢���洢��Ԫ
	ul_SaveAddr = p->ul_SaveAddr;
	switch(p->uch_TypeFlag)
	{
	  //INT8U����
	  case KIND_BOOL:
	  case KIND_INT8U:
	  case KIND_INT8S:
	    for(k=0; k < uin_Len; k++)
	    {
            uch_Temp = *(INT8U*)(ul_VariableAddr+k*MW1);
            if(p->uch_LimitFlag == LIMIT_RANGE)
            {
                if(uch_Temp >= (INT8U)p->f_Min && uch_Temp <= (INT8U)p->f_Max)
                {
                    WriteCharToFlash(ul_SaveAddr+k*MW1,uch_Temp);
                }
                else
                {
                    return(FALSE);
                }
            }
            else
            {
                 WriteCharToFlash(ul_SaveAddr+k*MW1,uch_Temp);
            }
	    }
	    break;
	    
	  //INT16U����
	  case KIND_INT16U:
	  case KIND_INT16S:
	    for(k=0; k < uin_Len; k++)
	    {
    	    uin_Temp = *(INT16U*)(ul_VariableAddr+k*MW2);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(uin_Temp >= (INT16U)p->f_Min && uin_Temp <= (INT16U)p->f_Max)
    	        {
    	            WriteIntToFlash(ul_SaveAddr+k*MW2,uin_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteIntToFlash(ul_SaveAddr+k*MW2,uin_Temp);
    	    }
	    }
	    break;
	
	  //INT32U����
	  case KIND_INT32U:
	  case KIND_INT32S:
	    for(k=0; k < p->uin_Len; k++)
	    {
    	    ul_Temp = *(INT32U*)(ul_VariableAddr+k*MW4);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(ul_Temp >= (INT32U)p->f_Min && ul_Temp <= (INT32U)p->f_Max)
    	        {
    	            WriteLongToFlash(ul_SaveAddr+k*MW4,ul_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteLongToFlash(ul_SaveAddr+k*MW4,ul_Temp);
    	    }
	    }
	    break;
	    
	  //FP32����
	  case KIND_FP32:
	  case KIND_FP64:
	    for(k=0; k < p->uin_Len; k++)
	    {
    	    f_Temp = *(FP32*)(ul_VariableAddr+k*MW4);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(f_Temp >= (FP32)p->f_Min && f_Temp <= (FP32)p->f_Max)
    	        {
    	            WriteFloatToFlash(ul_SaveAddr+k*MW4,f_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteFloatToFlash(ul_SaveAddr+k*MW4,f_Temp);
    	    }
	    }
	    break;
	    
	  default:
	    break;
	}
	return(TRUE);	
}
//------------------------------------------- FLASH(д)-------------------------------------//
void WriteCharToFlash(INT32U uin_Addr,INT8U uch_Data)
{
	MT25QL128_Write((u8*)&uch_Data,uin_Addr,1);
}   
void WriteIntToFlash(INT32U uin_Addr,INT16U uin_Data)
{
	MT25QL128_Write((u8*)&uin_Data,uin_Addr,2);
}   
void WriteLongToFlash(INT32U uin_Addr,INT32U ul_Data)
{
	MT25QL128_Write((u8*)&ul_Data,uin_Addr,4);
} 
void WriteFloatToFlash(INT32U uin_Addr,FP32 f_Data)
{
	MT25QL128_Write((u8*)&f_Data,uin_Addr,4);
} 

//-------------------------------------------FLASH(��)-------------------------------------//
INT8U ReadCharFromFlash(INT32U uin_Addr)
{
    u8 auch_Data[4]={0};   
    MT25QL128_Read(auch_Data,uin_Addr,1);
    return(*(u8*)(auch_Data));
} 
INT16U ReadIntFromFlash(INT32U uin_Addr)
{
    u8 auch_Data[4]={0};
    MT25QL128_Read(auch_Data,uin_Addr,2);
    return(*(u16*)auch_Data);
} 
INT32U ReadLongFromFlash(INT32U uin_Addr)
{
    u8 auch_Data[4]={0};
    MT25QL128_Read(auch_Data,uin_Addr,4);
    return(*(u32*)auch_Data);
}   
FP32 ReadFloatFromFlash(INT32U uin_Addr)
{
    u8 auch_Data[4]={0};
    MT25QL128_Read(auch_Data,uin_Addr,4);
    return(*(FP32*)auch_Data);
}
