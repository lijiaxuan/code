/* 
* @Author: obsidian
* @Date:   2016-01-16 14:57:21
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-14 14:32:43
*/

#include "../inc/radar.h"
#include "../inc/radar_global.h"
#include "driver/delay/inc/delay.h"
#include "driver/math/inc/math.h"
#include "driver/led/inc/led.h"
#include "module/GPS/inc/GPS_global.h"
#include "misc.h"
#include "stm32f10x.h"
#include "math.h"
#include "stm32f10x_dma.h"
#include "stm32f10x.h"
#include "module/def.h"
#include "stm32f10x_can.h"
#include "module/pid/inc/pid_global.h"
#ifndef PI 
#define PI 3.1415926
#endif
static unsigned char		radar_raw_data[20] 	= {0};
static unsigned short		current_index 						= 0;
static 	struct Radar_data	radar_data[RADAR_DATALEN] 			= {0};
static	unsigned short		line_index 							= 0;
static  unsigned int			dis[720] = {0};
static CanTxMsg  TxM;
//     1     0
// *   *     *   *
//	 *         *
//          car
//	     *          *
//	   *   *      *   *
//         2             

//static  
static struct Point island[12]= 
{
	{787.88,7228.40 },
	{687.88,7128.40 },
	{587.88,7228.40 },
	{81.21,7228.40  },
	{-19.21,7128.40 },
	{-119.34,7228.40},
	{234.34,6533.43 },
	{334.34,6633.43 },
	{434.34,6533.43 },
	{941.44,6533.43 },
	{1041.44,6633.43},
	{1141.44,6533.43},
};

void uart_init(u32 bound);
void get_coordinate(void);
void Can1_Init(void);

void update()
{
	float 	delta_x		 = 0;
	float   delta_y      = 0;
	float   delta_radian = 0;
	struct  Point pre_real_point = {0}; 
	#ifdef RIGHT
	if(radar.x < 800 || radar.x > 1200 || radar.y < 6600 || radar.y > 7100)
	{
		radar.status = ON;
		return;
	}
	#endif
	#ifdef LEFT
	if(radar.x > - 800 || radar.x < - 1200 || radar.y < 6600 || radar.y > 7100)
	{
		radar.status = ON;
		return;
	}
	#endif
	delta_x = gps.rear_point.x - radar.pre_point.x;
	delta_y = gps.rear_point.y - radar.pre_point.y;
	pre_real_point.x = radar.x - radar.len * cos(radar.radian);
	pre_real_point.y = radar.y - radar.len * sin(radar.radian);
	gps.rear_point.x = pre_real_point.x + delta_x;
	gps.rear_point.y = pre_real_point.y + delta_y;
	gps.status = ON;
	delta_radian = 3.1415926 / 2.0 - radar.radian - radar.pre_radian;
	gps.base_radian += delta_radian;
	gps.GPS_update();
	radar.status = OFF;

	
	pid.pid_clear();
	LED2_on;
}

void radar_init()
{
	Can1_Init();
}
void scan()
{
	TxM.RTR = CAN_RTR_Data;
 	TxM.IDE = CAN_Id_Standard;
	TxM.DLC = 8;
    TxM.StdId = 0x1;
    *((int *)TxM.Data) = 0;
    *((int *)(TxM.Data + 4)) = 0;
    #ifdef RIGHT
    TxM.Data[0] = 1;
    #else
    TxM.Data[0] = 0;
    #endif
    CAN_Transmit(CAN1,&TxM);
}
/*
USART
 */

void Can_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Configure CAN1 IOs **********************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         //PA11:CAN1-RX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			   //ÊäÈëÉÏÀ­
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					 //PA12:CAN1-TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			 //žŽÓÃÄ£Êœ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

void CAN_config_init(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
 
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
    // 36M 250k速率
  CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=DISABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=DISABLE;
  //CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_12tq;
  CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;
  CAN_InitStructure.CAN_Prescaler=9;
  CAN_Init(CAN1, &CAN_InitStructure);
  CAN_OperatingModeRequest( CAN1, CAN_OperatingMode_Normal);
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure); 
}
 
void Can1_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;   
      // 首先打开电源及时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  
   
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
   
    NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // 然后配置pin
	Can_GPIO_Config();
    CAN_config_init();
    CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE);  // fifo0中断
    CAN_ITConfig(CAN1, CAN_IT_FMP1 | CAN_IT_FF1 | CAN_IT_FOV1, ENABLE);  // fifo1中断
    //CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);                // 发送中断
}
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	static unsigned char recv1 = 0;
	static unsigned char recv2 = 0;
 	CanRxMsg   RxMessage;
	CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	if(RxMessage.StdId == 0x2)
	{
		radar.x = *((float *)(RxMessage.Data));
		radar.y = *((float *)(RxMessage.Data + 4));
		recv1 = 1;
	}
	else if(RxMessage.StdId == 0x3)
	{
		radar.radian = *((float *)(RxMessage.Data));
		recv2 = 1;
	}
	if(recv1 ==1 && recv2 == 1)
	{
		radar.update();
	}
}

//Ö÷¿ØÏòGPS°å·¢ËÍµÄÊýŸÝ
void CAN1_RX1_IRQHandler(void)
{

}

uint8_t Can_State = 0;
void Can_SendData(CanTxMsg* TxM)
{
	if(CAN_Transmit(CAN1,TxM)==CAN_NO_MB)
    {
			Can_State = 1;
    }
		else 
		{
			Can_State = 0;
		}
}
