#ifndef __LCD_INIT_H
#define __LCD_INIT_H

//#include "sys.h"
#include "stdint.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 135
#define LCD_H 240

#else
#define LCD_W 240
#define LCD_H 135
#endif



//-----------------LCD端口定义---------------- 


#define LCD_SCLK_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)//SCL=SCLK=PB14
#define LCD_SCLK_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)


#define LCD_MOSI_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)//SDA=MOSI=PB12
#define LCD_MOSI_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)


#define LCD_RES_Clr()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)//RES=PB8
#define LCD_RES_Set()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)


#define LCD_DC_Clr()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)//DC=PB6
#define LCD_DC_Set()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
 		     

#define LCD_CS_Clr()   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET)//CS=PD2
#define LCD_CS_Set()   HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)


#define LCD_BLK_Clr()  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET)//BLK=PC11
#define LCD_BLK_Set()  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET)




void LCD_GPIO_Init(void);//初始化GPIO
void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif




