#ifndef ENCODER_C
#define ENCODER_C

#include "encoder.h"


struct Encoder_parameter Encoders_par[2]=
{	
	{512*4,0.0786225332,-1},
 	{512*4,0.0796072707, 1},
};

struct Encoder_data  Encoders[2]=
{
	{0,0,0,0},
	{0,0,0,0},
};

int32_t GET_ENCODER(int8_t i)
{
	int32_t temp;
	temp = (int32_t)(((int32_t)(r_now[i]))*Encoders_par[i].count_per_rotation+(int32_t)(Tim_S[i]->CNT));
	return temp;
}

void Encoder_Update(void)
{
	char i;
	int32_t now;
	int32_t step;
	
	for(i=0;i<2;i++)
	{
		now = GET_ENCODER(i);
		step=now-Encoders[i].last_count;
		if(step<2000&&step>-2000)
		{
				Encoders[i].last_count=now;
				Encoders[i].total_count+=step*Encoders_par[i].direction;
				Encoders[i].distance+=step*Encoders_par[i].convert*Encoders_par[i].direction;	  
	    }
	}
}

void Encoder_Clear(int index)
{
	Encoders[index].distance=0.0;
	Encoders[index].last_count=GET_ENCODER(index);
	Encoders[index].total_count=0;
}

#endif 
