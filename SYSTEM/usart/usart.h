
/*
 * @Author: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @Date: 2023-03-15 20:43:36
 * @LastEditors: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @LastEditTime: 2023-03-27 20:33:14
 * @FilePath: \USERe:\Git\Repository\Health-detection-system\SYSTEM\usart\usart.h
 * @Description: ����Ĭ������,������`customMade`, ��koroFileHeader�鿴���� ��������: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define  USART_REC_LEN  			200  	//�����������ֽ��� 200


void USART_Config(void);

	  	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

/*-----------------------------usart2-------------------------------------------------*/

#define HC05_MAX_RECV_LEN		200					//�����ջ����ֽ���
#define HC05_MAX_SEND_LEN		200					//����ͻ����ֽ���
extern u8 HC05_RECV_BUF[HC05_MAX_RECV_LEN];
extern u8 HC05_SEND_BUF[HC05_MAX_SEND_LEN];
extern u16 HC05_RX_STA;   						//��������״̬
void uart_hc05_init(u32 bound);
void hc05_send_byte(uint8_t ch);
void hc05_send_str(const unsigned char *p_str);



/*---------------------------------usaart 3-----------------------------------------*/
#define  USART_HRV_LEN              88      // ����Ѫѹ�����������ֽ��� 88
extern u8  USART_HRV_RX_BUF[USART_HRV_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_HRV_RX_STA;         		//����״̬���	
extern u8 senor_Flag;
void uart_hrv_init(void);



#endif


