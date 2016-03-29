#ifndef _MYMATH_H
#define _MYMATH_H
 
#define Abs(a)   ((a)>0?(a):-(a))
 
#include "stm32f10x.h"
#include "MyTypeDef.h"

int32_t Int32Abs(int32_t Data);
int64_t Int64Abs(int64_t Data);
float F_Abs(float Data);

float GetLength(Pointfp64 a,Pointfp64  b);
float GetDis_P2L(Pointfp64  p,float a,float b,float c);
float GetDis_P2L_PK( Pointfp64 p, Pointfp64 p0,double k);
int circle_circle_intersection(double x0, double y0, double r0,
                               double x1, double y1, double r1,
                               double *xi, double *yi,
                               double *xi_prime, double *yi_prime);
#endif
