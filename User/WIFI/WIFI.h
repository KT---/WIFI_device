#ifndef __WIFI_H
#define	__WIFI_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include <stdio.h>
#include <string.h>


/********************串口宏定义*******************************/
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
/**************************************************************/

#define  WIFI_ready                    wifi_status & 0x80000000

void WIFI_Init(void);
void WIFI_Config(void);

extern u8 wifi_usart_buf[255];
extern u32 wifi_status; 
#endif


