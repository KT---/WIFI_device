#ifndef __ESP8266_H
#define	__ESP8266_H


#include "stm32f10x.h"

#include "stm32f10x_it.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "debug.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>


/**********************串口宏定义*******************************/
#define         WIFI_USARTx           	       	USART3
#define         WIFI_USART_CLK          	     	RCC_APB1Periph_USART3
#define         WIFI_USART_APBxClkCmd     	   	RCC_APB1PeriphClockCmd
#define         WIFI_USART_BAUDRATE          		115200

// USART GPIO 引脚宏定义
#define         WIFI_USART_GPIO_CLK          		(RCC_APB2Periph_GPIOB)
#define         WIFI_USART_GPIO_APBxClkCmd   		RCC_APB2PeriphClockCmd
    
#define         WIFI_USART_TX_GPIO_PORT      		GPIOB   
#define         WIFI_USART_TX_GPIO_PIN       		GPIO_Pin_10
#define         WIFI_USART_RX_GPIO_PORT      		GPIOB
#define         WIFI_USART_RX_GPIO_PIN       		GPIO_Pin_11

#define         WIFI_USART_IRQ               		USART3_IRQn
#define         WIFI_USART_IRQHandler        		USART3_IRQHandler

#define         WIFI_enable                  		GPIO_SetBits(GPIOB,GPIO_Pin_8);
#define         WIFI_disable                 		GPIO_ResetBits(GPIOB,GPIO_Pin_8);

#define         WIFI_Set												GPIO_SetBits(GPIOB,GPIO_Pin_9);
#define					WIFI_Reset                  	 	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
/************************************************************************/

/*******************串口监测定时器宏定义*******************************/
#define					WIFI_USART_TIM                 	TIM2
#define         WIFI_USART_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
#define         WIFI_USART_TIM_CLK              RCC_APB1Periph_TIM2
#define         WIFI_USART_TIM_Period           (1000-1)
#define         WIFI_USART_TIM_Prescaler        71
#define         WIFI_USART_TIM_IRQ              TIM2_IRQn
#define         WIFI_USART_TIM_IRQHandler       TIM2_IRQHandler
/************************************************************************/

/*******************WIFI_status 标志位定义*******************************/
#define					WIFI_rx_get                   	(u8)((wifi_status & 0x80000000) >> 31)
#define					WIFI_ready											(u8)((wifi_status & 0x40000000) >> 30)
#define					WIFI_AP													(u8)((wifi_status & 0x20000000) >> 29)
#define					WIFI_TCP												(u8)((wifi_status & 0x10000000) >> 28)
#define					WIFI_buffer_len               	(u16)(wifi_status & 0x0000FFFF)

/*******************WIFI_status 标志位置0*******************************/
#define 				WIFI_rx_get_rst                	wifi_status &= 0x7FFF0000



#define					ERR															0
#define					OK															1
#define					OT															2



void WIFI_ESP8266_USART_Init(void);
void WIFI_ESP8266_USART_TIM_Init(void);
void WIFI_ESP8266_GPIO_Init(void);
void WIFI_Generic_CmdSend(char * Data, ... );
u8 WIFI_ESP8266_Restart(void);
u8 WIFI_ESP8266_JoinAP(const char *ssid,const char *password);
u8 WIFI_TcpUdp_Connet(const char *ConnetType,const char *IPaddr,const u32 PortNum);
u8 WIFI_Send_ModeSet(const char MODE);
u8 WIFI_NoParam_Cmd(const char *AT_Cmd);
u8 WIFI_PostRequest(const char *URL,const u16 value_length,const char *Value,const char *FB_KeyWord);

u8 WIFI_FeedbackProcess(char *Fb_s_Info,char *Fb_f_Info, u32 w_time,const char *RemindInfo);

void clear_buffer(u8 *buffer,u16 length);
u16 Usart_buf_find_char(u8 *target_char);

extern u8 wifi_usart_buf[1024];
extern u8 *Request_buf;
extern u32 wifi_status; 


#endif 
