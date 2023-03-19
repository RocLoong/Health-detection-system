/*************************************************************************
 ����ʹ������ԭ�ӵı�׼����, ����ѧϰʹ��
**************************************************************************************/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "max30102.h" 
#include "myiic.h"
#include "algorithm.h"
#include "oled.h"
#include "debug.h"
#include <stdlib.h>
//#include <stdio.h>
#define MAX_BRIGHTNESS 255
#define PPG_WAVE_COUNT 	4
#define PPG_MIN_COUNT 	0X3FFFF


void hrv_display();
void max30102_display();




uint32_t aun_ir_buffer[500]; 	//IR����,�����ԭʼ���ݻ�����
uint32_t aun_red_buffer[500];  	//Red����,���ԭʼ���ݻ�����
int32_t n_ir_buffer_length;    	//���ݳ���
	
int32_t n_sp02; 				//Ѫ��ֵ
int32_t n_heart_rate;   		//����ֵ
	
int8_t ch_spo2_valid;   		//�㷨ִ�гɹ���־λ
int8_t ch_hr_valid;    			

extern u8 senor_Flag;

int main(void)
{ 
	NVIC_Configuration();     	//��������ʼ��
	delay_init();	    		  
	uart_init(115200);
	uart_hrv_init();
	LED_Init();
	OLED_Init();
	max30102_init();
	

	while(1)
	{
		// max30102_display();
		hrv_display();
	}
}





void max30102_display()
{
	uint32_t un_min, un_max; 	//ԭʼ���ݵ����/��Сֵ
	uint32_t Wave_Range;		//������ʾ���㱶��
	u16 i;						//
	
	s8 Wave_sum;				//OLED���δ�С
	u8 temp[6];					//ƴ�ֽ�
	u8 str[30];					//�ַ�����ʾ����
	u8 dis_hr=0,dis_spo2=0;
	u8 wave;
	i=0;					//���ó�ʼֵ
	un_max=0;
	un_min=PPG_MIN_COUNT;
	maxim_max30102_read_reg(REG_REV_ID,temp);//��ID,�ɲ�Ҫ
	
	sprintf((char *)str,"  HR:   SpO2:  ");
	OLED_ShowString(0,0,str,16);
	OLED_Refresh_Gram();		//������ʾ��OLED	
	for(i=100;i<500;i++)	//������ǰ��100->Ŀ���Ƕ���500�������е�ǰ��100������,������100���²ɼ���ֵ
	{
		aun_red_buffer[i-100]=aun_red_buffer[i];
		aun_ir_buffer[i-100]=aun_ir_buffer[i];
		
		if(i > 400)
		{
			if(un_min>aun_ir_buffer[i])  
				un_min=aun_ir_buffer[i];	//�ҵ���Сֵ
			if(un_max<aun_ir_buffer[i])
				un_max=aun_ir_buffer[i];	//�ҵ����ֵ
		}
	}
	Wave_Range = un_max-un_min; //���㲨�岨�ȵĲ�ֵ
	Wave_Range = Wave_Range/14; //���㱶��

	for(i=400;i<500;i++)  	//���100�������ݵ�����
	{
		wave++;				//����OLEDˢ���ٶ�
		while(MAX30102_INT==1);
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_ir_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2]; //�ɼ��ͺϲ��õ�ԭʼ���� 
		aun_red_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];  

		// printf("%d\r\n",aun_ir_buffer[i]);
		if(aun_ir_buffer[i] > 5000) //��ָ�ſ�ʱһ�㶼����5000
		{
			if(aun_ir_buffer[i] > un_max) un_max = (aun_ir_buffer[i] + 100); //ˢ�±���
			else if(aun_ir_buffer[i] < un_min)
			{
				un_min = aun_ir_buffer[i] - 200;
				Wave_Range = un_max-un_min;
				Wave_Range = Wave_Range/14;
			}
			
			Wave_sum = (un_max - aun_ir_buffer[i])/Wave_Range; //�õ�OLED��ʾ�Ĳ���ֵ
		}
		else Wave_sum=1;

		if(Wave_sum>16) Wave_sum = 16; 							//��ֵ����,����
		// printf("%d, %d \r\n",aun_ir_buffer[i],aun_red_buffer[i]);
		//printf("%d, %d, %d, %d \r\n",aun_ir_buffer[i],un_max,un_min,Wave_Range);
		if(wave==PPG_WAVE_COUNT)
		{
			OLED_wave(Wave_sum);  								//��ʾ����
			wave=0;
		}
	}

	//ִ���㷨,ȥֱ��,�˲�,���㲨�η�ֵ��
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
	
	// ������ʾ
	if(ch_hr_valid==1 && ch_spo2_valid==1&& n_sp02<100&&n_sp02>70 ) //������ʾ����, ��ѧ������Ȼ��Ӱ��
	{
		dis_hr = n_heart_rate;
		dis_spo2 = n_sp02;
		sprintf((char *)str,"%3d  %3d",dis_hr,dis_spo2);
		OLED_ShowString(0,20,str,24);
	}
	else
	{
		dis_hr = 0;
		dis_spo2 = 0;
		sprintf((char *)str," --   -- ");
		OLED_ShowString(0,20,str,24);
	}
	
	printf("Heart:%3d Spo2 %3d\r\n",dis_hr,dis_spo2);

	OLED_Refresh_Gram();//������ʾ��OLED	

}


void hrv_display()
{
	debug();
	Usart_SendByte(USART2, 0x8a);
	debug();


	//	    OLED_ShowNum(100,48,USART_HRV_RX_BUF[0],3,16,1);	//�ֽ�ͷ
	// 		OLED_ShowNum(10,20,USART_HRV_RX_BUF[65],2,16,1);	//����
	// 		OLED_ShowNum(50,20,USART_HRV_RX_BUF[66],2,16,1);	//Ѫ��
	// 		OLED_ShowNum(100,20,USART_HRV_RX_BUF[67],2,16,1);	//΢ѭ��
	// 		OLED_ShowNum(10,48,USART_HRV_RX_BUF[71],3,16,1);	//����ѹ
	// 		OLED_ShowNum(50,48,USART_HRV_RX_BUF[72],3,16,1);	//����ѹ
	char str[30];
	debug();
	if(senor_Flag)
	{
		sprintf((char *)str,"  :   Low:  ");
		OLED_ShowString(0,0,str,16);
		OLED_Refresh_Gram();		//������ʾ��OLED	

		sprintf((char *)str,"%3d  %3d",USART_HRV_RX_BUF[71],USART_HRV_RX_BUF[72]);
		OLED_ShowString(0,20,str,24);
		OLED_Refresh_Gram();		//������ʾ��OLED	

	}
	 
	printf("hr = %d\r\n",USART_HRV_RX_BUF[65]);
	printf("high pressure = %d\r\n",USART_HRV_RX_BUF[71]);
	printf("low pressure = %d\r\n",USART_HRV_RX_BUF[72]);
	printf("hrv = %d\r\n",USART_HRV_RX_BUF[77]);
	// 
}

