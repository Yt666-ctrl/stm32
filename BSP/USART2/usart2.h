#ifndef __USART2_H
#define __USART2_H 

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stdio.h"	  

//////////////////////////////////////////////////////////////////////////////////	   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口2初始化代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2018/6/26
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved  
////////////////////////////////////////////////////////////////////////////////// 	
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef __IO uint16_t vu16;

#define USART1_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART1_MAX_SEND_LEN		400					//最大发送缓存字节数
#define USART2_RX_EN 			1					//0,不接收;1,接收.

extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//接收缓冲,最大USART2_MAX_RECV_LEN字节
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//发送缓冲,最大USART2_MAX_SEND_LEN字节
extern UART_HandleTypeDef huart1;

extern vu16 USART1_RX_STA;   						//接收数据状态

void USART1_Init(u32 bound);

void u1_printf(char* fmt,...);

#endif	   
















