
#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include "system_stm32f10x.h"
#include "gps.h"
#include "lcd.h"
#include "engineConfig.h"
#include "brake.h"


#define ANGLE_CONVERT_pwm1  3.6
#define ANGLE_CONVERT_pwm2  3.6


extern int mid_value_pwm1;
extern int mid_value_pwm2;

extern float engine_angle1;
extern float engine_angle2;

void SET_ANGLE(double angle,int pwm_port);

void TIM_PWM_Init(void);

#endif
