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

u8 temperature;//�¶�
u8 humidity;//ʪ��
float Light;//����
uint8_t  Concentration;// CO2Ũ��

u8 atr[64];//�¶�ʵ��ֵ��������
u8 btr[64];//ʪ��ʵ��ֵ��������
char ctr[30];//����ʵ��ֵ��������
char dtr[10];//CO2Ũ��ʵ��ֵ��������

float temperature2 = 24;// �¶���ֵ
float humidity2 = 60;// ʪ����ֵ
float illumination2 = 140;// ������ֵ
float concentration2 = 32768.0;// CO2Ũ����ֵ

char atr2[10];//�¶�ʵ��ֵ��������
char btr2[10];//ʪ����ֵ��������
char ctr2[10];//������ֵ��������
char dtr2[10];//CO2Ũ����ֵ��������

uint32_t duty_cycle = 50; // ����ռ�ձ�Ϊ50%


//��ɫ�������ڱ仯�޸ĵ�ֵ
//��ɫ�����޷��ڵ�ǰҳ���޸ĵ�ֵ


// ʵ����ʪ�ȴ���ҳ��
void PageTemperatureHumidityProcess(u8 Flag, u8 Key) 
{
    static uint64_t s_ullLastUpdateTimeMs = 0;

    dht11_read_data(&temperature, &humidity); // ��ȡ��ʪ������

    float temperature_float = (float)temperature;
    float humidity_float = (float)humidity;

    if (Flag)
    {
        LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); // ����
    }

    if (HAL_GetTick() >= s_ullLastUpdateTimeMs + COMPASS_UPDATE_TIME)
    {
			
        s_ullLastUpdateTimeMs = HAL_GetTick();
		
		LCD_ShowChinese(60, 0, "ʵ��", BLACK, WHITE, 16, 0);
		LCD_ShowChinese(130, 0, "��ֵ", BLACK, WHITE, 16, 0);

		
		// ��ʾʵ����ʪ��	
        sprintf((char *)atr, "%2.2f", temperature_float);
        sprintf((char *)btr, "%2.2f", humidity_float);
		
        LCD_ShowChinese(15, 20, "�¶�", BLACK, WHITE, 16, 0);
        LCD_ShowChinese(15, 50, "ʪ��", BLACK, WHITE, 16, 0);

        LCD_ShowString(60, 20, atr, RED, WHITE, 16, 0);
        LCD_ShowString(60, 50, btr, RED, WHITE, 16, 0);
		
		// ��ʾ��ʪ�ȵ�λ
		Draw_Circle(190,23,2,BLACK); 
		LCD_ShowChar(195,20,'C',BLACK,WHITE,16,0);
		LCD_ShowChar(190,50,'%',BLACK,WHITE,16,0);
		
		// ��ʾ��ֵ��ʪ��
		sprintf(atr2, "%2.2f", temperature2);
        LCD_ShowString(130, 20, (const u8 *)atr2, BLUE, WHITE, 16, 0);
		sprintf(btr2, "%2.2f", humidity2);
		LCD_ShowString(130, 50, (const u8 *)btr2, BLUE, WHITE, 16, 0);
	
    }
}

// ���մ���ҳ��
void PageLightProcess(u8 Flag, u8 Key)
{
//	uint8_t percentage;
    static uint64_t s_ullLastUpdateTimeMs = 0;


    Light = Value_GY30();// ��ȡ����ֵ

    if (Flag)
    {
        LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); // ����
    }

    if (HAL_GetTick() >= s_ullLastUpdateTimeMs + COMPASS_UPDATE_TIME)
    {
        s_ullLastUpdateTimeMs = HAL_GetTick(); 
		
		// ��ʾʵ�ʹ���
        sprintf(ctr, "%07.1f", Light);				
        LCD_ShowChinese(15, 80, "����", BLACK, WHITE, 16, 0);
        LCD_ShowString(60, 80, (const u8 *)ctr, RED, WHITE, 16, 0);
		LCD_ShowString(190,80,"lux",BLACK,WHITE,16,0);
		
		// ��ʾ��ֵ����
		sprintf(ctr2, "%2.1f", illumination2);
        LCD_ShowString(130, 80, (const u8 *)ctr2, BLUE, WHITE, 16, 0);
		
		
    }
}

