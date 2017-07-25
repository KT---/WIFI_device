/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "WIFI.h"

//void GPIO_Config(void);
//void delay_ms(u16 time);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
char AT_CWJAP[]="AT+CWJAP=\"KLX\",\"klx39909679203\"\r\n";
int main(void)
{	
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
//	GPIO_Config();
//  USART_Config();
	USART1_Config();
  WIFI_Config();
	/* 发送一个字符串 */ 
//	Usart_SendString( USART1,"这是一个串口中断接收回显实验\n");
//	delay_ms(2000);
//  Usart_SendString( USART3,AT_CWJAP);
  while(1);
}



//void delay_ms(u16 time)
//{    
//   u16 i=0;  
//   while(time--)
//   {
//      i=12000;  
//      while(i--) ;    
//   }
//}
/*********************************************END OF FILE**********************/
