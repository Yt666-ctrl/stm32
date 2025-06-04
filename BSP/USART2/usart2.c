#include "usart2.h"	  
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "timer.h" 
//////////////////////////////////////////////////////////////////////////////////	   
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����2��ʼ������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2018/6/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved  
////////////////////////////////////////////////////////////////////////////////// 


UART_HandleTypeDef huart1; //UART2���

//���ڷ��ͻ����� 	
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 	//���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
//���ڽ��ջ����� 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART1_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART1_RX_STA=0;   

void USART1_IRQHandler(void)
{
	u8 res;
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        HAL_UART_Receive(&huart1,&res,1,1000); 
		if((USART1_RX_STA&(1<<15))==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 
			if(USART1_RX_STA<USART1_MAX_RECV_LEN)	//�����Խ�������
			{
                __HAL_TIM_SET_COUNTER(&TIM3_Handler,0);	//���������			
				if(USART1_RX_STA==0) 				//ʹ�ܶ�ʱ��3���ж� 
				{
                    __HAL_TIM_ENABLE(&TIM3_Handler); //ʹ�ܶ�ʱ��3
				}
				USART1_RX_BUF[USART1_RX_STA++]=res;	//��¼���յ���ֵ	 
			}else 
			{
				USART1_RX_STA|=1<<15;				//ǿ�Ʊ�ǽ������
			} 
		}
    }         													 
}   
//��ʼ��IO ����1
//bound:������ 
void USART1_Init(u32 bound)
{  	 
	GPIO_InitTypeDef GPIO_Initure;
	       
    //UART2 ��ʼ������
    __HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
    __HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART2ʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;			//����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

	GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
	GPIO_Initure.Mode=GPIO_MODE_INPUT;      //��������
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10

	huart1.Instance=USART1;					    //USART1
	huart1.Init.BaudRate=bound;				    //������
	huart1.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	huart1.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	huart1.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	huart1.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	huart1.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&huart1);					    //HAL_UART_Init()��ʹ��UART1
    
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);  //���������ж�
	HAL_NVIC_EnableIRQ(USART1_IRQn);				    //ʹ��USART2�ж�ͨ��
	HAL_NVIC_SetPriority(USART1_IRQn,0,2);			    //��2�����ȼ�0,2,������ȼ� 
 
	TIM3_Init(99,7199);	                //10ms�ж�һ��
    __HAL_TIM_DISABLE(&TIM3_Handler);   //�رն�ʱ��3 
	USART1_RX_STA=0;				    //���� 
}

//����1,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u1_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)//ѭ����������
	{
		while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
		USART1->DR=USART1_TX_BUF[j];  
	}
	    va_end(ap);
}

