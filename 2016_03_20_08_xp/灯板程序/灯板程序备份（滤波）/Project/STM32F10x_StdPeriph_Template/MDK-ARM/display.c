#include "stm32f10x.h"
#include "all.h"

int nn;
int mm;
u8 data00,data01,data10,data11;
void delay(unsigned long t)
		{
				for(;t>1;t--)
					{
					}
		}

void dispint(void)
		{
			 GPIO_InitTypeDef GPIO_InitStructure;		
			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	
				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
				GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(GPIOB, &GPIO_InitStructure);

				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
				GPIO_InitStructure.GPIO_Speed= GPIO_Speed_2MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(GPIOA, &GPIO_InitStructure);
				
				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		}

void Display(uint32_t dis)
		{
			data00=(u8)(dis>>24);
			data01=(u8)(dis>>16);
			data10=(u8)(dis>>8);
			data11=(u8)dis; 
			for(j=0;j<8;j++)
					{
							 GPIO_ResetBits(GPIOA,GPIO_Pin_1);//移位时钟 st0
							 GPIOA->ODR &= ~(0x30);	//数据输出寄存器
							 GPIOB->ODR &= ~(0x300);	
							 GPIOA->ODR |= (((~(data11)&0x80)>>3)|((~(data10)&0x80)>>2));
							 GPIOB->ODR |= (((~(data01)&0x80)<<1)|((~(data00)&0x80)<<2));
							 delay(30);  
							 GPIO_SetBits(GPIOA,GPIO_Pin_1);
							 delay(30);                        //数据处理后作为3个控制LED亮的芯片的输入输入，ds01 10 11
							 data00=data00<<1;
							 data01=data01<<1;
							 data10=data10<<1;
							 data11=data11<<1;
					} 
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);//锁相时钟 sh0
			delay(15);
			GPIO_SetBits(GPIOA,GPIO_Pin_0);

		}






 
