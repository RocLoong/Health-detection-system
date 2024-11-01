
/*
 * @Author: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @Date: 2023-03-15 20:43:36
 * @LastEditors: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @LastEditTime: 2023-03-27 20:33:14
 * @FilePath: \USERe:\Git\Repository\Health-detection-system\SYSTEM\usart\usart.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define  USART_REC_LEN  			200  	//定义最大接收字节数 200


void USART_Config(void);

	  	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

/*-----------------------------usart2-------------------------------------------------*/

#define HC05_MAX_RECV_LEN		200					//最大接收缓存字节数
#define HC05_MAX_SEND_LEN		200					//最大发送缓存字节数
extern u8 HC05_RECV_BUF[HC05_MAX_RECV_LEN];
extern u8 HC05_SEND_BUF[HC05_MAX_SEND_LEN];
extern u16 HC05_RX_STA;   						//接收数据状态
void uart_hc05_init(u32 bound);
void hc05_send_byte(uint8_t ch);
void hc05_send_str(const unsigned char *p_str);



/*---------------------------------usaart 3-----------------------------------------*/
#define  USART_HRV_LEN              88      // 定义血压串口最大接受字节数 88
extern u8  USART_HRV_RX_BUF[USART_HRV_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_HRV_RX_STA;         		//接收状态标记	
extern u8 senor_Flag;
void uart_hrv_init(void);



#endif


