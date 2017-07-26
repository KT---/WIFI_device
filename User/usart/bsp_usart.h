#ifndef __USART_H
#define	__USART_H


#include "stm32f10x.h"
#include "debug.h"
#include <stdio.h>



void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, u8 *str,u16 len);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART_H */
