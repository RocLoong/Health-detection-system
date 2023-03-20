#include "sys.h"
#include "usart.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
void USART_Config(void)
{
	uart_init(115200);
	uart_hrv_init();
}
	  
//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART_RX_STA=0;       //����״̬���	  

void uart_init(u32 bound){
    //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

/*-----------------------------------usart2 config -------------------------------------------*/
// �������ڳ�ʼ��
void uart_hc05_init(u32 bound){
    //GPIO�˿�����
  	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART3);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

   //Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������
    //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}
/*-----------------------------------usart3 config -------------------------------------------*/


u8 USART_HRV_RX_BUF[USART_HRV_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_HRV_RX_STA=0;       //����״̬���	  
u8 senor_Flag=0;

// Ѫѹ���ڳ�ʼ��
void uart_hrv_init(){
    USART_InitTypeDef USART_InitStructure;    
  	NVIC_InitTypeDef NVIC_InitStructure;     
    GPIO_InitTypeDef GPIO_InitStructure;    //����һ���ṹ�������������ʼ��GPIO    
   //ʹ�ܴ��ڵ�RCCʱ��    
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //ʹ��UART3����GPIOB��ʱ��    
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);    

   //����ʹ�õ�GPIO������    
   // Configure USART3 Rx (PB.11) as input floating      
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
   GPIO_Init(GPIOB, &GPIO_InitStructure);    

   // Configure USART3 Tx (PB.10) as alternate function push-pull    
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;    
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
   GPIO_Init(GPIOB, &GPIO_InitStructure);    

   //���ô���    
   USART_InitStructure.USART_BaudRate = 38400;    
   USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
   USART_InitStructure.USART_StopBits = USART_StopBits_1;    
   USART_InitStructure.USART_Parity = USART_Parity_No;    
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    

   // Enable the USART3 Interrupt     
   NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;    
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��  
   NVIC_Init(&NVIC_InitStructure);    

 // Configure USART3     
   USART_Init(USART3, &USART_InitStructure);//���ô���3    

  // Enable USART1 Receive interrupts ʹ�ܴ��ڽ����ж�    
   USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);    
   //���ڷ����ж��ڷ�������ʱ����    
   //USART_ITConfig(USART2, USART_IT_TXE, ENABLE);    

   // Enable the USART3     
   USART_Cmd(USART3, ENABLE);//ʹ�ܴ���3    

}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
	/* �ȴ��������ݼĴ���Ϊ�� */

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
		Res =USART_ReceiveData(USART3);						//(USART1->DR);	//��ȡ���յ�������
		if (Res==(0xff)) 									//�����ݰ������ֽ�Ϊ0xff,���������,������
		{
			flag = 1;
		}
		if(flag)
		{
			USART_HRV_RX_BUF[USART_HRV_RX_STA]=Res;					//ÿ�ν��ж������︳ֵ������
			// printf("[debug] %s  %d:  Res = %x\r\n",__FILE__, __LINE__,Res);																	
			// printf("[debug] %s  %d:  buffer_id = %x\r\n",__FILE__, __LINE__,USART_HRV_RX_STA);																	

			USART_HRV_RX_STA++;	
		}
		if(USART_HRV_RX_STA>(USART_HRV_LEN-1))					//�ﵽ76�ֽ�ʱ,�ص������1λ
		{
			USART_HRV_RX_STA=0;
			flag= 0;
			senor_Flag=1;
			dump_buffer();
		}   		 
     } 
}