// ������̼Ũ�ȴ���ҳ��
void PageCO2ConcentrationProcess(u8 Flag, u8 Key) 
{
	uint8_t  dity;
    static uint64_t s_ullLastUpdateTimeMs = 0;

    if (Flag)
    {
        LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); // ����
    }
	
	 SGP30(&Concentration, &dity); // ��ȡ��ʪ������
	  float Concentration_float = (float)Concentration;
   // float humidity_float = (float)humidity;

   // Concentration = SGP30();// ��ȡ������̼Ũ��ֵ	
	//Concentration=Concentration*0.000001;//������̼Ũ��ֵת���ɰٷֱ�	
	
	
    if (HAL_GetTick() >= s_ullLastUpdateTimeMs + COMPASS_UPDATE_TIME)
    {
        s_ullLastUpdateTimeMs = HAL_GetTick();

		// ��ʾʵ�ʶ�����̼Ũ��
       // sprintf(dtr, "%2.2f", Concentration);
        if (Concentration_float != 65535)
        {
            sprintf(dtr, "%07.1f", (float)Concentration_float);
            LCD_ShowString(60, 110, (const u8 *)dtr, BLUE, WHITE, 16, 0);
        }		
		LCD_ShowChinese(15, 110, "Ũ��", BLACK, WHITE, 16, 0);
       // LCD_ShowString(60, 110, (const u8 *)dtr, RED, WHITE, 16, 0);
		LCD_ShowString(190,110,"ppm",BLACK,WHITE,16,0);
		
		// ��ʾ��ֵ������̼Ũ��
		sprintf(dtr2, "%07.1f", concentration2);
        LCD_ShowString(130, 110, (const u8 *)dtr2, BLUE, WHITE, 16, 0);
    }
	
}

// ������⴦��ҳ��
void PageinfraredProcess(void) 
{
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 1)
    {

        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 60);//������������������
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);//���ƣ��ⱨ��
		Text_SMS();	// ���Ͷ���֪ͨ		
    }
    else
    {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);//�رշ���������
       HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    }
}


 // ��ֵ����ҳ��
 char c = 0;
char h=0;
void Pagethreshold(u8 Flag, u8 threshold) 
{
	dht11_read_data(&temperature, &humidity);// ��ȡ��ʪ������
	float temperature_float = (float)temperature;
	float humidity_float = (float)humidity;
	
	
    Light = Value_GY30();// ��ȡ����ֵ
	
	//Concentration = SGP30();// ��ȡ������̼Ũ��ֵ
	uint8_t  dity;

	 SGP30(&Concentration, &dity); // ��ȡ��ʪ������
	  float Concentration_float = (float)Concentration;
	//Concentration=Concentration*0.000001;

    switch (threshold)//�ֱ�����ֵ���Ľ�����ʾ��ʵ��ֵ����ֵ
    {
        case 1:
            if (Flag==0&c==0) {LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); c=1;}
			
			LCD_ShowChinese(130, 35, "ʵ��", BLACK, WHITE, 16, 0);
			LCD_ShowChinese(60,35, "��ֵ", BLACK, WHITE, 16, 0);
			
            sprintf(atr2, "%2.2f", temperature2);
            LCD_ShowChinese(15, 60, "�¶�", BLACK, WHITE, 16, 0);
            LCD_ShowString(60, 60, (const u8 *)atr2, RED, WHITE, 16, 0);
			Draw_Circle(185,63,2,BLACK); 
			LCD_ShowChar(190,60,'C',BLACK,WHITE,16,0);
			
			sprintf((char *)atr, "%2.2f", temperature_float);      
			LCD_ShowString(130, 60, atr, BLUE, WHITE, 16, 0);
			
            break;
        case 2:
           if (c==0) {LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); c=1;}
		   
		   	LCD_ShowChinese(130, 35, "ʵ��", BLACK, WHITE, 16, 0);
			LCD_ShowChinese(60, 35, "��ֵ", BLACK, WHITE, 16, 0);
		   
            sprintf(btr2, "%2.2f", humidity2);
            LCD_ShowChinese(15, 60, "ʪ��", BLACK, WHITE, 16, 0);
            LCD_ShowString(60, 60, (const u8 *)btr2, RED, WHITE, 16, 0);
			LCD_ShowChar(190,60,'%',BLACK,WHITE,16,0);
		   
		    sprintf((char *)btr, "%2.2f", humidity_float);
		    LCD_ShowString(130, 60, btr, BLUE, WHITE, 16, 0);
		   
            break;
        case 3:
            if (c==0) {LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); c=1;}
			
			LCD_ShowChinese(130, 35, "ʵ��", BLACK, WHITE, 16, 0);
			LCD_ShowChinese(60, 35, "��ֵ", BLACK, WHITE, 16, 0);
			
            sprintf(ctr2, "%2.1f", illumination2);
            LCD_ShowChinese(15, 60, "����", BLACK, WHITE, 16, 0);
            LCD_ShowString(60, 60, (const u8 *)ctr2, RED, WHITE, 16, 0);
			LCD_ShowString(195,60,"lux",BLACK,WHITE,16,0);
			
			sprintf(ctr, "%07.1f", Light);		
			LCD_ShowString(130, 60, (const u8 *)ctr, BLUE, WHITE, 16, 0);

            break;
        case 4:
            if (c==0) {LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); c=1;}
			
			LCD_ShowChinese(130, 35, "ʵ��", BLACK, WHITE, 16, 0);
			LCD_ShowChinese(60, 35, "��ֵ", BLACK, WHITE, 16, 0);
			
            sprintf(dtr2, "%07.1f", concentration2);
            LCD_ShowChinese(15, 60, "Ũ��", BLACK, WHITE, 16, 0);
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
void PageProcessing(void)//�������������л�������
{

	//int key_value=KeyScan();

    static char frequency = 0;


	Pagecontrast(); // �Ա�
    if (a == 0)
    {
        if (key_value == 1)
        {
			
			LCD_BLK_Set();//�򿪱���
			LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
			c = 0;
			key_value = 0;//
			a = 2;	
			
        }
	 }
    else if (a == 2)
    {
		
        PageTemperatureHumidityProcess(0, 0); // ��ʪ��
        PageLightProcess(0, 0); // ����
        PageCO2ConcentrationProcess(0, 0); // ������̼Ũ��
        PageinfraredProcess(); // ���� 
		
    }
    else if (a == 3)
    {
			if (frequency==1)// ��ֵ���ú���
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
 
	   if (key_value == 2)//�л����ƣ��л���ֵ�������ж�
		{
			c = 0;
			a=3;
							
			frequency+=1;//
			key_value = 0;
			if (frequency >= 5)
			{
				frequency = 0;
					a=2;
				 LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); // ����
			}
		}

}

