/*************************************************************************
 程序使用正点原子的标准库框架, 仅供学习使用
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




uint32_t aun_ir_buffer[500]; 	//IR数组,红外光原始数据缓存区
uint32_t aun_red_buffer[500];  	//Red数组,红光原始数据缓存区
int32_t n_ir_buffer_length;    	//数据长度
	
int32_t n_sp02; 				//血氧值
int32_t n_heart_rate;   		//心率值
	
int8_t ch_spo2_valid;   		//算法执行成功标志位
int8_t ch_hr_valid;    			

extern u8 senor_Flag;
u8 high_pressure = 0,low_pressure = 0,hrv = 0;

int main(void)
{ 
	NVIC_Configuration();     	//各函数初始化
	delay_init();	    		  
	USART_Config();
	LED_Init();
	OLED_Init();
	max30102_init();

	// 开启血压检测模块
	Usart_SendByte(USART3, 0x8a);
	
	while(1)
	{
		// max30102_display();
		hrv_display();
		delay_ms(100);
	}
}





void max30102_display()
{
	uint32_t un_min, un_max; 	//原始数据的最大/最小值
	uint32_t Wave_Range;		//用于显示计算倍率
	u16 i;						//
	
	s8 Wave_sum;				//OLED波形大小
	u8 temp[6];					//拼字节
	u8 str[30];					//字符串显示变量
	u8 dis_hr=0,dis_spo2=0;
	u8 wave;
	i=0;					//重置初始值
	un_max=0;
	un_min=PPG_MIN_COUNT;
	maxim_max30102_read_reg(REG_REV_ID,temp);//读ID,可不要
	
	sprintf((char *)str,"  HR:   SpO2:  ");
	OLED_ShowString(0,0,str,16);
	OLED_Refresh_Gram();		//更新显示到OLED	
	for(i=100;i<500;i++)	//数组往前移100->目的是丢弃500个数组中的前面100个数组,并更新100个新采集的值
	{
		aun_red_buffer[i-100]=aun_red_buffer[i];
		aun_ir_buffer[i-100]=aun_ir_buffer[i];
		
		if(i > 400)
		{
			if(un_min>aun_ir_buffer[i])  
				un_min=aun_ir_buffer[i];	//找到最小值
			if(un_max<aun_ir_buffer[i])
				un_max=aun_ir_buffer[i];	//找到最大值
		}
	}
	Wave_Range = un_max-un_min; //计算波峰波谷的差值
	Wave_Range = Wave_Range/14; //计算倍率

	for(i=400;i<500;i++)  	//填充100个新数据到数组
	{
		wave++;				//减慢OLED刷新速度
		while(MAX30102_INT==1);
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_ir_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2]; //采集和合并得到原始数据 
		aun_red_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];  

		// printf("%d\r\n",aun_ir_buffer[i]);
		if(aun_ir_buffer[i] > 5000) //手指放开时一般都大于5000
		{
			if(aun_ir_buffer[i] > un_max) un_max = (aun_ir_buffer[i] + 100); //刷新倍率
			else if(aun_ir_buffer[i] < un_min)
			{
				un_min = aun_ir_buffer[i] - 200;
				Wave_Range = un_max-un_min;
				Wave_Range = Wave_Range/14;
			}
			
			Wave_sum = (un_max - aun_ir_buffer[i])/Wave_Range; //得到OLED显示的波形值
		}
		else Wave_sum=1;

		if(Wave_sum>16) Wave_sum = 16; 							//数值过大,纠正
		// printf("%d, %d \r\n",aun_ir_buffer[i],aun_red_buffer[i]);
		//printf("%d, %d, %d, %d \r\n",aun_ir_buffer[i],un_max,un_min,Wave_Range);
		if(wave==PPG_WAVE_COUNT)
		{
			OLED_wave(Wave_sum);  								//显示波形
			wave=0;
		}
	}

	//执行算法,去直流,滤波,计算波形幅值等
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
	
	// 数据显示
	if(ch_hr_valid==1 && ch_spo2_valid==1&& n_sp02<100&&n_sp02>70 ) //划定显示区间, 光学会受自然光影响
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

	OLED_Refresh_Gram();//更新显示到OLED	

}


void hrv_display()
{
	//	    OLED_ShowNum(100,48,USART_HRV_RX_BUF[0],3,16,1);	//字节头
	// 		OLED_ShowNum(10,20,USART_HRV_RX_BUF[65],2,16,1);	//心率
	// 		OLED_ShowNum(50,20,USART_HRV_RX_BUF[66],2,16,1);	//血氧
	// 		OLED_ShowNum(100,20,USART_HRV_RX_BUF[67],2,16,1);	//微循环
	// 		OLED_ShowNum(10,48,USART_HRV_RX_BUF[71],3,16,1);	//收缩压
	// 		OLED_ShowNum(50,48,USART_HRV_RX_BUF[72],3,16,1);	//舒张压
	char str[30];
	if(USART_HRV_RX_BUF[71] != 0) high_pressure = USART_HRV_RX_BUF[71];
	if(USART_HRV_RX_BUF[72] != 0) low_pressure = USART_HRV_RX_BUF[72];
	if(USART_HRV_RX_BUF[72] != 0) hrv =  USART_HRV_RX_BUF[76];
	sprintf((char *)str," High :   Low:  ");
	OLED_ShowString(0,0,str,16);
	OLED_Refresh_Gram();		//更新显示到OLED	
	if(senor_Flag)
	{
		senor_Flag=0;
		sprintf((char *)str,"%3d  %3d",high_pressure,low_pressure);
		OLED_ShowString(0,20,str,24);
		OLED_Refresh_Gram();		//更新显示到OLED	
	}
	

}

