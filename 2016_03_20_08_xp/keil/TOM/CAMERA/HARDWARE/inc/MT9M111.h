#ifndef __MT9M111_H
#define __MT9M111_H

#include "stm32f4xx_conf.h"
#include "delay.h"
#define CHANGE_REG_NUM 162

#define IMG_STA_X 10
#define IMG_STA_Y 34

#define imgwidth 320
#define imgheight 240

#define DCMI_DR_ADDRESS       	0x50050028   
#define FSMC_LCD_ADDRESS      	0x6C000002
#define OV7670_REG_NUM  		160
#define SCL_H     GPIOG->BSRRL=GPIO_Pin_8
#define SCL_L     GPIOG->BSRRH=GPIO_Pin_8
#define SDA_H     GPIOG->BSRRL=GPIO_Pin_7
#define SDA_L     GPIOG->BSRRH=GPIO_Pin_7

#define OE_H()              GPIOH->BSRRL = GPIO_Pin_13
#define OE_L()              GPIOH->BSRRH = GPIO_Pin_13
#define STANDBY_H()         GPIOG->BSRRL = GPIO_Pin_6
#define STANDBY_L()         GPIOG->BSRRH = GPIO_Pin_6

#define SCL_read      GPIOG->IDR  & GPIO_Pin_8
#define SDA_read      GPIOG->IDR  & GPIO_Pin_7



#define SCCB_DATA_IN      SCCB_SID_IN()
#define SCCB_DATA_OUT     SCCB_SID_OUT()
#define SCCB_SID_STATE	  GPIOG->IDR & GPIO_Pin_7

void Cam_Init(void);
unsigned char  OV7670_Init(void);
extern void Cam_Start(void);
extern void Cam_Stop(void);
void OV7670_HW(unsigned int  hstart,unsigned int vstart,unsigned int hstop,unsigned int vstop);
void SCCB_Init(void);
void SCCB_SID_OUT(void);
void SCCB_SID_IN(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SENSOR_CLK_init_ON(void);
void SENSOR_CLK_init_OFF(void);
void noAck(void);
unsigned char SENSOR_init(void);
void I2C_Ack(void);
void I2C_delay(void);
unsigned char I2C_GPIO_Config(void);
void I2C_NoAck(void);
unsigned char I2C_ReadByte(void);  //数据从高位到低位//
void I2C_SendByte(unsigned char SendByte); //数据从高位到低位//
int I2C_Start(void);
void I2C_Stop(void);
int I2C_WaitAck(void); 	 //返回为:=1有ACK,=0无ACK




//void delay_ms(unsigned int time);
//void delay_us(unsigned int i);

#endif 

