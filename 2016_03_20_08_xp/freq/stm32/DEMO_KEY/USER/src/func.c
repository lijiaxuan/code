/* 
* @Author: obsidian
* @Date:   2016-01-20 14:42:43
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-21 09:42:36
*/
#include "stm32f10x_dac.h"
#include "ch452.h"
#include "../inc/func.h"
char data[10] = {0,0,0,0,'.',0,0,0};

void fresh_normal();
struct UI ui = 
{
	0,0,
	{
		{{20,40},	{120,40	}, {200,40},	{"Hz","KHz","MHz"},	"freqency"},
		{{20,100},	{120,100}, {200,100},  {"ns","us","ms"}, "delta_time"},
		{{20,160},	{120,160}, {200,160},	{"%"},"duty"}
	},
	{
		SELECT_COLOR,
		GET_COLOR
	}
};

char blank_data[26] = "                        ";

// 5b 5a 59 58
// 53 52 51 50
// 4b 4a 49 48
// 43 42 41 40

void key_process(void)
{
	key_v=CH452_GetKey();
	switch(key_v)
	{
		case	KEY_UP:

			switch(ui.state)
			{
				case NO_ACTION:
				break;
				case SELECT:
					if(ui.selected == 0)
					{
						ui.selected = 2;
					}
					else
					{
						ui.selected = (ui.selected - 1) % 3;
					}
				break;
				case GET:
				break;
			}
			fresh_screen();
			break;
		case	KEY_DOWN:
			switch(ui.state)
			{
				case NO_ACTION:
				break;
				case SELECT:
					ui.selected = (ui.selected + 1) % 3;
				break;
				case GET:
				break;
			}
			fresh_screen();
			break;
		case	KEY_FOR:
			switch(ui.state)
			{
				case NO_ACTION:
					ui.state = SELECT;
					ui.selected = 0;
					break;
				case SELECT:
					ui.state = GET;
					// todo
					//
					//
					switch(ui.selected)
					{
						case 0:
							GPIOE->ODR |= MOD;
							DAC_SetChannel1Data(DAC_Align_12b_R,3400);

  							DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
							break;
						case 1:
							DAC_SetChannel1Data(DAC_Align_12b_R,2000);

  							DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
							break;
						case 2:
							DAC_SetChannel1Data(DAC_Align_12b_R,2000);

  							DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
							break;
					}
					break;
				case GET:
					break;
			}
			fresh_screen();
			break;
		case	KEY_BACK:
			switch(ui.state)
			{
				case NO_ACTION:
				break;
				case SELECT:
					ui.state = NO_ACTION;
					ui.selected = 0;
					GPIOE->ODR &= ~MOD;
				break;
				case GET:
					ui.state = SELECT;
					ui.selected = 0;
				break;
			}
			fresh_screen();
			break;		
		default:
			break;	
	}
	// num2str(100.30);
	// pen->fcolor = NORMAL_COLOR;
	// pen->bcolor = VGA_LGRAY;
	// pen->dcolor = VGA_LGRAY;
	// pen->PutString(50,80,(unsigned char *)data);
}

void fresh_screen(void)
{
	fresh_normal();
	switch(ui.state)
	{
		case NO_ACTION:
		break;
		case SELECT:
			pen->fcolor = SELECT_COLOR;
			pen->PutString(ui.widget[ui.selected].point[0],ui.widget[ui.selected].point[1],(unsigned char *)ui.widget[ui.selected].text);
		break;
		case GET:
			pen->fcolor = GET_COLOR;
			pen->PutString(ui.widget[ui.selected].point[0],ui.widget[ui.selected].point[1],(unsigned char *)ui.widget[ui.selected].text);
		break;
	}
}
void fresh_normal()
{
	unsigned char i = 0;

  	pen->font_height=EZ_FONT_HEIGHT_16;
  	pen->font_style=0;
	pen->fcolor = VGA_LGRAY;
  	pen->font_type=EZ_FONT_TYPE_ASCII_ARIAL;
  	pen->FillRect(0,0,400,240);

	pen->fcolor = NORMAL_COLOR;
	pen->bcolor = VGA_LGRAY;
	pen->dcolor = VGA_LGRAY;
	for(i = 0;i < 3 ;i ++)
	{
		pen->PutString(ui.widget[i].point[0],ui.widget[i].point[1],(unsigned char *)ui.widget[i].text);	
	}

}

//
//0 - 1000
//  9 0 1 . 9 0
//0 1 2 3 4 5 6 
void num2str(float num)
{
	signed char i = 0;
	unsigned short zheng = (unsigned short) num;
	float xiao = num - zheng;
	unsigned short k = zheng;
	float tmp = xiao;
	for(i = 3; i >= 0; i --)
	{
		if(k == 0)
		{
			data[i] = ' ';
			continue;
		}
		data[i] = k % 10 + '0';
		k = k / 10;
	}
	for(i = 5; i < 9; i ++)
	{
		if(ui.selected == 2 && i > 6)
		{
			data[i] = ' ';
			continue;
		}
		xiao = xiao * 10 - (unsigned short)xiao * 10;
		data[i] = (unsigned char)xiao + '0';
	}
}