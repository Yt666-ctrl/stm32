#ifndef __sgp30_H
#define __sgp30_H
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

//#define  SGP30_SCL   PBout(0)
//#define  SGP30_SDA   PBout(1)



// USART GPIO 引脚宏定义
#define  SGP30_SCL_GPIO_CLK        RCC_APB2Periph_GPIOB
#define  SGP30_SCL_GPIO_PORT       GPIOB
#define  SGP30_SCL_GPIO_PIN        GPIO_PIN_0

#define  SGP30_SDA_GPIO_SDA        RCC_APB2Periph_GPIOB
#define  SGP30_SDA_GPIO_PORT       GPIOB
#define  SGP30_SDA_GPIO_PIN        GPIO_PIN_1

#define  SGP30_SDA_READ()          HAL_GPIO_ReadPin(SGP30_SDA_GPIO_PORT, SGP30_SDA_GPIO_PIN) //
/* IO操作函数 */
#define SGP30_SCL(x)     do{ x ? \
                                HAL_GPIO_WritePin(SGP30_SCL_GPIO_PORT, SGP30_SCL_GPIO_PIN, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(SGP30_SCL_GPIO_PORT, SGP30_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                            }while(0)  
#define SGP30_SDA(x)     do{ x ? \
                                HAL_GPIO_WritePin(SGP30_SDA_GPIO_PORT, SGP30_SDA_GPIO_PIN, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(SGP30_SDA_GPIO_PORT, SGP30_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                            }while(0)  
														
														
#define SGP30_read  0xb1  //SGP30的读地址
#define SGP30_write 0xb0  //SGP30的写地址


void SGP30_IIC_Start(void);				//发送IIC开始信号
void SGP30_IIC_Stop(void);	  			//发送IIC停止信号
void SGP30_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint16_t SGP30_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t SGP30_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void SGP30_IIC_Ack(void);					//IIC发送ACK信号
void SGP30_IIC_NAck(void);				//IIC不发送ACK信号
void SGP30_IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t SGP30_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	
void SGP30_Init(void);				  
void SGP30_Write(uint8_t a, uint8_t b);
uint32_t SGP30_Read(void);

//uint32_t SGP30(void);
void SGP30(uint8_t *CO2Data, uint8_t *TVOCData);

#endif

