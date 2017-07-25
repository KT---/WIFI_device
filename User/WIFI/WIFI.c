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

u8 wifi_usart_buf[255];
u32 wifi_status = 0; 
	
void WIFI_Init(void)
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
	
	/**************����IO**************************/
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
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/**************************************************/
}

void WIFI_Config(void)
{
	
	WIFI_Init();
	
	WIFI_enable;
  WIFI_Set;
	
}

//�����жϺ���
void WIFI_USART_IRQHandler(void) 
{
  uint8_t ucTemp;
	if(USART_GetITStatus(WIFI_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(WIFI_USARTx);
		wifi_usart_buf[wifi_status] = ucTemp;
		if((wifi_usart_buf[wifi_status] == 0x0a) && (wifi_usart_buf[wifi_status-1] == 0x0d))
		{
			wifi_status |= 0x80000000;
			wifi_status &= 0xFFFF0000;
		}
		wifi_status++;
//    USART_SendData(USART1,(u8)wifi_status); 
	}
}
