#include "sys.h"
#include "usart.h"	 
#include "debug.h" 
////////////////////////////////////////////////////////////////////////////////// 	 
void USART_Config(void)
{
	uart_init(115200);
	uart_hrv_init();
	uart_hc05_init(9600);
}
	  
//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART_RX_STA=0;       //接收状态标记	  

void uart_init(u32 bound){
    //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

   //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}

/*-----------------------------------usart2 config -------------------------------------------*/

u8 HC05_RECV_BUF[HC05_MAX_RECV_LEN];
u8 HC05_SEND_BUF[HC05_MAX_SEND_LEN];
u16 HC05_RX_STA;   					
// 蓝牙串口初始化
void uart_hc05_init(u32 bound){
    //GPIO端口设置
  	  USART_InitTypeDef USART_InitStructure;    
    NVIC_InitTypeDef NVIC_InitStructure;     
    GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO    
   //使能串口的RCC时钟    
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //使能UART3所在GPIOB的时钟    
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);    

   //串口使用的GPIO口配置    
   // Configure USART2 Rx (PA3) as input floating      
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
   GPIO_Init(GPIOA, &GPIO_InitStructure);    

   // Configure USART2 Tx (PA2) as alternate function push-pull    
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;    
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
   GPIO_Init(GPIOA, &GPIO_InitStructure);    

   //配置串口    
   USART_InitStructure.USART_BaudRate = bound;    
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
   USART_InitStructure.USART_StopBits = USART_StopBits_1;    
   USART_InitStructure.USART_Parity = USART_Parity_No;    
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    


   // Configure USART3     
   USART_Init(USART2, &USART_InitStructure);//配置串口3    

  // Enable USART1 Receive interrupts 使能串口接收中断    
   USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);    
   //串口发送中断在发送数据时开启    
   //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);    

   // Enable the USART3     
   USART_Cmd(USART2, ENABLE);//使能串口3    

   //串口中断配置    
   //Configure the NVIC Preemption Priority Bits       
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);    

   // Enable the USART3 Interrupt     
   NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;    
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
   NVIC_Init(&NVIC_InitStructure);    
}

void USART2_IRQHandler(void)
{

  // u8 Res;
  // if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
  // {
  //   printf("[debug] %s  %d:  Res = %x\r\n",__FILE__, __LINE__,Res);																	
  //   Res =USART_ReceiveData(USART2);	//读取接收到的数据
  //   USART_SendData(USART1,Res);		//往串口1发送数据
  // }
}


void hc05_send_str(const unsigned char *p_str)
{
  while(*p_str != '\0')
  {
    hc05_send_byte(*p_str);
    p_str++;
  }
}

void hc05_send_byte(uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(USART2,ch);
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	

}
/*-----------------------------------usart3 config -------------------------------------------*/


u8 USART_HRV_RX_BUF[USART_HRV_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART_HRV_RX_STA=0;       //接收状态标记	  
u8 senor_Flag=0;

// 血压串口初始化
void uart_hrv_init(){
    USART_InitTypeDef USART_InitStructure;    
  	NVIC_InitTypeDef NVIC_InitStructure;     
    GPIO_InitTypeDef GPIO_InitStructure;    //声明一个结构体变量，用来初始化GPIO    
   //使能串口的RCC时钟    
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能UART3所在GPIOB的时钟    
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);    

   //串口使用的GPIO口配置    
   // Configure USART3 Rx (PB.11) as input floating      
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
   GPIO_Init(GPIOB, &GPIO_InitStructure);    

   // Configure USART3 Tx (PB.10) as alternate function push-pull    
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
   GPIO_Init(GPIOB, &GPIO_InitStructure);    

   //配置串口    
   USART_InitStructure.USART_BaudRate = 38400;    
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
   USART_InitStructure.USART_StopBits = USART_StopBits_1;    
   USART_InitStructure.USART_Parity = USART_Parity_No;    
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    

   // Enable the USART3 Interrupt     
   NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;    
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能  
   NVIC_Init(&NVIC_InitStructure);    

   // Configure USART3     
   USART_Init(USART3, &USART_InitStructure);//配置串口3    

   // Enable USART1 Receive interrupts 使能串口接收中断    
   USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
   //串口发送中断在发送数据时开启    
   //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);    

   // Enable the USART3     
   USART_Cmd(USART3, ENABLE);//使能串口3    

}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
	/* 等待发送数据寄存器为空 */

	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

static void dump_buffer()
{
	printf("\r\n");
	for(int i = 0; i < USART_HRV_LEN; i++)
	{
		printf("%d,",USART_HRV_RX_BUF[i]);
	}
	printf("\r\n");
	printf("hr = %d\r\n",USART_HRV_RX_BUF[65]);
	printf("high pressure = %d\r\n",USART_HRV_RX_BUF[71]);
	printf("low pressure = %d\r\n",USART_HRV_RX_BUF[72]);
	printf("hrv = %d\r\n",USART_HRV_RX_BUF[76]);
}

void USART3_IRQHandler(void)
{
	u8 Res;
	static u8 flag;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
    USART_ClearITPendingBit(USART3,USART_IT_RXNE);
    Res =USART_ReceiveData(USART3);						//(USART1->DR);	//读取接收到的数据
    if (Res==(0xff)) 									//当数据包的首字节为0xff,则继续接收,否则丢弃
    {
      flag = 1;
    }
    if(flag)
    {
      USART_HRV_RX_BUF[USART_HRV_RX_STA]=Res;					//每次进中断在这里赋值给数组
      // printf("[debug] %s  %d:  Res = %x\r\n",__FILE__, __LINE__,Res);																	
      // printf("[debug] %s  %d:  buffer_id = %x\r\n",__FILE__, __LINE__,USART_HRV_RX_STA);																	

      USART_HRV_RX_STA++;	
    }
    if(USART_HRV_RX_STA>(USART_HRV_LEN-1))					//达到76字节时,回到数组第1位
    {
      USART_HRV_RX_STA=0;
      flag= 0;
      senor_Flag=1;
      dump_buffer();
    }   		 
  } 
}