void Pagecontrast(void) // �ԱȺ������Ա���ֵ��ʵ��ֵ�����ڻ����������λ��������ͬ��16�ֱ�������
{
	
	if  (a==2)
	{
		// ��ʼ������
		char temperatureFlag, humidityFlag, illuminationFlag, concentrationFlag;
		temperatureFlag = humidityFlag = illuminationFlag = concentrationFlag = 0;

		// �¶ȱȽ�
		if ((temperature2 - temperature) > 3 || (temperature - temperature2) > 3) {
			temperatureFlag = 1;
		} 
		else
		{
			temperatureFlag = 0;
		}

		// ʪ�ȱȽ�
		if ((humidity - humidity2) > 3 || (humidity2 - humidity) > 3) 
		{
			humidityFlag = 2;
		} 
		else 
		{
			humidityFlag = 0;
		}

		// ���ձȽ�
		if ((Light - illumination2) > 3 || (illumination2 - Light) > 3) {
			illuminationFlag = 4;
		} else {
			illuminationFlag = 0;
		}

		// CO2Ũ�ȱȽ�
		if ((Concentration - concentration2) > 3 || (concentration2 - Concentration) > 3) {
			concentrationFlag = 9;
		} else {
			concentrationFlag = 0;
		}

		char total = 0;
		total = temperatureFlag + humidityFlag + illuminationFlag + concentrationFlag;	
		
//		�¶�:do
//		ʪ��:re
//		�¶�+ʪ��:do,re
//		����:mi
//		�¶�+����:do,mi
//		ʪ��+����:re,mi
//		�¶�+ʪ��+����:re,mi,fa
//		Ũ��:fa
//		�¶�+Ũ��:si
//		ʪ��+Ũ��:sol,fa
//		�¶�+ʪ��+Ũ��:do,re,mi
//		����+Ũ��:mi.fa
//		�¶�+����+Ũ��:fa,sol,la
//		ʪ��+����+Ũ��:si,mi,la
//		�¶�+ʪ��+����+Ũ��:do.re,,fa,sol,si		
		
		// �����ܺ��������
		switch (total) 
		{
			case 1: // �¶�
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_DO, duty_cycle); 	// ���� `do` ����50% ռ�ձ�
				break;
			case 2: // ʪ��	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_RE,  duty_cycle); 	// ���� `re` ����50% ռ�ձ�					
				break;
			case 3: // �¶�+ʪ��
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_DO, duty_cycle); 	// ���� `do` ����50% ռ�ձ�
					Play_Note(NOTE_RE,  duty_cycle); 	// ���� `re` ����50% ռ�ձ�
				break;
			case 4: // ����	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_MI,  duty_cycle);	// ���� `mi` ����50% ռ�ձ�
				break;
			case 5: // �¶�+����
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_DO, duty_cycle); 	// ���� `do` ����50% ռ�ձ�
					Play_Note(NOTE_MI,  duty_cycle);	// ���� `mi` ����50% ռ�ձ�
				break;
			case 6: // ʪ��+����
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_RE,  duty_cycle); 	// ���� `re` ����50% ռ�ձ�		
					Play_Note(NOTE_MI,  duty_cycle);	// ���� `mi` ����50% ռ�ձ�
				break;
			case 7: // �¶�+ʪ��+����	
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);	
					
					Play_Note(NOTE_RE,  duty_cycle); 	// ���� `re` ����50% ռ�ձ�
					Play_Note(NOTE_MI,  duty_cycle);	// ���� `mi` ����50% ռ�ձ�
					Play_Note(NOTE_FA, duty_cycle); 	// ���� `fa` ����50% ռ�ձ�
				break;
			case 9: // Ũ��		
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);	
					
					Play_Note(NOTE_FA, duty_cycle); 	// ���� `fa` ����50% ռ�ձ�			
				break;
			case 10: // �¶�+Ũ��
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_SI, duty_cycle);		// ���� `si` ����50% ռ�ձ�
				break;
			case 11: // ʪ��+Ũ��
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_SOL, duty_cycle); // ���� `sol` ����50% ռ�ձ�
					Play_Note(NOTE_FA, duty_cycle); // ���� `fa` ����50% ռ�ձ�
				break;
			case 12: // �¶�+ʪ��+Ũ��
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_DO,  duty_cycle); // ���� `do` ���� 500ms��50% ռ�ձ�
					Play_Note(NOTE_RE,  duty_cycle); // ���� `re` ���� 500ms��50% ռ�ձ�
					Play_Note(NOTE_MI, duty_cycle); // ���� `mi` ���� 500ms��50% ռ�ձ�
				break;
			case 13: // ����+Ũ��
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_MI,  duty_cycle);	// ���� `mi` ����50% ռ�ձ�
					Play_Note(NOTE_FA, duty_cycle); 	// ���� `fa` ����50% ռ�ձ�
				break;
			case 14: // �¶�+����+Ũ��
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_FA, duty_cycle); 	// ���� `fa` ����50% ռ�ձ�
					Play_Note(NOTE_SOL,  duty_cycle);	// ���� `sol` ����50% ռ�ձ�
					Play_Note(NOTE_LA, duty_cycle); 	// ���� `la` ����50% ռ�ձ�
				break;
			case 15: // ʪ��+����+Ũ��
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
					
					Play_Note(NOTE_SI, duty_cycle); // ���� `si` ���� 500ms��50% ռ�ձ�
					Play_Note(NOTE_LA, duty_cycle); 	// ���� `la` ����50% ռ�ձ�
					Play_Note(NOTE_MI, duty_cycle); // ���� `mi` ���� 500ms��50% ռ�ձ�
				break;
			case 16: // �¶�+ʪ��+����+Ũ��
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
			
					Play_Note(NOTE_DO, duty_cycle); 	// ���� `do` ����50% ռ�ձ�
					Play_Note(NOTE_RE,  duty_cycle); 	// ���� `re` ����50% ռ�ձ�
					Play_Note(NOTE_MI,  duty_cycle);	// ���� `mi` ����50% ռ�ձ�
					Play_Note(NOTE_FA, duty_cycle); 	// ���� `fa` ����50% ռ�ձ�
					Play_Note(NOTE_SOL,  duty_cycle);	// ���� `sol` ����50% ռ�ձ�
					Play_Note(NOTE_LA, duty_cycle); 	// ���� `la` ����50% ռ�ձ�
					Play_Note(NOTE_SI, duty_cycle);		// ���� `si` ����50% ռ�ձ�
				break;
			default:
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
					
					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
				break;
		}
	}
}

