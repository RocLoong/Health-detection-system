/*
 * @Author: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @Date: 2023-03-25 11:14:19
 * @LastEditors: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @LastEditTime: 2023-04-06 17:22:25
 * @FilePath: \USERe:\Git\Repository\Health-detection-system\HARDWARE\KEY\key.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "key.h"
#include "delay.h"

 	    
void key_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
} 


/*
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;
	if(mode)key_up=1;  	
	if(key_up&&(KEY0==0||KEY1==0||KEY2==1))
	{
		delay_ms(10);
		key_up=0;
		if(KEY0==0) return KEY0_PRES;
		else if(KEY1==0) return KEY1_PRES;
		else if(KEY2==1) return KEY3_PRES; 
	}else if(KEY0==1&&KEY1==1&&KEY2==0)key_up=1; 	     
	return 0;
}
*/



u8 key_scan(void)
{	 
	static u8 mode = 3;
	if(KEY0 == 0 && KEY1 == 1 && KEY2 == 1)
	{
		mode = KEY0_PRES;
	}
	else if(KEY0 == 1 && KEY1 == 0 && KEY2 == 1)
	{
		mode = KEY1_PRES;
	}
	else if(KEY0 == 1 && KEY1 == 1 && KEY2 == 0)
	{
		mode = KEY2_PRES;
	}
	return mode;
}