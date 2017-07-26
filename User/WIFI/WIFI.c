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
  *@name WIFI模块串口初始化
  */
void WIFI_USART_Init(void)
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
	
	
	//	/**************配置IO**************************/
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
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/**************************************************/

}


/**
  *@name WIFI模块使能及重置引脚初始化
  */
//void WIFI_GPIO_Init()
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	/**************配置IO**************************/
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
//	/*选择要控制的GPIO引脚*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	

//	/*设置引脚模式为通用推挽输出*/
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

//	/*设置引脚速率为50MHz */   
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

//	/*调用库函数，初始化GPIO*/
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
//	
//	/*选择要控制的GPIO引脚*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;

//	/*调用库函数，初始化GPIO*/
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	/**************************************************/
//}


/**
  *@name USART3串口接收监测定时器初始化
  */
void WIFI_USART_TIM_Init(void)
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
	TIM_Cmd(WIFI_USART_TIM, ENABLE);
}


/**
  *@name WIFI配置
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
  *@name WIFI占用串口USART3中断函数
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
  *@name 串口3监测中断函数
  */
void  WIFI_USART_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( WIFI_USART_TIM, TIM_IT_Update) != RESET ) 
	{	
    wifi_status |= 0x80000000;

		TIM_ClearITPendingBit(WIFI_USART_TIM , TIM_FLAG_Update);  		 
	}		 	
}
