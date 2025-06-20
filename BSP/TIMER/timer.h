#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/5/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

void TIM3_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void);
#endif

