#ifndef __DCMI_H
#define __DCMI_H
#include "stm32f4xx_conf.h"

#define DCMI_DR_ADDRESS       	0x50050028   
#define FSMC_LCD_ADDRESS      	0x6C000002
 
#define OV_REG_NUM             79   //OV7725
#define SCCB_SIC_H()     GPIOE->BSRRL=GPIO_Pin_2
#define SCCB_SIC_L()     GPIOE->BSRRH=GPIO_Pin_2
#define SCCB_SID_H()     GPIOB->BSRRL=GPIO_Pin_5
#define SCCB_SID_L()     GPIOB->BSRRH=GPIO_Pin_5
#define SCCB_DATA_IN      SCCB_SID_IN()
#define SCCB_DATA_OUT     SCCB_SID_OUT()
// #define SCCB_SID_STATE	 GPIOE->IDR&GPIO_Pin_5

extern void Cam_Start(void);
extern void Cam_Stop(void);
extern unsigned char  OV7725_Init(void);
extern void Cam_Start_ov7725(void);
extern void OV7725_HW(unsigned int  hstart,unsigned int vstart,unsigned int hstop,unsigned int vstop);
void SCCB_Init(void);
void SCCB_SID_OUT(void);
void SCCB_SID_IN(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void noAck(void);
unsigned char  SCCB_Write(unsigned char  m_data);
unsigned char  SCCB_Read(void);
unsigned char  OV7725_WriteReg(unsigned char  regID, unsigned char  regDat);
unsigned char  OV7725_ReadReg(unsigned char  regID, unsigned char  *regDat);
void OV7725_Reset(void);
unsigned char  OV7725_ReadID(void);

extern void set_15fps(void); 
extern void set_25fps(void);
extern void set_30fps(void);
extern void set_wb_office(void);
extern void rmov_banding_50hz(void);
extern void rmov_banding_60hz(void);
extern void set_wb_sunny(void);
extern void set_wb_cloudy(void);
extern void set_wb_home(void);
extern void set_wb_night(void);


static const unsigned char  OV7725_reg[OV_REG_NUM][2]=
{	
   
 
      
  ////////////////////////////////////////////////////////////
      /*以下为OV7725 QVGA RGB565参数 
       guanfu_wang  
       http://mcudiy.taobao.com/
      */
  /////////////////////////////////////////////////////////// 
  {0x0c,0x00},///重要 flip control bit7:vertical bit6:horizenal  0x80 0xc0
  {0x09,0x03},
  {0x0d,0x41},//PLL bit[7:6]: 01--4x 10--6x 11-8x
  {0x0f,0x01},//Auto Window Setting bit[0]
/*--------------White Balance---------------*/
	{0x13,0xff},//AGC/AWB/AEC Enable
//(0x01,0x5a)'  //Sunny
//(0x02,0x5c)'
  {0x0e,0x01}, //bit[7]:auto frame rate control 0xe5:night mode 0x01:normal
//(0x2d,0x00),
//(0x2e,0x00),
/*__________________________________________*/

  {0x14,0x06},
  {0x15,0x00},//重要参数 bit[5]:0--free run pclk 1--pclk does not toggle during hor blank
 
{0x11,0x01},//CLKRC  bit[5:0] internal clock pre-scaler
  {0x12,0x46},//QVGA RGB565 0x46;vga 0x06 
              //bit[6]:0--VGA 1--QVGA bit[3:2]:00--GBR422 01--RGB565 10--RGB555 11--RGB444
              //bit[1:0]:00--YUV 01--Processed Bayer Raw 10--RGB 11--Bayer Raw

	//(0x10,0x00) AEC
  
    //{0x32,0x00},
    {0x32,0x00},//HREF
    {0x2a,0x00},//EXHCL

    {0x17,0x26}, //Hstart
    {0x18,0xA0}, //Hsize
    {0x19,0x07}, //Vstart
    {0x1A,0xF0}, //Vsize
    {0x29,0xA0}, //HoutSize
    {0x2C,0xF0}, //VoutSize

 
	//{0x33,0x40},
  {0x33,0x00},//  fps
  {0x34,0x00},//7f
  //{0x23,0x01},
  {0x22,0xaf},
  {0x23,0x01},


  {0x42,0x7f},
  {0x4d,0x00},//0x09
  {0x63,0xf0},
  {0x64,0x1f},
  {0x65,0x20},
  {0x66,0x00},
  {0x67,0x00},
  {0x69,0x5d},  //0x50
 
   

  {0x24,0x75},
  {0x25,0x63},
  {0x26,0xd1},
  {0x2b,0xff},
  {0x6b,0x00},
  
  {0x8e,0x10},
  {0x8f,0x00},
  {0x90,0x00},//
  {0x91,0x00},//
  {0x92,0x00},//
  {0x93,0x00},
  
  {0x94,0x2c},
  {0x95,0x24},
  {0x96,0x08},
  {0x97,0x14},
  {0x98,0x24},
  {0x99,0x38},
  {0x9a,0x9e},
  {0x9b,0x00},//set luma 
  {0x9c,0x20},//set contrast //对比
  {0xa7,0x40},//set saturation  
  {0xa8,0x40},//set saturation 
  {0xa9,0x80},//set hue 
  {0xaa,0x80},//set hue 
  
  {0x9e,0x81},
  {0xa6,0x06},

  {0x7e,0x0c},
  {0x7f,0x16},
  {0x80,0x2a},
  {0x81,0x4e},
  {0x82,0x61},
  {0x83,0x6f},
  {0x84,0x7b},
  {0x85,0x86},
  {0x86,0x8e},
  {0x87,0x97},
  {0x88,0xa4},
  {0x89,0xaf},
  {0x8a,0xc5},
  {0x8b,0xd7},
  {0x8c,0xe8},
  {0x8d,0x20},
  
  {0x49,0x10},
  {0x4a,0x10},
  {0x4b,0x14},
  {0x4c,0x17},
  {0x46,0x05},
  
  {0x47,0x08},
  
  {0x00,0x60},   

};



#endif 

