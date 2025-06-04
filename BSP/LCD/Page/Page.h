#ifndef __PAGE_H
#define __PAGE_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define COMPASS_UPDATE_TIME 10

typedef unsigned char u8;
void PageTemperatureHumidityProcess(u8 Flag, u8 Key);  //�¶�ʪ��
void PageLightProcess(u8 Flag, u8 Key); //����
void PageCO2ConcentrationProcess(u8 Flag, u8 Key); //������̼Ũ��
void PageProcessing(void);	
void Pagethreshold(u8 Flag, u8 threshold);  //��ֵ
void PageinfraredProcess(void);  //�������

void Pagcontraste(void);
void Pagecontrast(void); // �Ա�
#endif

