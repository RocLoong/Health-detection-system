#include "hc05.h"
#include "usart.h"
#if 1
//初始化HC05模块
//返回值:0,成功;1,失败.
u8 HC05_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_STATE,ENABLE);	//使能PORTA C时钟	

    GPIO_InitStructure.GPIO_Pin = STATE_Pin;				 // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化PA4

	uart_hc05_init(38400);	//初始化串口2 38400波特率.
	
	hc05_send_str("AT+NAME=HC05\r\n");  // 设置蓝牙的名称为hc05
	hc05_send_str("AT+PIN=1234\r\n");  // 设置蓝牙的密码为1234
	hc05_send_str("AT+ROLE=1\r\n");    // 设置蓝牙的角色为从机
	hc05_send_str("AT+CMODE=0\r\n"); 	   // 蓝牙连接模式为任意地址连接模式
	hc05_send_str("AT+UART=38400,0,0\r\n");     // 蓝牙通信串口波特率为1152，停止位1位，无校验位（以后正常模式都会采用这个波特率来进行通讯，AT 模式依然是 38400）
	hc05_send_str("AT+RMAAD\r\n");     // 清空配对列表
	
    // u8 retry=10,t;	  		 
    // u8 temp=1;


    

    // GPIO_InitStructure.GPIO_Pin = EN_Pin;				 // 端口配置
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    // GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化PC4

    // GPIO_SetBits(GPIOC,EN_Pin);

    // USART2_Init(9600);	//初始化串口2为:9600,波特率.

    // delay_ms(1500);

    // while(retry--)
    // {
    //     HC05_EN=1;					//KEY置高,进入AT模式
    //     delay_ms(10);

    //     u2_printf("AT\r\n");		//发送AT测试指令
    //     HC05_EN=0;					//KEY拉低,退出AT模式
    //     for(t=0;t<10;t++) 			//最长等待50ms,来接收HC05模块的回应
    //     {
    //         if(HC05_RX_STA&0X8000)break;
    //         delay_ms(5);
    //     }		
    //     if(HC05_RX_STA&0X8000)	//接收到一次数据了
    //     {
    //         temp=HC05_RX_STA&0X7FFF;	//得到数据长度
    //         HC05_RX_STA=0;			 
    //         if(temp==4&&HC05_RECV_BUF[0]=='O'&&HC05_RECV_BUF[1]=='K')
    //         {
    //             temp=0;//接收到OK响应
    //             break;
    //         }
    //     }			    		
    // }		    
    // if(retry==0)temp=1;	//检测失败
    // return temp;	 
}
//获取ATK-HC05模块的角色
//返回值:0,从机;1,主机;0XFF,获取失败.							  
u8 HC05_Get_Role(void)
{	 		    
	// u8 retry=0X0F;
	// u8 temp,t;
	// while(retry--)
	// {
	// 	HC05_EN=1;					//KEY置高,进入AT模式
	// 	delay_ms(10);
	// 	u2_printf("AT+ROLE?\r\n");	//查询角色
	// 	for(t=0;t<20;t++) 			//最长等待200ms,来接收HC05模块的回应
	// 	{
	// 		delay_ms(10);
	// 		if(HC05_RX_STA&0X8000)break;
	// 	}		
	// 	HC05_EN=0;					//KEY拉低,退出AT模式
	// 	if(HC05_RX_STA&0X8000)	//接收到一次数据了
	// 	{
	// 		temp=HC05_RX_STA&0X7FFF;	//得到数据长度
	// 		HC05_RX_STA=0;			 
	// 		if(temp==13&&HC05_RECV_BUF[0]=='+')//接收到正确的应答了
	// 		{
	// 			temp=HC05_RECV_BUF[6]-'0';//得到主从模式值
	// 			break;
	// 		}
	// 	}		
	// }
	// if(retry==0)temp=0XFF;//查询失败.
	// return temp;
} 			
//ATK-HC05设置命令
//此函数用于设置ATK-HC05,适用于仅返回OK应答的AT指令
//atstr:AT指令串.比如:"AT+RESET"/"AT+UART=9600,0,0"/"AT+ROLE=0"等字符串
//返回值:0,设置成功;其他,设置失败.							  
u8 HC05_Set_Cmd(u8* atstr)
{	 		    
	// u8 retry=0X0F;
	// u8 temp,t;
	// while(retry--)
	// {
	// 	HC05_EN=1;					//KEY置高,进入AT模式
	// 	delay_ms(10);
	// 	u2_printf("%s\r\n",atstr);	//发送AT字符串
	// 	HC05_EN=0;					//KEY拉低,退出AT模式
	// 	for(t=0;t<20;t++) 			//最长等待100ms,来接收HC05模块的回应
	// 	{
	// 		if(HC05_RX_STA&0X8000)break;
	// 		delay_ms(5);
	// 	}		
	// 	if(HC05_RX_STA&0X8000)	//接收到一次数据了
	// 	{
	// 		temp=HC05_RX_STA&0X7FFF;	//得到数据长度
	// 		HC05_RX_STA=0;			 
	// 		if(temp==4&&HC05_RECV_BUF[0]=='O')//接收到正确的应答了
	// 		{			
	// 			temp=0;
	// 			break;			 
	// 		}
	// 	}		
	// }
	// if(retry==0)temp=0XFF;//设置失败.
	// return temp;
} 
#endif