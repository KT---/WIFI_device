/**
  ******************************************************************************
  * @file    main.c
  * @author  KT
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "WIFI.h"
#include "bsp_SysTick.h"
#include "debug.h"
#include <string.h>

//void GPIO_Config(void);
//void delay_ms(u16 time);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
u8 AT_CWJAP[]="AT+CWJAP=\"KLX\",\"klx39909679203\"\r\n";
u8 DA_Signal[]="\r\n0D0A0D0A\r\n";
//wifi_usart_buf
int main(void)
{	
	u16 i;
  SysTick_Init();
	debug_Init();
  WIFI_Config();
//  printf("debug功能已开 \r\n");
//	printf("length = %d",strlen(AT_CWJAP));

  while(1)
	{
			if(WIFI_ready)
			{
				
				Usart_SendString(DEBUG_USARTx,wifi_usart_buf);
//				Usart_SendString(DEBUG_USARTx,DA_Signal);	
				Usart_SendByte(DEBUG_USARTx,(u8)(wifi_status & 0x0000FFFF));
				for(i=0;i<(wifi_status & 0x0000FFFF);i++)
				{
					wifi_usart_buf[i] = 0;
				}
				
				wifi_status &= 0x7FFF0000;
//				USART_ITConfig(WIFI_USARTx, USART_IT_RXNE, ENABLE);	
			}
	}
	
}



