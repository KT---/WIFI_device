/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �����жϽ��ղ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "WIFI.h"

//void GPIO_Config(void);
//void delay_ms(u16 time);
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
char AT_CWJAP[]="AT+CWJAP=\"KLX\",\"klx39909679203\"\r\n";
int main(void)
{	
  /*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
//	GPIO_Config();
//  USART_Config();
	USART1_Config();
  WIFI_Config();
	/* ����һ���ַ��� */ 
//	Usart_SendString( USART1,"����һ�������жϽ��ջ���ʵ��\n");
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
