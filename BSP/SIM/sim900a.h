#ifndef __SIM900A_H__
#define __SIM900A_H__	 

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
/************************************************************/
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK NANO STM32开发板
//ATK-SIM800C GSM/GPRS模块驱动	  
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2018/7/30
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2018-2028
//All rights reserved	
/************************************************************/

void sim800c_test(void);
u8 sim800c_check_call_sms_ready(u8 *ack,u16 waittime);
void Text_SMS(void);

#endif





