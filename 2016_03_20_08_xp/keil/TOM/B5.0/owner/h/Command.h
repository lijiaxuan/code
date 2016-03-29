#ifndef _COMMAND_H
#define _COMMAND_H
	#include "stm32f10x.h"
	
	
	void R_CAMER_OFFSET_FUNC(void);//接收位置偏差信息 mm 右偏为正
	void R_CAMER_ANGLE_FUNC(void);//接收角度偏差信息	 度 右偏为正
	
	
    void W_PWM_FUNC(void);
	
	
#endif
