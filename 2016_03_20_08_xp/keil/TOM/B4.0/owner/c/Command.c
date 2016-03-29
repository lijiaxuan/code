#include "all.h"

double pos_error;
double ang_error;
double pwm_duty_1=0;

uint8_t SEND_CONVER_FLAG=0;

void R_CAMER_OFFSET_FUNC(void)//接收位置偏差信息 mm 右偏为正
{
	pos_error =	MISO_CAMER_OFFSET.f64_data;
}

void R_CAMER_ANGLE_FUNC(void)//接收角度偏差信息	 度 右偏为正
{
	ang_error =	MISO_CAMER_ANGLE.f64_data;
}

void W_PWM_FUNC(void)
{
// 	PWM1_SetDuty(MOSI_PWM.u16_data[0]);
// 	PWM2_SetDuty(MOSI_PWM.u16_data[1]);

    pwm_duty_1=MOSI_PWM.u16_data[0];
    pwm_duty_1=MOSI_PWM.u16_data[1];
    
    MISO_CMD=8;
    //Can_Database[HASH_TABLE[G_M_CMD_ID]].Data_ptr=0;
    Write_Database(G_M_CMD_ID);
}

//先看GPS精度怎么样 校正是在出现意外时才使用的方式
//调PID 改路径 提高控制精度
//控制精度是可以提高的
//我们的控制精度为什么上不去呢  控制周期已经很小了 也出现了震荡

//校正有红外测距 接近开关  颜色传感器
//   ADC           IO       TIM

//可以整合到一块板子上
//一块板子负责红外测距； 检测到合适的位置后，通知主控，从主控获得GPS数据
//马上计算 得到校正后的GPS数据 马上传送给主控更新GPS数据 速度足够快
//这之间两块板子之间要通信  用CAN

void GPS_REVEISE(void)
{
    if(SA1_OFF) 
        return;
    else
        return;
    //GPS_Revise(Pointfp64 position);
}
