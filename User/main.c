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
#include "Timer.h"

//void GPIO_Config(void);
//void delay_ms(u16 time);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
u8 AT_CWJAP[]="AT+CWJAP=\"KLX\",\"klx39909679203\"\r\n";
u8 DA_Signal[10]={0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00};
//wifi_usart_buf
u16 Usart_buf_fine_char(u8 *target_char);
int main(void)
{	
	u16 i;
  SysTick_Init();
//	Delay_ms(2000);
	debug_Init();
//	GENERAL_TIM_Init();
  WIFI_Config();
//	Delay_ms(1000);
//  printf("debug功能已开 \r\n");
//	printf("length = %d",strlen(AT_CWJAP));

  while(1)
	{
			if(WIFI_ready)
			{
//				USART_ITConfig(WIFI_USARTx, USART_IT_RXNE, DISABLE);
//				Usart_SendString(DEBUG_USARTx,wifi_usart_buf,(u16)(wifi_status & 0x0000FFFF));
				  if(Usart_buf_fine_char((u8 *)("ready")))
					{
						printf("WIFI模块ready = %d",Usart_buf_fine_char((u8 *)("ready")));
					}
					
//				Usart_SendString(DEBUG_USARTx,DA_Signal,10);	
//				Usart_SendByte(DEBUG_USARTx,(u8)(wifi_status & 0x0000FFFF));
				for(i=0;i<=(wifi_status & 0x0000FFFF);i++)
				{
					wifi_usart_buf[i] = 0;
				}
				
				wifi_status &= 0x7FFF0000;
	
			}
	}
	
}

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

