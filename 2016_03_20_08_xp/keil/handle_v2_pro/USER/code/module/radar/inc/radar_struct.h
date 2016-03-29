/* 
* @Author: obsidian
* @Date:   2016-01-16 14:40:34
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-26 17:43:23
*/
#ifndef RADAR_STRUCT_H
#include "stm32f10x_usart.h"

#define RADAR_RAW_DATALEN	0x2c0
#define RADAR_DATALEN		20
#define DISTANCE_MIN		50
#define DISTANCE_MAX		2000
#define RADIAN_STEP			3.1415926 / 512.0
// 213   555
// 

struct Radar_point
{
	float			radian;
	signed int 	distance;
};

struct Radar_data
{
	float				start_r;
	float 				end_r;
	signed int		start_d;
	signed int		end_d;
	unsigned int 		island_id;
};

struct Radar_commands
{
	unsigned char			GDGS[16];
	unsigned char			BM[6];
	unsigned char			QT[6];
};
struct Radar
{
	USART_TypeDef*  	USARTx;
	void				(*init)(float * px,float *py,float* pradian);
	void				(*update)();
	void				(*send_qt)();
	void				(*send_bm)();
	void				(*send_gs)();
	unsigned short		counter;
	unsigned char		status;
	float				angle;
	float				x;
	float				y;
};


#define RADAR_STRUCT_H
#endif
