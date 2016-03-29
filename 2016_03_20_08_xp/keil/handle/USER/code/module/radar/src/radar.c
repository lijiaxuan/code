/* 
* @Author: obsidian
* @Date:   2016-01-16 14:57:21
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-23 19:37:26
*/
#include "../inc/radar.h"
#include "../inc/radar_global.h"
#include "driver/delay/inc/delay.h"
#include "misc.h"
#include "stm32f10x.h"
#include "math.h"
#include "stm32f10x_dma.h"
#include "stm32f10x.h"
#ifndef PI 
#define PI 3.1415926
#endif

static unsigned char		radar_raw_data[RADAR_RAW_DATALEN] 	= {0};
static unsigned short		current_index 						= 0;
static 	struct Radar_data	radar_data[RADAR_DATALEN] 			= {0};
static	unsigned short		line_index 							= 0;


//static  
struct Islan_point
{
	float x;
	float y;
};

static struct Islan_point island[4]= 
{
	{687.88,7128.40},
	{-19.21,7128.40},
	{1041.44,6633.43},
	{334.34,6633.43},
	
};

static float island_distance = 707.1;

// judge angle
void uart_init(u32 bound);
void get_coordinate();


void radar_init()
{
	uart_init(115200);
// 	radar.send_qt();
 	delay_ms(3);
 	radar.send_bm();
 	delay_ms(3);
 	radar.status = 1;
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
	while(radar_command.GDGS[index] != 0x00)
	{
		USART_SendData(radar.USARTx, radar_command.GDGS[index]);
		index ++;
	}
}

void update()
{
	unsigned short 				raw_index			 	= 0;
	unsigned short 				index 					= 0;

	signed short 				tmp_distance 			= 0;
	float						tmp_radian 				= 0;

	unsigned char 				*pdata 					= (unsigned char *)&radar_data;
	
	struct Radar_point 			last_radar_point 		= {0,0};
	
	unsigned char				find_start 				= 0;
    unsigned short				last_index			    = 0;
	for(raw_index = 0;raw_index < (current_index - 20) / 2;raw_index ++)
	{
		tmp_distance = (radar_raw_data[raw_index*2 + 19] << 6) + (radar_raw_data[raw_index*2 + 20]);
		tmp_radian = - PI * 3 / 4.0 + (213 + raw_index) * RADIAN_STEP;
		
		if(tmp_distance > DISTANCE_MAX)
		{
			tmp_distance = 0;
		}
		if(fabs(tmp_distance - last_radar_point.distance) > 30)
		{
			if(find_start == 0)
			{
				radar_data[line_index].radar_point[0].radian = tmp_radian;
				radar_data[line_index].radar_point[0].distance = tmp_distance;
				find_start = 1;
				last_index = raw_index;
			}
			else if(find_start == 1)
			{
				if(raw_index - last_index > 10)
				{
					radar_data[line_index].radar_point[1].distance = last_radar_point.distance;
					radar_data[line_index].radar_point[1].radian = tmp_radian - RADIAN_STEP;
					find_start = 0;
					line_index ++;
				}
				else
				{
					radar_data[line_index].radar_point[0].radian = 0;
					radar_data[line_index].radar_point[0].distance = 0;
					find_start = 0;
				}
				if(tmp_distance > 30)
				{
					radar_data[line_index].radar_point[0].radian = tmp_radian;
					radar_data[line_index].radar_point[0].distance = tmp_distance;			
					find_start = 1;
					last_index = raw_index;
				}
			}
		}
		last_radar_point.distance = tmp_distance;
		last_radar_point.radian = tmp_radian;
	}
	if(find_start == 1)
	{
		radar_data[line_index].radar_point[1].distance = tmp_distance;
		radar_data[line_index].radar_point[1].radian = tmp_radian;
	}
	get_coordinate();
	USART_SendData(USART1, 0xff);
	USART_SendData(USART1, 0xff);
	for(index = 0;index <50 * 2 * 8;index ++)
	{
		USART_SendData(USART1, *(pdata + index));
		*(pdata + index) = 0;
	}

	for(index = 0;index <4; index ++)
	{
		USART_SendData(USART1, 0x01);
	}

	pdata = (unsigned char *)& radar.x;
	for(index = 0;index <4; index ++)
	{
		USART_SendData(USART1, *(pdata + index));
	}
	pdata = (unsigned char *)& radar.y;
	for(index = 0;index <4; index ++)
	{
		USART_SendData(USART1, *(pdata + index));
	}
	pdata = (unsigned char *)& radar.angle;
	for(index = 0;index <4; index ++)
	{
		USART_SendData(USART1, *(pdata + index));
	}
}

