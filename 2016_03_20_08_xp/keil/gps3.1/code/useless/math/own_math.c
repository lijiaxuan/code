#ifndef MATH_C
#define MATH_C
	
#include "math.h"



int32_t Int32Abs(int32_t Data)
{
	if(Data<0)
		return -Data;
	else 
		return Data;
}
int64_t Int64Abs(int64_t Data)
{
	if(Data<0)
		return -Data;
	else 
		return Data;
}

float F_Abs(float Data)
{
	if(Data<0)
		return -Data;
	else 
		return Data;
}
#endif
