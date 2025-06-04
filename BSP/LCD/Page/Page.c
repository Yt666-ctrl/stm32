#include "page.h"
#include "lcd.h"
#include "lcd_init.h"
#include "key.h"
#include "stdio.h"
#include "dht11.h"
#include <string.h>
#include "gy30.h" 
#include "sgp30.h"
#include "tim.h"
#include <stdlib.h>
#include "sim900a.h"

float concentration;

u8 temperature;//温度
u8 humidity;//湿度
float Light;//光照
uint8_t  Concentration;// CO2浓度

u8 atr[64];//温度实际值储存数组
u8 btr[64];//湿度实际值储存数组
char ctr[30];//光照实际值储存数组
char dtr[10];//CO2浓度实际值储存数组

float temperature2 = 24;// 温度阈值
float humidity2 = 60;// 湿度阈值
float illumination2 = 140;// 光照阈值
float concentration2 = 32768.0;// CO2浓度阈值

char atr2[10];//温度实际值储存数组
char btr2[10];//湿度阈值储存数组
char ctr2[10];//光照阈值储存数组
char dtr2[10];//CO2浓度阈值储存数组

uint32_t duty_cycle = 50; // 设置占空比为50%


//红色代表正在变化修改的值
//蓝色代表无法在当前页面修改的值


// 实际温湿度处理页面
void PageTemperatureHumidityProcess(u8 Flag, u8 Key) 
{
    static uint64_t s_ullLastUpdateTimeMs = 0;

    dht11_read_data(&temperature, &humidity); // 读取温湿度数据

    float temperature_float = (float)temperature;
    float humidity_float = (float)humidity;

    if (Flag)
    {
        LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); // 清屏
    }

    if (HAL_GetTick() >= s_ullLastUpdateTimeMs + COMPASS_UPDATE_TIME)
    {
			
        s_ullLastUpdateTimeMs = HAL_GetTick();
		
		LCD_ShowChinese(60, 0, "实际", BLACK, WHITE, 16, 0);
		LCD_ShowChinese(130, 0, "阈值", BLACK, WHITE, 16, 0);

		
		// 显示实际温湿度	
        sprintf((char *)atr, "%2.2f", temperature_float);
        sprintf((char *)btr, "%2.2f", humidity_float);
		
        LCD_ShowChinese(15, 20, "温度", BLACK, WHITE, 16, 0);
        LCD_ShowChinese(15, 50, "湿度", BLACK, WHITE, 16, 0);

        LCD_ShowString(60, 20, atr, RED, WHITE, 16, 0);
        LCD_ShowString(60, 50, btr, RED, WHITE, 16, 0);
		
		// 显示温湿度单位
		Draw_Circle(190,23,2,BLACK); 
		LCD_ShowChar(195,20,'C',BLACK,WHITE,16,0);
		LCD_ShowChar(190,50,'%',BLACK,WHITE,16,0);
		
		// 显示阈值温湿度
		sprintf(atr2, "%2.2f", temperature2);
        LCD_ShowString(130, 20, (const u8 *)atr2, BLUE, WHITE, 16, 0);
		sprintf(btr2, "%2.2f", humidity2);
		LCD_ShowString(130, 50, (const u8 *)btr2, BLUE, WHITE, 16, 0);
	
    }
}

// 光照处理页面
void PageLightProcess(u8 Flag, u8 Key)
{
//	uint8_t percentage;
    static uint64_t s_ullLastUpdateTimeMs = 0;


    Light = Value_GY30();// 获取光照值

    if (Flag)
    {
        LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); // 清屏
    }

    if (HAL_GetTick() >= s_ullLastUpdateTimeMs + COMPASS_UPDATE_TIME)
    {
        s_ullLastUpdateTimeMs = HAL_GetTick(); 
		
		// 显示实际光照
        sprintf(ctr, "%07.1f", Light);				
        LCD_ShowChinese(15, 80, "光照", BLACK, WHITE, 16, 0);
        LCD_ShowString(60, 80, (const u8 *)ctr, RED, WHITE, 16, 0);
		LCD_ShowString(190,80,"lux",BLACK,WHITE,16,0);
		
		// 显示阈值光照
		sprintf(ctr2, "%2.1f", illumination2);
        LCD_ShowString(130, 80, (const u8 *)ctr2, BLUE, WHITE, 16, 0);
		
		
    }
}

