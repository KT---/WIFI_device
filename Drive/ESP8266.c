/**
  ******************************************************************************
  * @file   
  * @author  KT
  * @version V1.0
  * @date    2017
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 
#include "ESP8266.h"


u8 wifi_usart_buf[1024] = {0};
u32 wifi_status; 
u16 TIM_Multi = 0;

/**********************************************************/


/**
  *@name WIFIģ�鴮�ڳ�ʼ��
  */
void WIFI_ESP8266_USART_Init(void)
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
void WIFI_ESP8266_GPIO_Init(void)
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


/****************************************************************************
  *@name WIFI���ڼ�ⶨʱ����ʼ��
  ***************************************************************************/
void WIFI_ESP8266_USART_TIM_Init(void)
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
/****************************************************************************/


/**
  *@name ESP8266Ӳ����
  */
u8 WIFI_ESP8266_Restart(void)
{
	u32 w_time = 0;
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
			return OT;
		}
	}
	if(Usart_buf_find_char((u8 *)("ready")))
	{
		printf("WIFIģ��ready\r\n\r\n");
		clear_buffer(wifi_usart_buf,WIFI_buffer_len);
		WIFI_rx_get_rst;
		return OK;
	}
	else
	{
		printf("WIFIģ���ʼ��ʧ��\r\n\r\n");
		clear_buffer(wifi_usart_buf,WIFI_buffer_len);
		WIFI_rx_get_rst;
		return ERR;
	}
}
/****************************************************************************/


/**
  *@name ����AP
  */
u8 WIFI_ESP8266_JoinAP(u8 *ssid,u8 *password)
{
	u32 w_time = 0;
	WIFI_Generic_CmdSend("AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
	while(!(WIFI_rx_get && Usart_buf_find_char((u8 *)("OK"))))
	{
		Delay_us(1000);
		if(++w_time > 20000)
		{
			printf("AP���ӳ�ʱ\r\n\r\n");
			clear_buffer(wifi_usart_buf,WIFI_buffer_len);
			WIFI_rx_get_rst;
			return OT;
		}
		if(Usart_buf_find_char((u8 *)("ERROR")))
		{
			printf("AP����ʧ��\r\n\r\n");
			clear_buffer(wifi_usart_buf,WIFI_buffer_len);
			WIFI_rx_get_rst;
			return ERR;
		}
	}
	printf("AP���ӳɹ�\r\n\r\n");
	clear_buffer(wifi_usart_buf,WIFI_buffer_len);
	WIFI_rx_get_rst;
	return OK;
}
/****************************************************************************/


/**
	* @name��itoa
	* @describe ������������ת�����ַ���,��WIFI_Generic_CmdSend()����
	*/
static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}
	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}
	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';
		/* Make the value positive. */
		value *= -1;
	}
	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;
		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}
	/* Null terminate the string. */
	*ptr = 0;
	return string;
} /* NCL_Itoa */
/****************************************************************************/


/**
	* @name��WIFI_Generic_CmdSend
	* @describe ��WIFIָ���ʽ�������ͨ�������������
	*/
void WIFI_Generic_CmdSend(char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(WIFI_USARTx, 0x0d);
					Data ++;
				break;
				case 'n':							          //���з�
					USART_SendData(WIFI_USARTx, 0x0a);	
					Data ++;
				break;
				default:
					Data ++;
				break;
			}			 
		}
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						USART_SendData(WIFI_USARTx,*s);
						while( USART_GetFlagStatus(WIFI_USARTx, USART_FLAG_TXE) == RESET );
					}				
					Data++;				
				break;
				case 'd':			
					//ʮ����
					d = va_arg(ap, int);					
					itoa(d, buf, 10);					
					for (s = buf; *s; s++) 
					{
						USART_SendData(WIFI_USARTx,*s);
						while( USART_GetFlagStatus(WIFI_USARTx, USART_FLAG_TXE) == RESET );
					}					
					Data++;				
				break;				
				default:
					Data++;				
				break;				
			}		 
		}		
		else USART_SendData(WIFI_USARTx, *Data++);
		while ( USART_GetFlagStatus ( WIFI_USARTx, USART_FLAG_TXE ) == RESET );
	}
}
/****************************************************************************/


/******************************************************************************
  *@name ����buffer�ж��Ƿ����ĳ�ֶ�
  *****************************************************************************/
u16 Usart_buf_find_char(u8 *target_char)
{
	u16 i,j;
//	u8 ready[5]={0x72,0x65,0x61,0x64,0x79};
	for(i=0;i<=WIFI_buffer_len;i++)
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
/****************************************************************************/


/****************************************************************************
  *@name WIFIռ�ô���USART3�жϺ���
  ***************************************************************************/
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
/****************************************************************************/


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
/****************************************************************************/


/**
  *@name buffer������պ���
  */
void clear_buffer(u8 *buffer,u16 length)
{
	u16 i;
	for( i = 0;i < length;i++)
	{
		buffer[i] = 0;
	}
}
/****************************************************************************/
