#ifndef __SIM900A_H__
#define __SIM900A_H__	 

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
/************************************************************/
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK NANO STM32������
//ATK-SIM800C GSM/GPRSģ������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/7/30
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2018-2028
//All rights reserved	
/************************************************************/

void sim800c_test(void);
u8 sim800c_check_call_sms_ready(u8 *ack,u16 waittime);
void Text_SMS(void);

#endif





