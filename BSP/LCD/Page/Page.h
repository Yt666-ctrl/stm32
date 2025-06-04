#ifndef __PAGE_H
#define __PAGE_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#define COMPASS_UPDATE_TIME 10

typedef unsigned char u8;
void PageTemperatureHumidityProcess(u8 Flag, u8 Key);  //温度湿度
void PageLightProcess(u8 Flag, u8 Key); //光照
void PageCO2ConcentrationProcess(u8 Flag, u8 Key); //二氧化碳浓度
void PageProcessing(void);	
void Pagethreshold(u8 Flag, u8 threshold);  //阈值
void PageinfraredProcess(void);  //人体红外

void Pagcontraste(void);
void Pagecontrast(void); // 对比
#endif

