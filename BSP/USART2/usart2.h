#ifndef __USART2_H
#define __USART2_H 

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stdio.h"	  

//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����2��ʼ������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/6/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved  
////////////////////////////////////////////////////////////////////////////////// 	
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef __IO uint16_t vu16;

#define USART1_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART1_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART2_RX_EN 			1					//0,������;1,����.

extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern UART_HandleTypeDef huart1;

extern vu16 USART1_RX_STA;   						//��������״̬

void USART1_Init(u32 bound);

void u1_printf(char* fmt,...);

#endif	   
















