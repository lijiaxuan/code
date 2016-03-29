#ifndef __USART_H
#define __USART_H

#include "stm32f4xx_conf.h"

extern void COM1Init(u32 BaudRate);
extern void COM2Init(u32 BaudRate);
extern void COM3Init(u32 BaudRate);
extern void COM4Init(u32 BaudRate);
extern void COM5Init(u32 BaudRate);
extern void COM6Init(u32 BaudRate);

extern void com2_send_color(u16 data);
extern void com6_send_color(u16 data);
#endif

