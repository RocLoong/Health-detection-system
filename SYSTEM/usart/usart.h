/*
 * @Author: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @Date: 2023-03-15 20:43:36
 * @LastEditors: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @LastEditTime: 2023-03-19 12:56:39
 * @FilePath: \USERe:\Git\Repository\Health-detection-system\SYSTEM\usart\usart.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define  USART_REC_LEN  			200  	//定义最大接收字节数 200
	  	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

/*---------------------------------usaart 2-----------------------------------------*/
#define  USART_HRV_LEN              76      // 定义血压串口最大接受字节数 76
extern u8  USART_HRV_RX_BUF[USART_HRV_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_HRV_RX_STA;         		//接收状态标记	
void uart_hrv_init(void);



#endif