// 二氧化碳浓度处理页面
void PageCO2ConcentrationProcess(u8 Flag, u8 Key) 
{
	uint8_t  dity;
    static uint64_t s_ullLastUpdateTimeMs = 0;

    if (Flag)
    {
        LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); // 清屏
    }
	
	 SGP30(&Concentration, &dity); // 读取温湿度数据
	  float Concentration_float = (float)Concentration;
   // float humidity_float = (float)humidity;

   // Concentration = SGP30();// 获取二氧化碳浓度值	
	//Concentration=Concentration*0.000001;//二氧化碳浓度值转化成百分比	
	
	
    if (HAL_GetTick() >= s_ullLastUpdateTimeMs + COMPASS_UPDATE_TIME)
    {
        s_ullLastUpdateTimeMs = HAL_GetTick();

		// 显示实际二氧化碳浓度
       // sprintf(dtr, "%2.2f", Concentration);
        if (Concentration_float != 65535)
        {
            sprintf(dtr, "%07.1f", (float)Concentration_float);
            LCD_ShowString(60, 110, (const u8 *)dtr, BLUE, WHITE, 16, 0);
        }		
		LCD_ShowChinese(15, 110, "浓度", BLACK, WHITE, 16, 0);
       // LCD_ShowString(60, 110, (const u8 *)dtr, RED, WHITE, 16, 0);
		LCD_ShowString(190,110,"ppm",BLACK,WHITE,16,0);
		
		// 显示阈值二氧化碳浓度
		sprintf(dtr2, "%07.1f", concentration2);
        LCD_ShowString(130, 110, (const u8 *)dtr2, BLUE, WHITE, 16, 0);
    }
	
}

// 人体红外处理页面
void PageinfraredProcess(void) 
{
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 1)
    {

        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 60);//蜂鸣器报警，声报警
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);//亮灯，光报警
		Text_SMS();	// 发送短信通知		
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);//关闭蜂鸣器报警
       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    }
}


 // 阈值设置页面
 char c = 0;
char h=0;
void Pagethreshold(u8 Flag, u8 threshold) 
{
	dht11_read_data(&temperature, &humidity);// 读取温湿度数据
	float temperature_float = (float)temperature;
	float humidity_float = (float)humidity;
	
	
    Light = Value_GY30();// 获取光照值
	
	//Concentration = SGP30();// 获取二氧化碳浓度值
	uint8_t  dity;

	 SGP30(&Concentration, &dity); // 读取温湿度数据
	  float Concentration_float = (float)Concentration;
	//Concentration=Concentration*0.000001;

    switch (threshold)//分别在阈值更改界面显示，实际值和阈值
    {
        case 1:
            if (Flag==0&c==0) {LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); c=1;}
			
			LCD_ShowChinese(130, 35, "实际", BLACK, WHITE, 16, 0);
			LCD_ShowChinese(60,35, "阈值", BLACK, WHITE, 16, 0);
			
            sprintf(atr2, "%2.2f", temperature2);
            LCD_ShowChinese(15, 60, "温度", BLACK, WHITE, 16, 0);
            LCD_ShowString(60, 60, (const u8 *)atr2, RED, WHITE, 16, 0);
			Draw_Circle(185,63,2,BLACK); 
			LCD_ShowChar(190,60,'C',BLACK,WHITE,16,0);
			
			sprintf((char *)atr, "%2.2f", temperature_float);      
			LCD_ShowString(130, 60, atr, BLUE, WHITE, 16, 0);
			
            break;
        case 2:
           if (c==0) {LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); c=1;}
		   
		   	LCD_ShowChinese(130, 35, "实际", BLACK, WHITE, 16, 0);
			LCD_ShowChinese(60, 35, "阈值", BLACK, WHITE, 16, 0);
		   
            sprintf(btr2, "%2.2f", humidity2);
            LCD_ShowChinese(15, 60, "湿度", BLACK, WHITE, 16, 0);
            LCD_ShowString(60, 60, (const u8 *)btr2, RED, WHITE, 16, 0);
			LCD_ShowChar(190,60,'%',BLACK,WHITE,16,0);
		   
		    sprintf((char *)btr, "%2.2f", humidity_float);
		    LCD_ShowString(130, 60, btr, BLUE, WHITE, 16, 0);
		   
            break;
        case 3:
            if (c==0) {LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); c=1;}
			
			LCD_ShowChinese(130, 35, "实际", BLACK, WHITE, 16, 0);
			LCD_ShowChinese(60, 35, "阈值", BLACK, WHITE, 16, 0);
			
            sprintf(ctr2, "%2.1f", illumination2);
            LCD_ShowChinese(15, 60, "光照", BLACK, WHITE, 16, 0);
            LCD_ShowString(60, 60, (const u8 *)ctr2, RED, WHITE, 16, 0);
			LCD_ShowString(195,60,"lux",BLACK,WHITE,16,0);
			
			sprintf(ctr, "%07.1f", Light);		
			LCD_ShowString(130, 60, (const u8 *)ctr, BLUE, WHITE, 16, 0);

            break;
        case 4:
            if (c==0) {LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); c=1;}
			
			LCD_ShowChinese(130, 35, "实际", BLACK, WHITE, 16, 0);
			LCD_ShowChinese(60, 35, "阈值", BLACK, WHITE, 16, 0);
			
            sprintf(dtr2, "%07.1f", concentration2);
            LCD_ShowChinese(15, 60, "浓度", BLACK, WHITE, 16, 0);
            LCD_ShowString(60, 60, (const u8 *)dtr2, RED, WHITE, 16, 0);
			LCD_ShowString(195,60,"ppm",BLACK,WHITE,16,0);

			if (Concentration_float != 65535)
			{
				sprintf(dtr, "%07.1f", (float)Concentration_float);
				LCD_ShowString(130, 60, (const u8 *)dtr, BLUE, WHITE, 16, 0);
			}	
          
			 //sprintf(dtr, "%2.2f", Concentration);   	
			//LCD_ShowString(130, 60, (const u8 *)dtr, BLUE, WHITE, 16, 0);

            break;
        default:
            break;
    }
	
}