void get_coordinate()
{
	float a = 0;
	float b = 0;
	float c1[2] = {0};
	float c2[2] = {0};
	float r1 = 0;
	float r2 = 0;
	float t1 = 0;
	float t2 = 0;
	float theta_l = 0;
	float cl = 0;
	float cos_rc = 0;
	float sin_rc = 0;
	float delta_theta1 = 0;
	float delta_theta2 = 0;
	float x_ = 0;
	float y_ = 0;
	float xs = 0;
	float ys = 0;
	if(line_index == 4)
	{
		delta_theta1 = radar_data[1].radar_point[1].radian - radar_data[0].radar_point[1].radian;
		delta_theta2 = radar_data[3].radar_point[0].radian - radar_data[2].radar_point[0].radian;
		t1 = island_distance / (2.0 * tan(delta_theta1));
		t2 = island_distance / (2.0 * tan(delta_theta2));
		r1 = island_distance / (2.0 * sin(delta_theta1));
		r2 = island_distance / (2.0 * sin(delta_theta2));

		c1[0] = island[0].x - island_distance / 2.0;
		c1[1] = island[0].y - t1;

		c2[0] = island[2].x - island_distance / 2.0;
		c2[1] = island[2].y + t2;

		theta_l = atan((c2[1] - c1[1])/(c2[0] - c1[0])) + PI / 2;
		cl = sqrt((c2[0] - c1[0]) * (c2[0] - c1[0]) + (c2[1] - c1[1]) * (c2[1] - c1[1]));
		cos_rc = (r2 * r2 + cl * cl - r1 * r1) / (2 * r2 * cl);
		sin_rc = sqrt(1 - cos_rc * cos_rc);
		y_ = -r2 * cos_rc;
		x_ = r2 * sin_rc;
		xs = x_ * cos(-theta_l) + y_ * sin(-theta_l);
		ys = -x_ * sin(-theta_l) + y_ * cos(-theta_l);
		radar.x = c2[0] - xs;
		radar.y = c2[1] - ys;
		radar.angle =PI/2 -  (atan((radar.x - island[0].x)/(island[0].y - radar.y )) - radar_data[0].radar_point[1].radian);
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	  
 	//USART1_RX	  PA.10
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);     


}

void USART3_DMA3_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* USARTy RX DMA1 Channel --DMA1_Channel5(triggered by USART1 Rx event) Config */
	// DMA_DeInit(DMA1_Channel5); 												
	// DMA_InitStructure.DMA_PeripheralBaseAddr =(unsigned int)&USART3->DR;		
	// DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)radar_raw_data;	
	// DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					
	// DMA_InitStructure.DMA_BufferSize = 0x80f;					
	// DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	
	// DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				
	// DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	// DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;		
	// DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						
	// DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				
	// DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;						
	// DMA_Init(DMA1_Channel5, &DMA_InitStructure);							

	
	// USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);

	// DMA_ITConfig(DMA1_Channel3,DMA_IT_TC, ENABLE);

	// DMA_Cmd(DMA1_Channel3, DISABLE);
}
// void DMA1_Channel3_IRQHandler(void)
// {
// 	if(DMA_GetITStatus(DMA1_IT_GL3)==SET)
// 	{
// 		DMA_ClearITPendingBit(DMA1_IT_GL3);
// 	}
// }



void USART3_IRQHandler(void)
{
	unsigned char			data = 0;
	static signed char   	dir = 1; 
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	data = USART_ReceiveData(USART3);
	if(radar.status == 0)
		{return;}
	if(data == 0x0a)
	{
		if(dir == -1)
		{
			if(current_index == 0x2bf)
			{
				radar.update();
			}
			current_index = 0;

			line_index = 0;
			dir = 1;
		}
		else
		{
			current_index = current_index - 1;
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
