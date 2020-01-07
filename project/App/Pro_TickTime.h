//==================================================================================================
//| 文件名称 | Pro_TickTime.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Pro_TickTime.cpp所对应的头文件
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | Copyright 2009
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2009.4.2    | gao       | 初版
//==================================================================================================
#ifndef __PRO_TICKTIM_H__
#define __PRO_TICKTIM_H__

//========================= Macro,Typestruct, & Class =====================//
#define TIME_20MS       (1)     //10毫秒
#define TIME_1SEC       (99)    //1秒
#define TIME_7SEC       (699)   //7秒
#define TIME_10SEC      (999)   //10秒
#define TIME_30SEC      (2999)   //30秒
#define TIME_1SEC_IC    (99)     //zhliu (7)     //1秒

//========================= Variable,Object & Fuction =====================//
INT32U p_CylSub(INT32U x, INT32U y);
void   TickTimeInit(void);
void   TickDelayUs(INT32U ul_Us);
void   TickDealyMs(INT32U ul_Ms);
void   StartTickUs(INT32U *pul_Us);
void   StartTickMs(INT32U *pul_Ms);
INT32U GetTickUs(INT32U *pul_Us);
INT32U GetTickMs(INT32U *pul_Ms);
INT32U GetTickMsExt(INT32U *pul_Ms);

#endif

