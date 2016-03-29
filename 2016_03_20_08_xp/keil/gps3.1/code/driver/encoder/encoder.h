#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"


struct Encoder_parameter
{
	unsigned short 				count_per_rotation;
  	float          				convert;
	signed char    				direction;
};

struct  Encoder_data
{
	float 						distance;
	float 						delta_distance;
	signed int 					last_count;
	signed int     				total_count;
};

extern int16_t r_now[2];
extern TIM_TypeDef* Tim_S[2];
extern struct Encoder_parameter Encoders_par[2];
extern struct Encoder_data  Encoders[2];

void Encoder_Update(void);

void Encoder_Clear(int index);

void QEI_Init(void);

#endif 
