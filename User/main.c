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
//u8 AT_CWJAP[]="AT+CWJAP=\"KLX\",\"klx39909679203\"\r\n";
void USARTx_printf(USART_TypeDef * USARTx, char * Data, ... );

int main(void)
{	
  SysTick_Init();
	debug_Init();
  WIFI_Config();
//  USARTx_printf(DEBUG_USARTx,"USARTx_printf function test \r\nssid=%s\r\npassword=%s","KLX","klx39909679203");
//	Delay_ms(500);
//	USARTx_printf(WIFI_USARTx,"AT+CWJAP=\"%s\",\"%s\"\r\n","10niu-yanfa","yingjian20141014");


  while(1)
	{
		
    
	}
	
}



