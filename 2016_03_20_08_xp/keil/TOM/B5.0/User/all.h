#ifndef _ALL_H
#define _ALL_H


//	#define UART_DEBUG
    #define LCD_DEBUG

	#define  FIRST	//不同的小车 要改 码盘转换系数 陀螺仪转换系数 舵机中值和转换系数 必要时要改PID 暂定不改路径
//	#define  SECOND


	#include "stm32f10x.h"
	#include "math.h"
	
	#define fp64 double
	#define fp32  float
	#define PI	3.14159265359	
	
	#include "MyTypeDef.h"
	#include "init.h"
    #include "update.h"
	
	//#include "can.h"
    #include "canljx.h"
	#include "qei.h"
	#include "usart.h"
	#include "gyro.h"
	#include "SysTick.h"
	#include "gps.h"
	#include "encoder.h"
	#include "measure.h"

	#include "mymath.h"
	//#include "can_database.h"
	//#include "Command.h"
	#include "flash.h"
	#include "pwm.h"

	#include "pointpath.h"
    #include "brake.h"
    #include "sa.h"
	
	#include "lcd.h"
	#include "key.h"
	#include "pid.h"
	
	#include "TypeDef.h"
	#include "func.h"
	#include "list.h"
	
	#include "M_GPS.h"
	#include "M_PID.h"
	#include "M_GO.h"
	#include "M_Gyro.h"
	#include "M_Encoder.h"
	#include "M_Colour.h"
	#include "M_Engine.h"
    
	
    #include "MMC_SD.h"
	#include "integer.h"
	#include "diskio.h"
	#include "ff.h"
	#include "spi.h"
	#include "MyFatFs.h"
	#include "sys.h"
	#include "stdarg.h"
	#include "string.h"
	#include "stdlib.h"
    #include "sd_serve.h"
	
#endif
