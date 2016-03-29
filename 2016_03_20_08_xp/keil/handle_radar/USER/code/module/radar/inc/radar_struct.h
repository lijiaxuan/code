/* 
* @Author: obsidian
* @Date:   2016-01-16 14:40:34
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-14 14:24:45
*/
#ifndef RADAR_STRUCT_H
#include "stm32f10x_usart.h"
#include "../../def.h"
#define RADAR_RAW_DATALEN	0x413
#define RADAR_DATALEN		20
#define DISTANCE_MIN		50
#define DISTANCE_MAX		3000
#define RADIAN_STEP			3.1415926 / 512.0
// 213   555
// 

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

struct Radar_point
{
	float			radian;
	signed int 	distance;
};

struct Radar_data
{
	float				start_r;
	float 				end_r;
	unsigned short		start_d;
	unsigned short		end_d;
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
	void				(*init)();
	void				(*update)();
	void				(*send_qt)();
	void				(*send_bm)();
	void				(*send_gs)();
	unsigned short		counter;
	unsigned char		status;
	float				radian;
	float				x;
	float				y;
	float				len;
	struct Point		pre_point;
	float				pre_radian;
	unsigned char		direction;
};


#define RADAR_STRUCT_H
#endif
