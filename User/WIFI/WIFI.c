/**
  ******************************************************************************
  * @file    WIFI.c
  * @author  KT
  * @version V1.0
  * @date    2017-7-25
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 
#include "WIFI.h"
//#include "timer.h"

u8 *wifi_usart_buf;
u32 wifi_status = 0; 



/**
  *@name WIFIģ�鴮�ڳ�ʼ��
  */
void WIFI_USART_Init(void)
{
	
	/**************��ʼ���ṹ��***********************/
	USART_InitTypeDef USART_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/**************���ô���***************************/
	// �򿪴���GPIO��ʱ��
	WIFI_USART_GPIO_APBxClkCmd(WIFI_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	WIFI_USART_APBxClkCmd(WIFI_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = WIFI_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WIFI_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = WIFI_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WIFI_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = WIFI_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(WIFI_USARTx, &USART_InitStructure);
	/**************************************************/
	
	/**************�����ж�***************************/
	// �����ж����ȼ�����
	  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = WIFI_USART_IRQ ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
	/**************************************************/
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(WIFI_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(WIFI_USARTx, ENABLE);	
	
	
	//	/**************����IO**************************/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	

	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;

	/*���ÿ⺯������ʼ��GPIO*/
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/**************************************************/

}


/**
  *@name WIFIģ��ʹ�ܼ��������ų�ʼ��
  */
//void WIFI_GPIO_Init()
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	/**************����IO**************************/
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
//	/*ѡ��Ҫ���Ƶ�GPIO����*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	

//	/*��������ģʽΪͨ���������*/
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

//	/*������������Ϊ50MHz */   
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

//	/*���ÿ⺯������ʼ��GPIO*/
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
//	
//	/*ѡ��Ҫ���Ƶ�GPIO����*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;

//	/*���ÿ⺯������ʼ��GPIO*/
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	/**************************************************/
//}


/**
  *@name USART3���ڽ��ռ�ⶨʱ����ʼ��
  */
void WIFI_USART_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	

  NVIC_InitTypeDef NVIC_InitStructure; 	
	
	
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	WIFI_USART_TIM_APBxClock_FUN(WIFI_USART_TIM_CLK, ENABLE);	
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=WIFI_USART_TIM_Period;
	// ʱ��Ԥ��Ƶ��
	TIM_TimeBaseStructure.TIM_Prescaler= WIFI_USART_TIM_Prescaler;	
	// ʱ�ӷ�Ƶ���� ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(WIFI_USART_TIM, &TIM_TimeBaseStructure);

	// ����������жϱ�־λ
	TIM_ClearFlag(WIFI_USART_TIM, TIM_FLAG_Update);
	
	// �����������ж�
	TIM_ITConfig(WIFI_USART_TIM,TIM_IT_Update,ENABLE);
	
	
	// �����ж���Ϊ0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = WIFI_USART_TIM_IRQ ;	
	// ���������ȼ�Ϊ 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// ������ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// ʹ�ܼ�����
	TIM_Cmd(WIFI_USART_TIM, ENABLE);
}


/**
  *@name WIFI����
  */
void WIFI_Config(void)
{
	
	WIFI_USART_Init();
//	WIFI_GPIO_Init();
	WIFI_USART_TIM_Init();
	
	WIFI_enable;
  WIFI_Set;
	
}


/**
  *@name WIFIռ�ô���USART3�жϺ���
  */
void WIFI_USART_IRQHandler(void) 
{
  uint8_t ucTemp;
	if(USART_GetITStatus(WIFI_USARTx,USART_IT_RXNE)!=RESET)
	{		
		TIM_ClearITPendingBit(WIFI_USART_TIM , TIM_FLAG_Update);
		
		ucTemp = USART_ReceiveData(WIFI_USARTx);
		wifi_usart_buf[(u16)wifi_status] = ucTemp;
 
		wifi_status++;
    USART_SendData(USART1,ucTemp); 
	}
}


/**
  *@name ����3����жϺ���
  */
void  WIFI_USART_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( WIFI_USART_TIM, TIM_IT_Update) != RESET ) 
	{	
    wifi_status |= 0x80000000;

		TIM_ClearITPendingBit(WIFI_USART_TIM , TIM_FLAG_Update);  		 
	}		 	
}
