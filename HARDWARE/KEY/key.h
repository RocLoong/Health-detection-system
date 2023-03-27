#ifndef __KEY_H_
#define __KEY_H_ 
#include "sys.h"
 

#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)
#define KEY2  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
 

#define KEY0_PRES	0		//KEY0  
#define KEY1_PRES	1		//KEY1 
#define KEY2_PRES	2		//WK_UP  

void key_init(void);
u8 key_scan(void);
#endif
