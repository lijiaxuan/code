#include "rng.h"
#include "stdlib.h"
#include "ILI9481.h"

volatile uint32_t seed;

const u16 SPEED_COLOR_TBL[10]={RED,GREEN,BLUE,BROWN,GRED,BRRED,CYAN,YELLOW,GRAY,MAGENTA};
//得到速度测试一次填充的相关信息
//*x,*y,*width,*height,*color:获取到的填充坐标/尺寸/颜色等信息 
void speed_test_get_fill_parameter(u16 *x,u16 *y,u16 *width,u16 *height,u16 *color)
{
	*x=speed_test_get_rval(0,700);
	*y=speed_test_get_rval(0,380);
	*width=speed_test_get_rval(100,800-*x);
	*height=speed_test_get_rval(100,480-*y);
	*color=SPEED_COLOR_TBL[speed_test_get_rval(0,9)]; 
}   


void RNG_Init(void)
{
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG,ENABLE);
	RNG_Cmd(ENABLE);
	GetSrandSeed();//第一次读取的随机数不可用
}

//============================================================
//硬件随机数提取函数
void GetSrandSeed(void)
{
	while(!RNG_GetFlagStatus(RNG_FLAG_DRDY));

	seed = RNG_GetRandomNumber();
	RNG_ClearFlag(RNG_FLAG_DRDY);
	
	srand(seed);
}

//得到一个随机数
//(min,max)期望的随机数范围
//返回值:符合期望的随机数值
u16 speed_test_get_rval(u16 min,u16 max)
{
	u16 t=0Xffff;
	while((t<min)||(t>max)) {GetSrandSeed(); t=seed;}	  
	return t;
}												 
//============================================================

