#include "stm32f10x.h"
#include "steering.h"
#include "pwm.h"
#include "delay.h"
#include "usart.h"
#include "math.h"
#include "stm32f10x_it.h"
int main(void)
 {	
		int fandelaytime;
		int fanduojipwm=90;
		int  qianlunduojipwmhandle;
		int  qianlunduojipwmreal=90;
		char flag=0;				//判断延时时间的标志位
		SystemInit ();
		delay_init ();	
		TIM2_GPIO_Config();
		TIM2_Mode_Config(90,90 );
		uart_init (115200);
		delay_ms (20);
// 		TIM2_Mode_Config(90,90);
	
// 		uart_init(9600);
// 		trans_gps (70,150);
	 

			 
	 
		 while(1)
		 {

			 //下面这一部分是用来控制帆的  			 			 
			   fandelaytime=10*fabs(trans_fan.rec_data-0x3000)/0x0800+5;//通过改变前面的系数来控制舵机变化速度的大小
			   fandelaytime=20-fandelaytime;
			   if(trans_fan.rec_data>(0x3000+0x400))
				 {
						flag =1;
						fanduojipwm++;
						set_anglefan(fanduojipwm);
							
				 }
				 if(trans_fan.rec_data<(0x3000-0x400))
				 {
						flag =1;
						fanduojipwm--;
						set_anglefan(fanduojipwm);
				 }
				 
// 				 //下面这一部分是用来控制前轮舵机的
// 			
// 				 qianlunduojipwmhandle=90+90*(trans_lunzi.rec_data-0x3000)/0x0800;
// 				  USART_SendData(USART3,0x01);
// 				 		 delay_ms(10); 
// 				 USART_SendData(USART3,trans_lunzi.temp [1]);
// 				 delay_ms(10); 
// 				 USART_SendData(USART3,trans_lunzi.temp [0]);
				 
				 
				  //下面这一部分是用来控制前轮的  			
				   if(trans_lunzi.rec_data-0x3000>0x40)
					{
					 qianlunduojipwmhandle=90+90*(trans_lunzi.rec_data-0x3040)/0x1000;
					}
						else if(trans_lunzi.rec_data-0x3000<-0x40)
						{
							qianlunduojipwmhandle=90+90*(trans_lunzi.rec_data-0x3000+0x40)/0x1000;
						}
							else
							{
									qianlunduojipwmhandle=90;
							} 
					if(qianlunduojipwmreal<qianlunduojipwmhandle)
				 {
						 qianlunduojipwmreal++;
						set_anglelunzi(qianlunduojipwmreal);
					
				
				 }
				 if(qianlunduojipwmreal>qianlunduojipwmhandle)
				 {
						qianlunduojipwmreal--;
						set_anglelunzi(qianlunduojipwmreal);	
				 }
				 if(flag==1)//判断帆是否转动
				 {
						delay_ms(fandelaytime);
					}
					else
					{
						delay_ms(10);
					}
					flag=0;
// 				  USART_SendData(USART3,0x01);
// 				 		 delay_ms(10); 
// // 				 USART_SendData(USART3,trans_lunzi.temp [1]);
// 					USART_SendData(USART3,qianlunduojipwmhandle);
// 				 delay_ms(10); 
// 					
// // 				 USART_SendData(USART3,trans_lunzi.temp [0]);
// 					
// 					 USART_SendData(USART3,0x02);
// 				 		 delay_ms(10); 
// // 				 USART_SendData(USART3,trans_lunzi.temp [1]);
// 					USART_SendData(USART3,trans_lunzi.temp [1]);
// 					 delay_ms(10);
// 					USART_SendData(USART3,trans_lunzi.temp [0]);
					
					

					 
				
// 					 USART_SendData(USART3,trans_lunzi.rec_data);   						 							 
			 }			 
				 
}				 

void set_anglelunzi(double angle1)
{
		if(angle1 >0&&angle1 <180)
					 {
					   TIM2 ->CCR4=200+800*angle1/180;				//PWM3
					   
					 }
}
void set_anglefan(double angle2)
{
		if(angle2 >0&&angle2 <180)
					 {
					   TIM2 ->CCR3=200+800*angle2/180;				//PWM4
					  
					 }
}