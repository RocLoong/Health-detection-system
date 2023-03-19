/*
 * @Author: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @Date: 2023-03-15 20:43:36
 * @LastEditors: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @LastEditTime: 2023-03-19 12:56:39
 * @FilePath: \USERe:\Git\Repository\Health-detection-system\SYSTEM\usart\usart.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define  USART_REC_LEN  			200  	//�����������ֽ��� 200
	  	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

/*---------------------------------usaart 2-----------------------------------------*/
#define  USART_HRV_LEN              76      // ����Ѫѹ�����������ֽ��� 76
extern u8  USART_HRV_RX_BUF[USART_HRV_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_HRV_RX_STA;         		//����״̬���	
void uart_hrv_init(void);



#endif