int a = 0;
int biao;
void PageProcessing(void)//按键启动界面切换，设置
{

	//int key_value=KeyScan();

    static char frequency = 0;


	Pagecontrast(); // 对比
    if (a == 0)
    {
        if (key_value == 1)
        {
			
			LCD_BLK_Set();//打开背光
			LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
			c = 0;
			key_value = 0;//
			a = 2;	
			
        }
	 }
    else if (a == 2)
    {
		
        PageTemperatureHumidityProcess(0, 0); // 温湿度
        PageLightProcess(0, 0); // 光照
        PageCO2ConcentrationProcess(0, 0); // 二氧化碳浓度
        PageinfraredProcess(); // 红外 
		
    }
    else if (a == 3)
    {
			if (frequency==1)// 阈值设置函数
			{

					Pagethreshold(0, 1);
					if (key_value == 3)
					{
						temperature2 += 1.0;
						key_value = 0;
					}
					else if (key_value == 4)
					{
						temperature2 -= 1.0;
						key_value = 0;
					}

			}
			if (frequency==2)
			{
					 Pagethreshold(0, 2);
					if (key_value == 3)
					{
						humidity2 += 1.0;
						key_value = 0;
					}
					else if (key_value == 4)
					{
						humidity2 -= 1.0;
						key_value = 0;
					}

			}
			if (frequency==3)
			{
					 Pagethreshold(0, 3);
					if (key_value == 3)
					{
						illumination2 += 1.0;
						key_value = 0;
					}
					else if (key_value == 4)
					{
						illumination2 -= 1.0;
						key_value = 0;
					}

			}		
			if (frequency==4)
			{
					 Pagethreshold(0, 4);
					if (key_value == 3)
					{
						concentration2 += 1;
						key_value = 0;
					}
					else if (key_value == 4)
					{
						concentration2 -= 1;
						key_value = 0;
					}

			}						
			
		}	
 
	   if (key_value == 2)//切换控制，切换阈值按键，判定
		{
			c = 0;
			a=3;
							
			frequency+=1;//
			key_value = 0;
			if (frequency >= 5)
			{
				frequency = 0;
					a=2;
				 LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); // 清屏
			}
		}

}

