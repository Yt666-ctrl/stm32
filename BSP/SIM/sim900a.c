#include "sim900a.h"		 	 	 	 	 	 
#include "string.h"    	
#include "usart2.h" 
#include "tim.h"
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

//sim800C发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//其他,期待应答结果的位置(str的位置)

u8* sim800c_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART1_RX_STA&0X8000)//接收到一次数据了
	{ 
		USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//向sim800C发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim800c_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART1_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		USART1->DR=(u32)cmd;
	}else u1_printf("%s\r\n",cmd);//发送命令
	
	if(ack&&waittime)		//需要等待应答
	{ 
	   while(--waittime)	//等待倒计时
	   {
		   Delay_ms(10);
		   if(USART1_RX_STA&0X8000)//接收到期待的应答结果
		   {
			   if(sim800c_check_cmd(ack))break;//得到有效数据 
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
	if(ack&&waittime)		//需要等待应答
	{ 
	   while(--waittime)	//等待倒计时
	   {
		   Delay_ms(10);
		   if(USART1_RX_STA&0X8000)//接收到期待的应答结果
		   {
			   if(sim800c_check_cmd(ack))break;//得到有效数据 
			   USART1_RX_STA=0;
		   } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
} 

//GSM检测(SIM卡准备和是否注册成功)
//返回值:2,正常
//其他,错误代码
u8 sim800c_gsminfo_show(void)
{
    static u8 sim_flag=0;
	
	USART1_RX_STA=0;
	
	switch(sim_flag)
	{
		case 0: 
			    if(sim800c_send_cmd("AT+CPIN?","OK",20)) //查询SIM卡是否在位 
				{
					 //printf("SIM卡不存在!\r\n");
				}else
				{
					 sim_flag=1;
					 //printf("SIM卡存在!\r\n");
				}break;
		case 1: 
			    if(sim800c_send_cmd("AT+CREG?","+CREG: 0,1",20))//查询SIM卡网络是否已注册
		        {
					 //printf("网络正在注册!\r\n");
				}else
				{
					 sim_flag=2;
					 //printf("网络注册成功!\r\n");
				}break;
	}
	USART1_RX_STA=0;
	
	return sim_flag;
} 

/*****************************************参数配置*******************************************************************/


static char *phone="\"00310033003200390038003900300034003000360033\"";//短信接收方手机号 “18628636497”UNICODE码
static char *content="67094EBA67654E86";//发送短信内容 “有人来了” UNICODE码

/*******************************************************************************************************************/

//设置TEXT文本模式发送中文短信
void Text_SMS(void)
{
	char temp[100]="AT+CMGS=";

	sim800c_send_cmd("AT+CMGF=1","OK",200);	// 设置SIM800C短信模式为文本模式
	sim800c_send_cmd("AT+CSMP=17,167,1,8","OK",200);// 设置短信发送参数：17表示设置短信协议参数，167表示设置信道参数，1表示设置信息类型，8表示设置信息编码
	sim800c_send_cmd("AT+CSCS=\"UCS2\"","OK",200);// 设置字符集为UCS2，用于发送Unicode编码的短信

    strcat(temp,(char*)phone); //字符串拼接函数
	sim800c_send_cmd((u8*)temp,">",200);//发送短信命令+电话号码 
	u1_printf("%s",content);//发送短信内容到GSM模块
    Delay_ms(50);//必须增加延时,否则接收方接收信息不全
	
	sim800c_send_cmd((u8*)0X1A,"+CMGS:",1000);//发送结束符,等待发送完成(最长等待10秒钟,因为短信长了的话,等待时间会长一些)
}
/*
//sim800C测试程序
void sim800c_test(void)
{ 
	u8 sim_ready=0;
	
	while(sim800c_send_cmd("AT","OK",100))//检测是否应答AT指令 
	{
		//printf("未检测到模块!!!\r\n");Delay_ms(800);
		//printf("未检测到模块!!!\r\n");HAL_Delay(800);
		//printf("尝试连接模块...\r\n");Delay_ms(400);
//        LED0=!LED0;		
	} 	 
	sim800c_send_cmd("ATE0","OK",200);//指令不回显
	//printf("\r\nSIM800C 中英文短信发送测试\r\n");
	
	//sim800c_send_cmd("AT+CREG=2","OK",200);
	
	while(1)
	{
		Delay_ms(10); 
		if(sim_ready)//SIM卡就绪.
		{
			Text_SMS();//发送短信测试
	    } 
		if(!sim_ready)//SIM卡没就绪
		{
			if(sim800c_gsminfo_show()==2)//GSM检测(SIM卡是否准备和注册成功)
//			if(sim800c_gsminfo_show()==1)//GSM检测(SIM卡是否准备和注册成功) 
				sim_ready=1; 
			else 
				sim_ready=0;
		}	
		 
	} 	
}
*/

