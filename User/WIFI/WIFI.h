#ifndef __WIFI_H
#define	__WIFI_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_SysTick.h"
#include "ESP8266.h"
#include "debug.h"
#include <stdio.h>
#include <string.h>







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



#endif


