#ifndef __rng_h
#define __rng_h
#include "stm32f4xx.h"

void RNG_Init(void);
void GetSrandSeed(void);
u16 speed_test_get_rval(u16 min,u16 max);
void speed_test_get_fill_parameter(u16 *x,u16 *y,u16 *width,u16 *height,u16 *color);



#endif
