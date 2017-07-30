#ifndef __WIFI_H
#define	__WIFI_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_SysTick.h"
#include "bsp_usart.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>


/**********************串口宏定义*******************************/
#define  WIFI_USARTx                   USART3
#define  WIFI_USART_CLK                RCC_APB1Periph_USART3
#define  WIFI_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  WIFI_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  WIFI_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  WIFI_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  WIFI_USART_TX_GPIO_PORT       GPIOB   
#define  WIFI_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  WIFI_USART_RX_GPIO_PORT       GPIOB
#define  WIFI_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  WIFI_USART_IRQ                USART3_IRQn
#define  WIFI_USART_IRQHandler         USART3_IRQHandler

#define  WIFI_enable                   GPIO_SetBits(GPIOB,GPIO_Pin_8);
#define  WIFI_disable                  GPIO_ResetBits(GPIOB,GPIO_Pin_8);

#define  WIFI_Set											 GPIO_SetBits(GPIOB,GPIO_Pin_9);
#define  WIFI_Reset                    GPIO_ResetBits(GPIOB,GPIO_Pin_9);
/************************************************************************/

/*******************串口监测定时器宏定义*******************************/
#define            WIFI_USART_TIM                   TIM2
#define            WIFI_USART_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            WIFI_USART_TIM_CLK               RCC_APB1Periph_TIM2
#define            WIFI_USART_TIM_Period            (1000-1)
#define            WIFI_USART_TIM_Prescaler         71
#define            WIFI_USART_TIM_IRQ               TIM2_IRQn
#define            WIFI_USART_TIM_IRQHandler        TIM2_IRQHandler
/************************************************************************/

/*******************WIFI_status 标志位定义*******************************/
#define  WIFI_rx_get                   (u8)((wifi_status & 0x80000000) >> 31)
#define  WIFI_buffer_len               (u16)(wifi_status & 0x0000FFFF)

/*******************WIFI_status 标志位重置*******************************/
#define  WIFI_rx_get_rst                wifi_status &= 0x7FFF0000

typedef enum{
	WIFI_Restart,
	WIFI_JoinAP,
	WIFI_TcpConnet,
	WIFI_Mode,
	WIFI_Send,
	WIFI_POST
} WIFI_HandlerType;

//typedef struct{
//	u8 header[4];
//	u8 cmd[10];
//	u8 fir_param[20];
//	u8 sec_param[30];
//	
//}  AT_cmd;

void WIFI_Config(void);
void clear_buffer(u8 *buffer,u16 length);
u16 Usart_buf_fine_char(u8 *target_char);
u32 WIFI_DataHandler( WIFI_HandlerType type);

extern u8 wifi_usart_buf[1024];
extern u32 wifi_status; 



#endif