void Pagecontrast(void) // 对比函数，对比阈值和实际值，高于或低于三个单位，产生不同的16种报警声音
{
	
	if  (a==2)
	{
		// 初始化变量
		char temperatureFlag, humidityFlag, illuminationFlag, concentrationFlag;
		temperatureFlag = humidityFlag = illuminationFlag = concentrationFlag = 0;

		// 温度比较
		if ((temperature2 - temperature) > 3 || (temperature - temperature2) > 3) {
			temperatureFlag = 1;
		} 
		else
		{
			temperatureFlag = 0;
		}

		// 湿度比较
		if ((humidity - humidity2) > 3 || (humidity2 - humidity) > 3) 
		{
			humidityFlag = 2;
		} 
		else 
		{
			humidityFlag = 0;
		}

		// 光照比较
		if ((Light - illumination2) > 3 || (illumination2 - Light) > 3) {
			illuminationFlag = 4;
		} else {
			illuminationFlag = 0;
		}

		// CO2浓度比较
		if ((Concentration - concentration2) > 3 || (concentration2 - Concentration) > 3) {
			concentrationFlag = 9;
		} else {
			concentrationFlag = 0;
		}

		char total = 0;
		total = temperatureFlag + humidityFlag + illuminationFlag + concentrationFlag;	
		
//		温度:do
//		湿度:re
//		温度+湿度:do,re
//		光照:mi
//		温度+光照:do,mi
//		湿度+光照:re,mi
//		温度+湿度+光照:re,mi,fa
//		浓度:fa
//		温度+浓度:si
//		湿度+浓度:sol,fa
//		温度+湿度+浓度:do,re,mi
//		光照+浓度:mi.fa
//		温度+光照+浓度:fa,sol,la
//		湿度+光照+浓度:si,mi,la
//		温度+湿度+光照+浓度:do.re,,fa,sol,si		
		
		// 根据总和设置输出
		switch (total) 
		{
			case 1: // 温度
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_DO, duty_cycle); 	// 播放 `do` 音符50% 占空比
				break;
			case 2: // 湿度	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_RE,  duty_cycle); 	// 播放 `re` 音符50% 占空比					
				break;
			case 3: // 温度+湿度
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_DO, duty_cycle); 	// 播放 `do` 音符50% 占空比
					Play_Note(NOTE_RE,  duty_cycle); 	// 播放 `re` 音符50% 占空比
				break;
			case 4: // 光照	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_MI,  duty_cycle);	// 播放 `mi` 音符50% 占空比
				break;
			case 5: // 温度+光照
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_DO, duty_cycle); 	// 播放 `do` 音符50% 占空比
					Play_Note(NOTE_MI,  duty_cycle);	// 播放 `mi` 音符50% 占空比
				break;
			case 6: // 湿度+光照
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_RE,  duty_cycle); 	// 播放 `re` 音符50% 占空比		
					Play_Note(NOTE_MI,  duty_cycle);	// 播放 `mi` 音符50% 占空比
				break;
			case 7: // 温度+湿度+光照	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);	
					
					Play_Note(NOTE_RE,  duty_cycle); 	// 播放 `re` 音符50% 占空比
					Play_Note(NOTE_MI,  duty_cycle);	// 播放 `mi` 音符50% 占空比
					Play_Note(NOTE_FA, duty_cycle); 	// 播放 `fa` 音符50% 占空比
				break;
			case 9: // 浓度		
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);	
					
					Play_Note(NOTE_FA, duty_cycle); 	// 播放 `fa` 音符50% 占空比			
				break;
			case 10: // 温度+浓度
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_SI, duty_cycle);		// 播放 `si` 音符50% 占空比
				break;
			case 11: // 湿度+浓度
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_SOL, duty_cycle); // 播放 `sol` 音符50% 占空比
					Play_Note(NOTE_FA, duty_cycle); // 播放 `fa` 音符50% 占空比
				break;
			case 12: // 温度+湿度+浓度
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_DO,  duty_cycle); // 播放 `do` 音符 500ms，50% 占空比
					Play_Note(NOTE_RE,  duty_cycle); // 播放 `re` 音符 500ms，50% 占空比
					Play_Note(NOTE_MI, duty_cycle); // 播放 `mi` 音符 500ms，50% 占空比
				break;
			case 13: // 光照+浓度
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_MI,  duty_cycle);	// 播放 `mi` 音符50% 占空比
					Play_Note(NOTE_FA, duty_cycle); 	// 播放 `fa` 音符50% 占空比
				break;
			case 14: // 温度+光照+浓度
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_FA, duty_cycle); 	// 播放 `fa` 音符50% 占空比
					Play_Note(NOTE_SOL,  duty_cycle);	// 播放 `sol` 音符50% 占空比
					Play_Note(NOTE_LA, duty_cycle); 	// 播放 `la` 音符50% 占空比
				break;
			case 15: // 湿度+光照+浓度
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_SI, duty_cycle); // 播放 `si` 音符 500ms，50% 占空比
					Play_Note(NOTE_LA, duty_cycle); 	// 播放 `la` 音符50% 占空比
					Play_Note(NOTE_MI, duty_cycle); // 播放 `mi` 音符 500ms，50% 占空比
				break;
			case 16: // 温度+湿度+光照+浓度
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
			
					Play_Note(NOTE_DO, duty_cycle); 	// 播放 `do` 音符50% 占空比
					Play_Note(NOTE_RE,  duty_cycle); 	// 播放 `re` 音符50% 占空比
					Play_Note(NOTE_MI,  duty_cycle);	// 播放 `mi` 音符50% 占空比
					Play_Note(NOTE_FA, duty_cycle); 	// 播放 `fa` 音符50% 占空比
					Play_Note(NOTE_SOL,  duty_cycle);	// 播放 `sol` 音符50% 占空比
					Play_Note(NOTE_LA, duty_cycle); 	// 播放 `la` 音符50% 占空比
					Play_Note(NOTE_SI, duty_cycle);		// 播放 `si` 音符50% 占空比
				break;
			default:
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
					
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
				break;
		}
	}
}

