#ifndef GYRO_C
#define GYRO_C

#include "gyro.h"
double  Gyro_Angle_Total=0;
double  Gyro_Total=0;
// double Gyro_Convert1=1.45820468;    //第三版车正转系数
// double Gyro_Convert2=1.45807606;    //第三版车反转系数

double Gyro_Convert1=1.4567945;    //第三版车正转系数
double Gyro_Convert2=1.4565394;    //第三版车反转系数
struct Gyro_Struct  Gyro_Now={0,0};


int64_t Gyro_Float_Total1;
int64_t Gyro_Float_Total2;
uint8_t Gyro_Float_Flag1=0;
uint8_t Gyro_Float_Flag2=0;
uint8_t Gyro_Float_Flag=0;

extern volatile uint8_t USART3_TxBuffer[TxBufferSize];
extern volatile uint8_t USART1_RxBuffer[RxBufferSize];

int64_t Int64Abs(int64_t Data)
{
	if(Data<0)
		return -Data;
	else 
		return Data;
}

void Gyro_Update(void);
void DMA1_Channel5_IRQHandler(void)
{
	static union u8to32
	{
		int32_t value;
		uint8_t datas[4];
	}gyro;
    
	if(DMA_GetITStatus(DMA1_IT_GL5)==SET)
	{
		gyro.datas[3] = USART1_RxBuffer[1];
		gyro.datas[2] = USART1_RxBuffer[2];
		gyro.datas[1] = USART1_RxBuffer[0];
		gyro.datas[0] = 0;
		
		DMA_ClearITPendingBit(DMA1_IT_GL5);
		gyro.value >>=8;
		Gyro_Now.value= gyro.value;
		Gyro_Now.count++;
		        
		Gyro_Update();	//陀螺仪角度信息只在这一处更新
		
		
		if(Gyro_Now.count==256&&Gyro_Float_Flag1)
		{
			Gyro_Float_Total1=Gyro_Total;
			Gyro_Float_Flag1=0;
		}
		if(Gyro_Now.count==20736 &&Gyro_Float_Flag2)//2O736=256+20480
		{
      Gyro_Float_Total2=Gyro_Total;
			Gyro_Float_Flag2=0;
			Gyro_Float_Flag=0;
		}
	}
}


//int32_t Gyro_Float=0;
int32_t Gyro_Float=0xFFFFFE35;
void Gyro_Update(void)
{	
	Gyro_Total+=Gyro_Now.value;//写这个变量只是校正零漂的时候用
	if(Gyro_Now.value-Gyro_Float<0)
		Gyro_Angle_Total=Gyro_Angle_Total+(Gyro_Now.value-Gyro_Float)*0.0000001*Gyro_Convert2;
	if(Gyro_Now.value-Gyro_Float>=0)
		Gyro_Angle_Total=Gyro_Angle_Total+(Gyro_Now.value-Gyro_Float)*0.0000001*Gyro_Convert1;
	
	
	Gyro_Angle_Total=Gyro_Angle_Total-(int)(Gyro_Angle_Total/360)*360;//强制类型转换 把角度范围转化到0到360度
	
 	if(Gyro_Angle_Total>180.0)
 		Gyro_Angle_Total-=360.0;
 	if(Gyro_Angle_Total<-180.0)
 		Gyro_Angle_Total+=360.0;
 }




void Gyro_CheckFloat(void)
{
	LED2_on;
	Gyro_Clear();
	Gyro_Float_Flag=1;
	Gyro_Float_Flag1=1;
	Gyro_Float_Flag2=1;
	while(Gyro_Float_Flag)
		delay(1);
	if(Gyro_Float_Total2-Gyro_Float_Total1==0)
	{
		Gyro_Angle_Total=0;
	}
	
//正常校零漂  remeber to change the number in Gyro_Update();
// 	else if(Int64Abs(Gyro_Float_Total2-Gyro_Float_Total1-Gyro_Float*2048)>81920)
// 	{
// 		Gyro_Float=(Gyro_Float_Total2-Gyro_Float_Total1)/2048;
// 	}
	
	
	//用一个特别长时间的校零漂  得到一个准确的值 每次都用同一个值  
	else if(Int64Abs(Gyro_Float_Total2-Gyro_Float_Total1-Gyro_Float*20480)>819200)
	{
		Gyro_Float=(Gyro_Float_Total2-Gyro_Float_Total1)/20480;
	}
	
	
	else
	{
		Gyro_Angle_Total=0;
	}
	
	Gyro_Clear();
	LED2_off;	
}

void Gyro_Clear(void)
{
	Gyro_Total=0;
	Gyro_Angle_Total=0;
	Gyro_Now.count=0;
}

#endif
