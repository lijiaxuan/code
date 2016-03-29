/* 
* @Author: obsidian
* @Date:   2016-01-16 14:57:21
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-22 20:16:14
*/

#include "../inc/radar.h"
#include "../inc/radar_global.h"
#include "driver/delay/inc/delay.h"
#include "driver/math/inc/math.h"
#include "misc.h"
#include "stm32f10x.h"
#include "math.h"
#include "stm32f10x_dma.h"
#include "stm32f10x.h"
#include "module/def.h"
#include "driver/can/inc/can.h"
#include "module/steer/inc/steer_global.h"
#ifndef PI 
#define PI 3.1415926
#endif
static unsigned char		radar_raw_data[0x420] 	= {0};
static unsigned short		current_index 						= 0;
static 	struct Radar_data	radar_data[10] 			= {0};
static	unsigned short		line_index 							= 0;
static  unsigned int			dis[550] = {0};

static unsigned short		distance = 120;

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

void update()
{
	unsigned short 				raw_index			 	= 0;

	signed short 				tmp_distance 			= 0;
	float						tmp_radian 				= 0;

	unsigned char 				*pdata 					= (unsigned char *)&radar_data;
	
	struct Radar_point 			last_radar_point 		= {0,0};
	
	unsigned char				find_start 				= 0;
    unsigned short				last_index			    = 0;
	float 				island_id				= 0;
	unsigned short				index = 0;
	signed short				left_r;
	signed short				right_r;
	for(raw_index = 0;raw_index < (current_index - 20) / 2;raw_index ++)
	{
		tmp_distance = (radar_raw_data[raw_index*2 + 19] << 6) + (radar_raw_data[raw_index*2 + 20]);
		tmp_radian = - ( - PI * 3 / 4.0 + (128 + raw_index) * RADIAN_STEP);
		dis[index] = tmp_distance;
		index ++;
	}
	for(index = 0; index < 256; index ++)
	{
		if(dis[index] > 500 && dis[index + 1] < 500 & dis[index + 1] > 200)
		{
			right_r = index;
		}
	}
	for(index = 256; index < 512; index ++)
	{
		if(dis[index] > 500 && dis[index - 1] < 500 && dis[index - 1] > 200)
		{
			left_r = index;
		}
	}
	if(left_r > 276 && left_r < 500 && right_r > 10 && right_r < 246)
	{
		steer.set_angle((left_r + right_r - 512.0) * 1);
	}//get_coordinate();
}

void get_angle()
{
	steer.set_angle(3 * 57.3 * (radar_data[0].start_r + radar_data[0].end_r));
}

void rotate(float a[] , float theta)
{
	float tmp[2] = {0};
	tmp[0] = a[0];
	tmp[1] = a[1];
	a[0] = tmp[0] * cos(theta) + tmp[1] * sin(theta);
	a[1] = -tmp[0] * sin(theta) + tmp[1] * cos(theta);
}

