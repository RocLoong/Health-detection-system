/*
 * @Author: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @Date: 2023-03-22 16:20:40
 * @LastEditors: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @LastEditTime: 2023-03-27 21:40:58
 * @FilePath: \USERe:\Git\Repository\Health-detection-system\HARDWARE\HC05\hc05.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __HC05_H
#define __HC05_H

#include "sys.h"
#include "usart.h"
//连接模块GPIO相关参数的一层封装
//**********************************************************************************
#define RCC_EN  RCC_APB2Periph_GPIOA
#define EN_Pin GPIO_Pin_8 
//**********************************************************************************

#define HC05_EN  	PAin(8)		//蓝牙连接状态信号

#define HC05_ADDR "98D3:02:960B52"

u8 HC05_Init(void);
//void HC05_CFG_CMD(u8 *str);
u8 HC05_Get_Role(void);
u8 HC05_Set_Cmd(u8* atstr);	
#endif
