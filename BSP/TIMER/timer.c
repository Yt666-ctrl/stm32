#include "timer.h"
#include "usart2.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2018/6/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Init(u16 arr,u16 psc)
{  
	__HAL_RCC_TIM3_CLK_ENABLE(); 
	
    TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
    TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
	TIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;//ʹ���Զ�����
    HAL_TIM_Base_Init(&TIM3_Handler);
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE 
	
	HAL_NVIC_SetPriority(TIM3_IRQn,0,1);    //�����ж����ȼ�����ռ���ȼ�0�������ȼ�1
	HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
	
  
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
   
	if(__HAL_TIM_GET_FLAG(&TIM3_Handler,TIM_FLAG_UPDATE)!=RESET)    //�����ж�
    {
        __HAL_TIM_CLEAR_IT(&TIM3_Handler,TIM_IT_UPDATE);            //����ж�
        USART1_RX_STA|=1<<15;	                                    //��ǽ������
        __HAL_TIM_DISABLE(&TIM3_Handler);                           //�رն�ʱ��7 
    }
		
}

