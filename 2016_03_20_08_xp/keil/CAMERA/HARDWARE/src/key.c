#include "delay.h"
#include "key.h"


void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9|GPIO_Pin_10;//Pa0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //输入1
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
}
//0 没连按
u8 KEY_Scan(u8 mode)
{
	static u8 key_up=1;
	if(mode) key_up=1;
	if(key_up&&KEY_OUT_UP==1||KEY_OUT_FALL==1||KEY_OUT_LEFT==1||KEY_OUT_RIGHT==1||KEY_OUT_CONFIRM==1||KEY_OUT_CANCEL==1)
	{
		delay_ms(3);
		key_up=0;
		if(KEY_OUT_UP==1) 					return KEY_UP;
		else if(KEY_OUT_FALL==1)		return KEY_FALL;
		else if(KEY_OUT_LEFT==1)		return KEY_LEFT;
		else if(KEY_OUT_RIGHT==1)		return KEY_RIGHT;
		else if(KEY_OUT_CONFIRM==1)	return KEY_CONFIRM;
		else if(KEY_OUT_CANCEL==1)	return KEY_CANCEL;
	}else if(KEY_OUT_UP==0||KEY_OUT_FALL==0||KEY_OUT_LEFT==0||KEY_OUT_RIGHT==0||KEY_OUT_CONFIRM==0||KEY_OUT_CANCEL==0) key_up=1;
	return 0;
}
