#include "sim900a.h"		 	 	 	 	 	 
#include "string.h"    	
#include "usart2.h" 
#include "tim.h"
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

//sim800C���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//����,�ڴ�Ӧ������λ��(str��λ��)

u8* sim800c_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART1_RX_STA&0X8000)//���յ�һ��������
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//��sim800C��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 sim800c_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART1_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		USART1->DR=(u32)cmd;
	}else u1_printf("%s\r\n",cmd);//��������
	
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{ 
	   while(--waittime)	//�ȴ�����ʱ
	   {
		   Delay_ms(10);
		   if(USART1_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		   {
			   if(sim800c_check_cmd(ack))break;//�õ���Ч���� 
			   USART1_RX_STA=0;
		   } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
} 

u8 sim800c_check_call_sms_ready(u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART1_RX_STA=0;
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{ 
	   while(--waittime)	//�ȴ�����ʱ
	   {
		   Delay_ms(10);
		   if(USART1_RX_STA&0X8000)//���յ��ڴ���Ӧ����
		   {
			   if(sim800c_check_cmd(ack))break;//�õ���Ч���� 
			   USART1_RX_STA=0;
		   } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
} 

//GSM���(SIM��׼�����Ƿ�ע��ɹ�)
//����ֵ:2,����
//����,�������
u8 sim800c_gsminfo_show(void)
{
    static u8 sim_flag=0;
	
	USART1_RX_STA=0;
	
	switch(sim_flag)
	{
		case 0: 
			    if(sim800c_send_cmd("AT+CPIN?","OK",20)) //��ѯSIM���Ƿ���λ 
				{
					 //printf("SIM��������!\r\n");
				}else
				{
					 sim_flag=1;
					 //printf("SIM������!\r\n");
				}break;
		case 1: 
			    if(sim800c_send_cmd("AT+CREG?","+CREG: 0,1",20))//��ѯSIM�������Ƿ���ע��
		        {
					 //printf("��������ע��!\r\n");
				}else
				{
					 sim_flag=2;
					 //printf("����ע��ɹ�!\r\n");
				}break;
	}
	USART1_RX_STA=0;
	
	return sim_flag;
} 

/*****************************************��������*******************************************************************/


static char *phone="\"00310033003200390038003900300034003000360033\"";//���Ž��շ��ֻ��� ��18628636497��UNICODE��
static char *content="67094EBA67654E86";//���Ͷ������� ���������ˡ� UNICODE��

/*******************************************************************************************************************/

//����TEXT�ı�ģʽ�������Ķ���
void Text_SMS(void)
{
	char temp[100]="AT+CMGS=";

	sim800c_send_cmd("AT+CMGF=1","OK",200);	// ����SIM800C����ģʽΪ�ı�ģʽ
	sim800c_send_cmd("AT+CSMP=17,167,1,8","OK",200);// ���ö��ŷ��Ͳ�����17��ʾ���ö���Э�������167��ʾ�����ŵ�������1��ʾ������Ϣ���ͣ�8��ʾ������Ϣ����
	sim800c_send_cmd("AT+CSCS=\"UCS2\"","OK",200);// �����ַ���ΪUCS2�����ڷ���Unicode����Ķ���

    strcat(temp,(char*)phone); //�ַ���ƴ�Ӻ���
	sim800c_send_cmd((u8*)temp,">",200);//���Ͷ�������+�绰���� 
	u1_printf("%s",content);//���Ͷ������ݵ�GSMģ��
    Delay_ms(50);//����������ʱ,������շ�������Ϣ��ȫ
	
	sim800c_send_cmd((u8*)0X1A,"+CMGS:",1000);//���ͽ�����,�ȴ��������(��ȴ�10����,��Ϊ���ų��˵Ļ�,�ȴ�ʱ��᳤һЩ)
}
/*
//sim800C���Գ���
void sim800c_test(void)
{ 
	u8 sim_ready=0;
	
	while(sim800c_send_cmd("AT","OK",100))//����Ƿ�Ӧ��ATָ�� 
	{
		//printf("δ��⵽ģ��!!!\r\n");Delay_ms(800);
		//printf("δ��⵽ģ��!!!\r\n");HAL_Delay(800);
		//printf("��������ģ��...\r\n");Delay_ms(400);
//        LED0=!LED0;		
	} 	 
	sim800c_send_cmd("ATE0","OK",200);//ָ�����
	//printf("\r\nSIM800C ��Ӣ�Ķ��ŷ��Ͳ���\r\n");
	
	//sim800c_send_cmd("AT+CREG=2","OK",200);
	
	while(1)
	{
		Delay_ms(10); 
		if(sim_ready)//SIM������.
		{
			Text_SMS();//���Ͷ��Ų���
	    } 
		if(!sim_ready)//SIM��û����
		{
			if(sim800c_gsminfo_show()==2)//GSM���(SIM���Ƿ�׼����ע��ɹ�)
//			if(sim800c_gsminfo_show()==1)//GSM���(SIM���Ƿ�׼����ע��ɹ�) 
				sim_ready=1; 
			else 
				sim_ready=0;
		}	
		 
	} 	
}
*/

