/*
 * @Author: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @Date: 2023-03-10 13:25:17
 * @LastEditors: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @LastEditTime: 2023-03-22 16:07:33
 * @FilePath: \USERe:\STM32\MAX30102\MAX30102\HARDWARE\OLED\oled.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    
////////////////////////////////////////////////////////////////////////////////// 	  
		    						  
//-----------------OLED端口定义----------------  					   
#define OLED_CS 	PAout(0)
#define OLED_RST 	PAout(5)
#define OLED_RS 	PAout(1)

//使用4线串行接口时使用 
#define OLED_SCLK PAout(4)
#define OLED_SDIN PAout(6)
		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_wave(u8 Wave_sum);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);	 
#endif  
	 



