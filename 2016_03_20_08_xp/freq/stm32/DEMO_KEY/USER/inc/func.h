/* 
* @Author: obsidian
* @Date:   2016-01-20 14:42:37
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-21 09:49:12
*/

#ifndef FUNC_H
#include "EZ_GUI.H"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#define 	NO_ACTION	0
#define 	SELECT		1
#define 	GET			2

#define 	KEY_UP		0x5a
#define 	KEY_DOWN	0x4a
#define 	KEY_FOR		0x52
#define 	KEY_BACK	0x4b

#define 	NORMAL_COLOR	VGA_WHITE	
#define 	GET_COLOR		VGA_BLUE
#define 	SELECT_COLOR	VGA_RED

#define 	RST			(1 << 11)
#define 	CS      	(1 << 12)
#define 	MOD     	(1 << 8)
#define 	SPICLK  	(1 << 14)
#define 	DATA    	(1 << 15)
#define     BPF			(1 << 7)


extern 	unsigned char 	key_v;
extern 	unsigned char 	status;
extern 	unsigned int 	num_per_T;
extern 	unsigned int 	num_per_duty;
extern 	unsigned int	Ts;

struct Widget
{
	unsigned short	point[2];
	unsigned short  data_point[2];
	unsigned short	unit_point[2];
	unsigned char*	unit[4]; 	
	char*			text;
};

struct UI
{
	signed char		state;
	signed char		selected;
	struct Widget  	widget[3];
	unsigned int	color[2];
};	


void key_process(void);
void fresh_screen(void);
void num2str(float num);
extern char blank_data[26];
extern char data[10];
extern EPEN *pen;
extern struct UI ui;
#define FUNC_H
#endif