void get_coordinate() // 0,3,0,8
{
	float 	c1[2] = {0};
	float   c2[2] = {0};
	float   l1 	  =  0 ;
	float 	l2    =  0 ;
	float   th_l1 =  0 ;
	float   th_l2 =  0 ;
	float   island_phi[12] = {0};
	float   r1	  =  0 ;
	float   r2    =  0 ;
	float   tmp1[2] = {0};
	float   tmp2[2] = {0};
	float   tmp3[2] = {0};
	float   delta_theta1 = 0;
	float   delta_theta2 = 0;
	float   theta_cl     = 0;
	float   cos_c		 = 0;
	float   sin_c        = 0;
	float   cl           = 0;
	float   i_r	         = 0;

	float 	delta_x		 = 0;
	float   delta_y      = 0;
	float   delta_radian = 0;
	struct  Point pre_real_point = {0}; 
	CanTxMsg  TxM;
	if(line_index < 3)
	{return;}
	if(radar.direction == 0)
	{
		island_phi[0] = radar_data[0].start_r;
		island_phi[3] = radar_data[1].start_r;
		island_phi[7] = radar_data[2].start_r;
	}
	else
	{
		island_phi[0] = radar_data[line_index - 1].end_r;
		island_phi[3] = radar_data[line_index - 2].end_r;
		island_phi[7] = radar_data[line_index - 3].end_r;
	}
	l1 = sqrt((island[0].x - island[3].x) * (island[0].x - island[3].x) 
			+ (island[0].y - island[3].y) * (island[0].y - island[3].y)) / 2;
	l2 = sqrt((island[0].x - island[7].x) * (island[0].x - island[7].x) 
			+ (island[0].y - island[7].y) * (island[0].y - island[7].y)) / 2;
	th_l1 = atan((island[0].y - island[3].y) / (island[0].x - island[3].x));
	if(island[0].x - island[3].x < 0)
	{
		th_l1 += PI;
	}
	th_l2 = atan((island[0].y - island[7].y) / (island[0].x - island[7].x));
	if(island[0].x - island[7].x < 0)
	{
		th_l2 += PI;
	}

	delta_theta1 = fabs(island_phi[0] - island_phi[3]);
	delta_theta2 = fabs(island_phi[0] - island_phi[7]);
	tmp1[0] = -l1;
	tmp1[1] = l1 / tan(delta_theta1);
	tmp2[0] = -l2;
	tmp2[1] = l2 / tan(delta_theta2);

	r1 = l1 / sin(delta_theta1);
	r2 = l2 / sin(delta_theta2);

	rotate(tmp1,PI - th_l1);
	rotate(tmp2,PI - th_l2);

	c1[0] = island[3].x + tmp1[0];
	c1[1] = island[3].y + tmp1[1];
	c2[0] = island[7].x + tmp2[0];
	c2[1] = island[7].y + tmp2[1];

	theta_cl = atan((c2[1] - c1[1])/(c2[0] - c1[0]));
	if(c2[0] - c1[0] < 0)
	{
		theta_cl += PI;
	}
	cl = sqrt((c2[1] -c1[1])*(c2[1] -c1[1]) + (c2[0] -c1[0])*(c2[0] -c1[0]));

	cos_c = (r1 * r1 + cl * cl - r2 * r2) / (2.0 * r1 * cl);
	sin_c = sqrt(1 - cos_c * cos_c);

	tmp3[0] = r1 * sin_c;
	tmp3[1] = r1 * cos_c;
	rotate(tmp3, - theta_cl + PI / 2.0);

	radar.x = c1[0] + tmp3[0];
	radar.y = c1[1] + tmp3[1];
	i_r = atan((radar.y - island[3].y) / (radar.x - island[3].x));
	if(radar.x - island[3].x > 0)
	{
		i_r += PI;
	}
// 	if(radar.x < 800 || radar.x > 1200 || radar.y < 6600 || radar.y > 7100)
// 	{

// 		radar.x = 0;
// 	  	radar.y = 0;
// 	  	radar.radian = 0;
//  		TxM.RTR = CAN_RTR_Data;
//  		TxM.IDE = CAN_Id_Standard;
// 		TxM.DLC = 8;
//     	TxM.StdId = 0xff;
//     	*((float *)TxM.Data) = 0;
//     	*((float *)(TxM.Data + 4)) = 0;
//     	CAN_Transmit(CAN1,&TxM);
// 		return;
// 	}
// 	
	if(radar.direction == 0)
	{
		radar.radian = (i_r + island_phi[3]);
		radar.x = - radar.x;
		radar.radian = PI - radar.radian;
	}
	else
	{
	  radar.radian = (i_r - island_phi[3]);
	}
 	TxM.RTR = CAN_RTR_Data;
 	TxM.IDE = CAN_Id_Standard;
	TxM.DLC = 8;
    TxM.StdId = 0x2;
    *((float *)TxM.Data) = radar.x;
    *((float *)(TxM.Data + 4)) = radar.y;
    CAN_Transmit(CAN1,&TxM);
    for(int i = 0;i < 100; i ++);
    TxM.StdId = 0x3;
    *((float *)TxM.Data) = radar.radian;
    *((float *)(TxM.Data + 4)) = 0;
    CAN_Transmit(CAN1,&TxM);
}



void radar_init()
{
	uart_init(115200);
	//radar.send_qt();
 	delay_ms(3);
 	radar.send_bm();
 	delay_ms(20);
	radar.status = ON;
}

void send_qt()
{
	unsigned short index = 0;
	while(radar_command.QT[index] != 0x00)
	{
		USART_SendData(radar.USARTx, radar_command.QT[index]);
		index ++;
	}
}

void send_gs()
{
	unsigned short index = 0;
	if(radar.status == OFF)
		{return;}
	while(radar_command.GDGS[index] != 0x00)
	{
		USART_SendData(radar.USARTx, radar_command.GDGS[index]);
		index ++;
	}
}

void send_bm()
{
	unsigned short index = 0;
	while(radar_command.BM[index] != 0x00)
	{
		USART_SendData(radar.USARTx, radar_command.BM[index]);
		index ++;
	}
}

/*
USART
 */

#ifdef RADAR

void uart_init(u32 bound)
{

    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	//USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
		//USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 


 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
   	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
   	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_Init(&NVIC_InitStructure);


	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ClearFlag(USART3,USART_FLAG_TC);
	USART_Init(USART3, &USART_InitStructure); 
		
	USART_Cmd(USART3, ENABLE);     
}
static int count = 0;
void USART3_IRQHandler(void)
{
	unsigned char			data = 0;
	static signed char   	dir = 1; 
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	data = USART_ReceiveData(USART3);
	if(data == 0x0a)
	{
		if(dir == -1)
		{
			if(current_index >= 0x413 && current_index <= 0x416)
			{
				//count ++;
				radar.update();
			}
			current_index = 0;

			line_index = 0;
			dir = 1;
			radar.send_gs();
		}
		else
		{
			current_index -- ;
			dir = -1;
		}
	}
	else
	{
		if(data != 0x0d)
		{
			radar_raw_data[current_index] = data - 0x30;
			current_index ++;
			dir = 1;
		}
	}
	//USART_SendData(USART1,data);
	//DMA??
}

#endif
