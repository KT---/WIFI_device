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









/**
  *@name WIFI配置
  */
void WIFI_Config(void)
{
	
	WIFI_ESP8266_USART_Init();
	WIFI_ESP8266_GPIO_Init();
	WIFI_ESP8266_USART_TIM_Init();
	
	WIFI_ESP8266_Restart();
	WIFI_Send_ModeSet(1);
	Delay_ms(10);
	WIFI_ESP8266_JoinAP("10niu-yanfa","yingjian20141014");
	Delay_ms(50);
	WIFI_TcpUdp_Connet("TCP","192.168.0.117",80);
	Delay_ms(50);
	
	WIFI_NoParam_Cmd("AT+CIPSEND");
	WIFI_PostRequest("http://localhost/LoginTest/connection-test.php",48,"Device_Num=001&CARD_Num=1234567890&Quantity=100","hello world!!");

}
/**********************************************************/


//u32 WIFI_DataHandler( WIFI_HandlerType type)
//{
//	u32 w_time = 0;
////	AT_cmd JoinAP;
//#if 0
//	u8 AT_CWJAP[]="AT+CWJAP=\"CandyTime_7ED191\",\"ktlovejackiemilktea&coffee\"\r\n";
//	u8 AT_CIPSTART[]="AT+CIPSTART=\"TCP\",\"192.168.100.15\",80\r\n";
//#else
//	u8 AT_CWJAP[]="AT+CWJAP=\"KLX\",\"klx39909679203\"\r\n";
//	u8 AT_CIPSTART[]="AT+CIPSTART=\"TCP\",\"192.168.199.145\",80\r\n";
//#endif
//	u8 AT_CIPMODE[]="AT+CIPMODE=1\r\n";
//	u8 AT_CIPSEND[]="AT+CIPSEND\r\n";
//	u8 POST_HEARD[]="POST http://localhost/LoginTest/connection-test.php HTTP/1.0\r\n";
//	u8 POST_TYPE[]="Content-Type: application/x-www-form-urlencoded\r\n";
//	u8 POST_LENGTH[]="Content-Length:47\r\n\r\n";
//	u8 POST_CONTENT[]="Device_Num=001&CARD_Num=1234567890&Quantity=100\r\n";
//	switch(type)
//	{
//		case WIFI_Restart:
//					WIFI_disable;
//					WIFI_Reset;
//					Delay_ms(100);
//					WIFI_enable;
//					WIFI_Set;
//					while(!WIFI_rx_get)
//					{
//						Delay_us(1000);
//						if(++w_time > 5000)
//						{
//							printf("WIFI模块初始化超时\r\n\r\n");
//							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//							WIFI_rx_get_rst;
//							return 0;
//						}
//					}
//					if(Usart_buf_fine_char((u8 *)("ready")))
//					{
//						printf("WIFI模块ready\r\n\r\n");
//						clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//						WIFI_rx_get_rst;
//						return 1;
//					}
//					else
//					{
//						printf("WIFI模块初始化失败\r\n\r\n");
//						clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//						WIFI_rx_get_rst;
//						return 0;
//					}
////					break;
//			
//		case WIFI_JoinAP:
//				  Usart_SendString(WIFI_USARTx,AT_CWJAP,strlen((const char *)AT_CWJAP));
//					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("OK"))))
//					{
//						Delay_us(1000);
//						if(++w_time > 20000)
//						{
//							printf("AP连接超时\r\n\r\n");
//							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//							WIFI_rx_get_rst;
//							return 0;
//						}
//					}
//					printf("AP连接成功\r\n\r\n");
//					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//					WIFI_rx_get_rst;
//					return 1;
////					break;
//		case WIFI_TcpConnet:
//					Usart_SendString(WIFI_USARTx,AT_CIPSTART,strlen((const char *)AT_CIPSTART));
//					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("OK"))))
//					{
//						Delay_us(1000);
//						if(++w_time > 5000)
//						{
//							printf("TCP连接超时\r\n\r\n");
//							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//							WIFI_rx_get_rst;
//							return 0;
//						}
//					}
//					printf("TCP连接成功\r\n\r\n");
//					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//					WIFI_rx_get_rst;
//					return 1;
//	  case WIFI_Mode:
//					Usart_SendString(WIFI_USARTx,AT_CIPMODE,strlen((const char *)AT_CIPMODE));
//					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("OK"))))
//					{
//						Delay_us(1000);
//						if(++w_time > 2000)
//						{
//							printf("MODE设置失败\r\n\r\n");
//							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//							WIFI_rx_get_rst;
//							return 0;
//						}
//					}
//					printf("MODE设置成功\r\n\r\n");
//					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//					WIFI_rx_get_rst;
//					return 1;
//	  case WIFI_Send:
//			    Usart_SendString(WIFI_USARTx,AT_CIPSEND,strlen((const char *)AT_CIPSEND));
//					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("OK"))))
//					{
//						Delay_us(1000);
//						if(++w_time > 3000)
//						{
//							printf("启动透传失败\r\n\r\n");
//							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//							WIFI_rx_get_rst;
//							return 0;
//						}
//					}
//					printf("启动透传成功\r\n\r\n");
//					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//					WIFI_rx_get_rst;
//					return 1;

//		case WIFI_POST:
//				  Usart_SendString(WIFI_USARTx,POST_HEARD,strlen((const char *)POST_HEARD));
//				  Usart_SendString(WIFI_USARTx,POST_TYPE,strlen((const char *)POST_TYPE));
//				  Usart_SendString(WIFI_USARTx,POST_LENGTH,strlen((const char *)POST_LENGTH));
//				  Usart_SendString(WIFI_USARTx,POST_CONTENT,strlen((const char *)POST_CONTENT));
//					while(!(WIFI_rx_get && Usart_buf_fine_char((u8 *)("hello device"))))
//					{
//						Delay_us(1000);
//						if(++w_time > 10000)
//						{
//							printf("POST请求失败\r\n\r\n");
//							clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//							WIFI_rx_get_rst;
//							return 0;
//						}
//					}
//					printf("POST请求成功\r\n\r\n");
//					clear_buffer(wifi_usart_buf,WIFI_buffer_len);
//					WIFI_rx_get_rst;
//					return 1;

//		default:break;
//	}
//	return 0;
//}






