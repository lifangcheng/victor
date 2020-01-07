
#include "TypeDefine.h"

#include "STM32_BSP.h"

#include "Pro_TickTime.h"

INT32U p_CylSub(INT32U x, INT32U y) 
{
    if(x >= y) 
    {
        return (x-y);
    } 
    else 
    {
        return ((INT32U)0xFFFFFFFF-y+x);
    }
}

void TickTimeInit(void)
{   
	;
}

void ClrCOP()  //
{
     WDI_TOGGLE();  //Çå³ý¿´ÃÅ¹·
}

void TickDelayUs(INT32U ul_Us)
{
   INT32U  ul_Temp0;
   INT32U  ul_Temp=0;
   ul_Temp0 = GetSystermTimerUs();
   while(ul_Temp < ul_Us)
   {
      ul_Temp = GetSystermTimerUs();
	  ul_Temp = p_CylSub(ul_Temp,ul_Temp0);
   }
}

void TickDealyMs(INT32U ul_Ms)   
{
   INT32U  ul_Temp0;
   INT32U  ul_Temp = 0;
   ul_Temp0 = GetSystermTimerMs();
   while( ul_Temp < ul_Ms)
   {
      ul_Temp = GetSystermTimerMs();
	  ul_Temp = p_CylSub(ul_Temp,ul_Temp0);
      ClrCOP();  
     
   }
}

void StartTickUs(INT32U *pul_Us)
{
    *pul_Us = GetSystermTimerUs();
}

void StartTickMs(INT32U *pul_Ms)
{
    *pul_Ms = GetSystermTimerMs();
}

INT32U GetTickUs(INT32U *pul_Us)
{
    INT32U  ul_Temp;
    
    ul_Temp = GetSystermTimerUs();
    ul_Temp = p_CylSub(ul_Temp,*pul_Us);
    
    return(ul_Temp);
}

INT32U gul_Tick1,gul_Tick2,gul_Tick3,gul_Tick4;

INT32U GetTickMs(INT32U *pul_Ms)
{
	static int i;
    static INT32U  ul_Temp;
	static INT32U  ul_Temp1;
    static INT32U  ul_Temp2;
	
	ul_Temp2 = *pul_Ms;
    
    ul_Temp1 = GetSystermTimerMs();
    ul_Temp = p_CylSub(ul_Temp1,*pul_Ms);
	
	if(ul_Temp > 2000)
	{
		i = 0;
	}
    
    return(ul_Temp);
}

INT32U p_CylSubExt(INT32U x, INT32U y) 
{
    if(x >= y) 
    {
        return (x-y);
    } 
    else 
    {
        return (x+10-y);
        //return ((INT32U)0xFFFFFFFF-y+x);
    }
}

INT32U GetTickMsExt(INT32U *pul_Ms)
{
	static int i;
    static INT32U  ul_Temp;
	static INT32U  ul_Temp1;
	    
    ul_Temp1 = GetSystermTimerMs();
    ul_Temp = p_CylSubExt(ul_Temp1,*pul_Ms);
	
	if(ul_Temp > 2000)
	{
		i = 0;
	}
    
    return(ul_Temp);
}

