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
u8 *Request_buf;
u32 wifi_status; 
u16 TIM_Multi = 0;

/**********************************************************/


/**
  *@name WIFI模块串口初始化
  */
void WIFI_ESP8266_USART_Init(void)
{
	
	/**************初始化结构体***********************/
	USART_InitTypeDef USART_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/**************配置串口***************************/
	// 打开串口GPIO的时钟
	WIFI_USART_GPIO_APBxClkCmd(WIFI_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	WIFI_USART_APBxClkCmd(WIFI_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = WIFI_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WIFI_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = WIFI_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(WIFI_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = WIFI_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(WIFI_USARTx, &USART_InitStructure);
	/**************************************************/
	
	/**************配置中断***************************/
	// 串口中断优先级配置
	  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = WIFI_USART_IRQ ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	/**************************************************/
	// 使能串口接收中断
	USART_ITConfig(WIFI_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(WIFI_USARTx, ENABLE);	

}
/**********************************************************/


/**
  *@name WIFI模块使能及重置引脚初始化
  */
void WIFI_ESP8266_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/**************配置IO**************************/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	

	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化GPIO*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	/*选择要控制的GPIO引脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;

	/*调用库函数，初始化GPIO*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/**************************************************/
}
/**********************************************************/


/****************************************************************************
  *@name WIFI串口监测定时器初始化
  ***************************************************************************/
void WIFI_ESP8266_USART_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	

  NVIC_InitTypeDef NVIC_InitStructure; 	
	
	
		// 开启定时器时钟,即内部时钟CK_INT=72M
	WIFI_USART_TIM_APBxClock_FUN(WIFI_USART_TIM_CLK, ENABLE);	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=WIFI_USART_TIM_Period;
	// 时钟预分频数
	TIM_TimeBaseStructure.TIM_Prescaler= WIFI_USART_TIM_Prescaler;	
	// 时钟分频因子 ，没用到不用管
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(WIFI_USART_TIM, &TIM_TimeBaseStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(WIFI_USART_TIM, TIM_FLAG_Update);
	
	// 开启计数器中断
	TIM_ITConfig(WIFI_USART_TIM,TIM_IT_Update,ENABLE);
	
	
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = WIFI_USART_TIM_IRQ ;	
	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// 使能计数器
	TIM_Cmd(WIFI_USART_TIM, DISABLE);
}
/****************************************************************************/


/**
  *@name ESP8266硬重启
  */
u8 WIFI_ESP8266_Restart(void)
{
	WIFI_disable;
	WIFI_Reset;
	Delay_ms(100);
	WIFI_enable;
	WIFI_Set;
	return WIFI_FeedbackProcess("ready","ERROR", 5000,"WIFI模块启动");
}
/****************************************************************************/


/**
  *@name 加入AP
  */
u8 WIFI_ESP8266_JoinAP(const char *ssid,const char *password)
{
	WIFI_Generic_CmdSend("AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);
	return WIFI_FeedbackProcess("OK","FAIL", 15000,"AP连接");
}
/****************************************************************************/


/**
  *@name TCP/UDP连接
  */
u8 WIFI_TcpUdp_Connet(const char *ConnetType,const char *IPaddr,const u32 PortNum)
{
	WIFI_Generic_CmdSend("AT+CIPSTART=\"%s\",\"%s\",%d\r\n",ConnetType,IPaddr,PortNum);
  return WIFI_FeedbackProcess("OK","ERROR", 5000,"TCP连接");
}
/****************************************************************************/


/**
  *@name 设置透传模式
  */
u8 WIFI_Send_ModeSet(const char MODE)
{
	WIFI_Generic_CmdSend("AT+CIPMODE=%d\r\n",MODE);
  return WIFI_FeedbackProcess("OK","ERROR", 5000,"透传模式设置");
}
/****************************************************************************/


/**
  *@name POST请求及数据返回处理
  */
u8 WIFI_PostRequest(const char *URL,const u16 value_length,const char *Value,const char *FB_KeyWord)
{
	u32 w_time = 0;
	
	WIFI_Generic_CmdSend("POST %s HTTP/1.0\r\n",URL);
	WIFI_Generic_CmdSend("Content-Type: application/x-www-form-urlencoded\r\n");
	WIFI_Generic_CmdSend("Content-Length:%d\r\n\r\n",value_length);
	WIFI_Generic_CmdSend("%s",Value);
	
	while(!(WIFI_rx_get && Usart_buf_find_char((u8 *)("200 OK"))))
	{
		Delay_us(1000);
		if(++w_time >= 10000)
		{
			printf("POST请求超时:408\r\n\r\n");
			WIFI_Generic_CmdSend("+++");
			clear_buffer(wifi_usart_buf,WIFI_buffer_len);
			WIFI_rx_get_rst;
			return OT;
		}
		if(Usart_buf_find_char((u8 *)("404 Not Found")))
		{
			printf("POST请求失败:404\r\n\r\n");
			WIFI_Generic_CmdSend("+++");
			clear_buffer(wifi_usart_buf,WIFI_buffer_len);
			WIFI_rx_get_rst;
			return ERR;
		}
	}
	if(Usart_buf_find_char((u8 *)FB_KeyWord))
	{
		Request_buf = wifi_usart_buf+Usart_buf_find_char((u8 *)FB_KeyWord);
		printf("POST请求成功\r\n\r\n");
		printf("%s",Request_buf);
		WIFI_Generic_CmdSend("+++");
		clear_buffer(wifi_usart_buf,WIFI_buffer_len);
		WIFI_rx_get_rst;
		return OK;
	}
	else
	{
		printf("POST请求接收数据异常\r\n\r\n");
		WIFI_Generic_CmdSend("+++");
		clear_buffer(wifi_usart_buf,WIFI_buffer_len);
		WIFI_rx_get_rst;
		return ERR;
	}


}
/****************************************************************************/


/**
  *@name 无参数AT指令传输
  */
u8 WIFI_NoParam_Cmd(const char *AT_Cmd)
{
	WIFI_Generic_CmdSend("%s\r\n",AT_Cmd);
  return WIFI_FeedbackProcess("OK","ERROR", 5000,"AT指令设置");
}
/****************************************************************************/


/**
  *@name WIFI模块指令结果返回处理
  */
u8 WIFI_FeedbackProcess(char *Fb_s_Info,char *Fb_f_Info, u32 w_time,const char *RemindInfo)
{
	while(!(WIFI_rx_get && Usart_buf_find_char((u8 *)Fb_s_Info)))
	{
		Delay_us(1000);
		if(!w_time--)
		{
			printf("%s超时\r\n\r\n",RemindInfo);
			clear_buffer(wifi_usart_buf,WIFI_buffer_len);
			WIFI_rx_get_rst;
			return OT;
		}
		if(Usart_buf_find_char((u8 *)Fb_f_Info))
		{
			printf("%s失败\r\n\r\n",RemindInfo);
			clear_buffer(wifi_usart_buf,WIFI_buffer_len);
			WIFI_rx_get_rst;
			return ERR;
		}
	}
	printf("%s成功\r\n\r\n",RemindInfo);
	clear_buffer(wifi_usart_buf,WIFI_buffer_len);
	WIFI_rx_get_rst;
	return OK;
}
/****************************************************************************/





/**
	* @name：itoa
	* @describe ：将整形数据转换成字符串,被WIFI_Generic_CmdSend()调用
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
	* @name：WIFI_Generic_CmdSend
	* @describe ：WIFI指令格式化输出，通用命令输出函数,类似printf()
	*/
void WIFI_Generic_CmdSend(char * Data, ... )
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(WIFI_USARTx, 0x0d);
					Data ++;
				break;
				case 'n':							          //换行符
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
				case 's':										  //字符串
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						USART_SendData(WIFI_USARTx,*s);
						while( USART_GetFlagStatus(WIFI_USARTx, USART_FLAG_TXE) == RESET );
					}				
					Data++;				
				break;
				case 'd':			
					//十进制
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
  *@name 串口buffer判断是否存在某字段
  *****************************************************************************/
u16 Usart_buf_find_char(u8 *target_char)
{
	u16 i,j;
	u8 len;
	len = strlen((const char *)target_char);
//	u8 ready[5]={0x72,0x65,0x61,0x64,0x79};
	for(i=0;i<=WIFI_buffer_len;i++)
	{
		if(wifi_usart_buf[i] == target_char[0])
		{
			for(j=1;j<len;j++)
			{
				if(wifi_usart_buf[i+j] !=  target_char[j])
				{
					break;
				}
			}
			if(j >= len)
			{
				return i;
			}
		}
	}
	return 0;
}
/****************************************************************************/


/****************************************************************************
  *@name WIFI占用串口USART3中断函数
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
  *@name 串口3监测中断函数
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
  *@name buffer数组清空函数
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
