#ifndef __DRV_S8377_H__
#define __DRV_S8377_H__

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "stm32f4xx.h"
#include "TypeDefine.h"

#define S8377_ST_SET()		(GPIOB->BSRRL=GPIO_Pin_8)
#define S8377_ST_CLR()		(GPIOB->BSRRH=GPIO_Pin_8)

#define S8377_CLK_SET()	    (GPIOB->BSRRL=GPIO_Pin_7)
#define S8377_CLK_CLR()	    (GPIOB->BSRRH=GPIO_Pin_7)

#define S8377_EOS_READ()	GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)

extern void S8377_init(void);
extern void S8377_StartIntegration(void);   
extern bool S8377_ImageCapture(INT16U* ImageData);
extern void S8377_erzhihua(uint8_t len, uint8_t *data);
extern void S8377_vImageCalculate(uint8_t * ImageData);
extern void S8377_CalculateIntegrationTime(void);
extern uint8_t S8377_PixelAverage(uint8_t len, uint8_t *data);

extern void SendHex(uint8_t hex);
extern void SendImageData(uint16_t * ImageData);
extern void GetCenter(uint8_t * ImageData);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_S8377_H__*/
