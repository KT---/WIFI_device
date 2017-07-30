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

u8 wifi_usart_buf[1024] = {0};
u32 wifi_status; 
u16 TIM_Multi = 0;


/**********************************************************/

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

}
/**********************************************************/


/**
  *@name WIFIģ��ʹ�ܼ��������ų�ʼ��
  */
void WIFI_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
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
/**********************************************************/


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
	TIM_Cmd(WIFI_USART_TIM, DISABLE);
}
/**********************************************************/


/**
  *@name WIFI����
  */
void WIFI_Config(void)
{
	
	WIFI_USART_Init();
	WIFI_GPIO_Init();
	WIFI_USART_TIM_Init();
	
//	WIFI_enable;
//  WIFI_Set;
	WIFI_DataHandler( WIFI_Restart);
	Delay_ms(10);
	WIFI_DataHandler( WIFI_JoinAP);
	Delay_ms(10);
	WIFI_DataHandler( WIFI_TcpConnet);
	Delay_ms(10);
	WIFI_DataHandler( WIFI_Mode);
	Delay_ms(10);
	WIFI_DataHandler( WIFI_Send);
	Delay_ms(10);
	WIFI_DataHandler( WIFI_POST);
//	Delay_ms(10);
//	WIFI_DataHandler( WIFI_Restart);
}
/**********************************************************/


u32 WIFI_DataHandler( WIFI_HandlerType type)
{
	u32 w_time = 0;
//	AT_cmd JoinAP;
#if 0
	u8 AT_CWJAP[]="AT+CWJAP=\"CandyTime_7ED191\",\"ktlovejackiemilktea&coffee\"\r\n";
	u8 AT_CIPSTART[]="AT+CIPSTART=\"TCP\",\"192.168.100.15\",80\r\n";
#else
	u8 AT_CWJAP[]="AT+CWJAP=\"KLX\",\"klx39909679203\"\r\n";
	u8 AT_CIPSTART[]="AT+CIPSTART=\"TCP\",\"192.168.199.145\",80\r\n";
#endif
	u8 AT_CIPMODE[]="AT+CIPMODE=1\r\n";
	u8 AT_CIPSEND[]="AT+CIPSEND\r\n";
	u8 POST_HEARD[]="POST http://localhost/LoginTest/connection-test.php HTTP/1.0\r\n";
	u8 POST_TYPE[]="Content-Type: application/x-www-form-urlencoded\r\n";
	u8 POST_LENGTH[]="Content-Length:47\r\n\r\n";
	u8 POST_CONTENT[]="Device_Num=001&CARD_Num=1234567890&Quantity=100\r\n";
	switch(type)
	{
		case WIFI_Restart:
					WIFI_disable;
					WIFI_Reset;
					Delay_ms(100);
					WIFI_enable;
					WIFI_Set;
					while(!WIFI_rx_get)
					{
						Delay_us(1000);
						if(++w_time > 5000)
						{
							printf("WIFIģ���ʼ����ʱ\r\n\r\n");
							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
							WIFI_rx_get_rst;
							return 0;
						}
					}
					if(Usart_buf_fine_char((u8 *)("ready")))
					{
						printf("WIFIģ��ready\r\n\r\n");
						clear_buffer(wifi_usart_buf,WIFI_buffer_len);
						WIFI_rx_get_rst;
						return 1;
					}
					else
					{
						printf("WIFIģ���ʼ��ʧ��\r\n\r\n");
						clear_buffer(wifi_usart_buf,WIFI_buffer_len);
						WIFI_rx_get_rst;
						return 0;
					}
//					break;
			
		case WIFI_JoinAP:
				  Usart_SendString(WIFI_USARTx,AT_CWJAP,strlen((const char *)AT_CWJAP));
					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("OK"))))
					{
						Delay_us(1000);
						if(++w_time > 20000)
						{
							printf("AP���ӳ�ʱ\r\n\r\n");
							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
							WIFI_rx_get_rst;
							return 0;
						}
					}
					printf("AP���ӳɹ�\r\n\r\n");
					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
					WIFI_rx_get_rst;
					return 1;
