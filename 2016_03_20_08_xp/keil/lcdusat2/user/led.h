#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define ON  0
#define OFF 1

//���κ꣬��������������һ��ʹ��
#define LED1(a)	if (a)	\
					GPIO_ResetBits(GPIOB,GPIO_Pin_10);\
					else		\
					GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define LED2(a)	if (a)	\
					GPIO_ResetBits(GPIOB,GPIO_Pin_11);\
					else		\
					GPIO_SetBits(GPIOB,GPIO_Pin_11)


void LED_GPIO_Config(void);
void LED_Init(void);
#endif /* __LED_H */
