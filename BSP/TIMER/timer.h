#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/25
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

void TIM3_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);
#endif