//					break;
		case WIFI_TcpConnet:
					Usart_SendString(WIFI_USARTx,AT_CIPSTART,strlen((const char *)AT_CIPSTART));
					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("OK"))))
					{
						Delay_us(1000);
						if(++w_time > 5000)
						{
							printf("TCP���ӳ�ʱ\r\n\r\n");
							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
							WIFI_rx_get_rst;
							return 0;
						}
					}
					printf("TCP���ӳɹ�\r\n\r\n");
					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
					WIFI_rx_get_rst;
					return 1;
	  case WIFI_Mode:
					Usart_SendString(WIFI_USARTx,AT_CIPMODE,strlen((const char *)AT_CIPMODE));
					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("OK"))))
					{
						Delay_us(1000);
						if(++w_time > 2000)
						{
							printf("MODE����ʧ��\r\n\r\n");
							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
							WIFI_rx_get_rst;
							return 0;
						}
					}
					printf("MODE���óɹ�\r\n\r\n");
					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
					WIFI_rx_get_rst;
					return 1;
	  case WIFI_Send:
			    Usart_SendString(WIFI_USARTx,AT_CIPSEND,strlen((const char *)AT_CIPSEND));
					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("OK"))))
					{
						Delay_us(1000);
						if(++w_time > 3000)
						{
							printf("����͸��ʧ��\r\n\r\n");
							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
							WIFI_rx_get_rst;
							return 0;
						}
					}
					printf("����͸���ɹ�\r\n\r\n");
					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
					WIFI_rx_get_rst;
					return 1;

		case WIFI_POST:
				  Usart_SendString(WIFI_USARTx,POST_HEARD,strlen((const char *)POST_HEARD));
				  Usart_SendString(WIFI_USARTx,POST_TYPE,strlen((const char *)POST_TYPE));
				  Usart_SendString(WIFI_USARTx,POST_LENGTH,strlen((const char *)POST_LENGTH));
				  Usart_SendString(WIFI_USARTx,POST_CONTENT,strlen((const char *)POST_CONTENT));
					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("hello device"))))
					{
						Delay_us(1000);
						if(++w_time > 10000)
						{
							printf("POST����ʧ��\r\n\r\n");
							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
							WIFI_rx_get_rst;
							return 0;
						}
					}
					printf("POST����ɹ�\r\n\r\n");
					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
					WIFI_rx_get_rst;
					return 1;

		default:break;
	}
	return 0;
}


void clear_buffer(u8 *buffer,u16 length)
{
	u16 i;
	for( i = 0;i < length;i++)
	{
		buffer[i] = 0;
	}
}


/**
  *@name ����buffer�ж��Ƿ����ĳ�ֶ�
  */
u16 Usart_buf_fine_char(u8 *target_char)
{
	u16 i,j;
//	u8 ready[5]={0x72,0x65,0x61,0x64,0x79};
	for(i=0;i<=(wifi_status & 0x0000FFFF);i++)
	{
		if(wifi_usart_buf[i] == target_char[0])
		{
			for(j=1;j<(sizeof(target_char)/sizeof(target_char[0]));j++)
			{
				if(wifi_usart_buf[i+j] !=  target_char[j])
				{
					break;
				}
				return i+1;
			}
		}
	}
	return 0;
}


/**
  *@name WIFIռ�ô���USART3�жϺ���
  */
void WIFI_USART_IRQHandler(void) 
{
  uint8_t ucTemp;
	if(USART_GetITStatus(WIFI_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(WIFI_USARTx);

#if 1
		wifi_usart_buf[(u16)wifi_status] = ucTemp;
		wifi_status++;
	
		TIM_Cmd(WIFI_USART_TIM, ENABLE);
		TIM_Multi = 0;
		TIM_ClearITPendingBit(WIFI_USART_TIM , TIM_FLAG_Update);
#endif
		
#if 1
    USART_SendData(DEBUG_USARTx,ucTemp); 
#endif
	}
}


/**
  *@name ����3����жϺ���
  */
void  WIFI_USART_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( WIFI_USART_TIM, TIM_IT_Update) != RESET ) 
	{	
		if(TIM_Multi >= 500)
		{
			wifi_status |= 0x80000000;
	//		wifi_status --;
//			USART_ITConfig(WIFI_USARTx, USART_IT_RXNE, DISABLE);
			TIM_Cmd(WIFI_USART_TIM, DISABLE);
		}
		TIM_ClearITPendingBit(WIFI_USART_TIM , TIM_FLAG_Update); 
 		TIM_Multi++;
	}		 	
}
