/*
* @Author: obsidian
* @Date:   2016-03-05 15:51:29
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-17 20:05:24
*/

#include "numstr.h"
#include "driver/math/inc/math.h"
#include "lcd_struct.h"
#include "lcd_global.h"

void num_init(void);
void num_input(char key);
void num_show(unsigned char pos);
void int2str(void);
void float2str_6(void);
void float2str_3(void);
void str2float(void);
void str2int(void);

struct Num num = 
{
    {0},
    0,
    0,
    -1,
    0,
    num_init,
    num_input,
    num_show,
    int2str,
    float2str_3,
    float2str_6,
    str2int,
    str2float,
};


void num_init()
{
    zero(num.numstr, 10);
    num.dotpos = -1;
    num.index = 0;
}
void num_input(char key)
{
    if(num.index > 10) {return;}

    if(key == 0xb && lcd.p_status->p_item->is_int == 0)
    {

        if(num.dotpos >= 0){return;}
        num.dotpos = num.index;
        num.numstr[num.index] = '.';
				num.index ++;
    }
    else if(key == 0xa)
    {
        if(num.index > 0){return;}
        num.numstr[0] = '-';
        num.index ++;
				return;
    }
    else if(key < 10)
    {
        num.numstr[num.index] = key + '0';
			  num.index ++;
    }
    
}
void num_show(unsigned char pos)
{
    // LCD_ShowString_BIG(140 + 220 * (pos - 1) * 2 / PAGE_SIZE , 70 + LINE_DIS * (((pos - 1)) % (PAGE_SIZE / 2)), "         ", WHITE,1);
    // LCD_ShowString_BIG(140 + 220 * (pos - 1) * 2 / PAGE_SIZE , 70 + LINE_DIS * (((pos - 1)) % (PAGE_SIZE / 2)), num.numstr, WHITE,1);
    LCD_ShowString(6, 7 , "         ", WHITE,1);
    LCD_ShowString(6, 7 , num.numstr, WHITE,1);


}

void int2str()
{
    unsigned char k = 0;
    unsigned char i = 0;
    num.init();
    unsigned int tmp = num.intvalue;
    for(k = 9;tmp != 0; tmp = tmp / 10,k --)
    {num.numstr[k] = tmp % 10 + '0';num.index ++;}
    for(i = 0;i < 10;i ++)
    {
        num.numstr[i] = num.numstr[i + k + 1];
        if(i + k > 8)
        {num.numstr[i] = ' ';}
    }
    num.numstr[i] = 0;
    if(num.intvalue == 0)
    {
        num.numstr[0] = '0';
    }
}

void float2str_3()
{
    signed char k = 0;
    signed char find_nonzero = 0;
    unsigned char neg = 0;
	float tmp_1 = num.floatvalue;
    if(tmp_1 == 0)
		{
			num.numstr[0] = '0';
			num.numstr[1] = '.';
			num.numstr[2] = '0';
			num.numstr[3] = ' ';
            num.numstr[4] = ' ';
            num.numstr[5] = ' ';
			return;
		}
    if(tmp_1 < 0)
    {
        tmp_1 = - tmp_1;
        neg = 1;
    }
    num.init();
    float tmp_2 = tmp_1 * 1000;
    num.intvalue = tmp_2;

    if(tmp_1 >= 1)
    {
        num.int2str();
        for(k = num.index; k > num.index - 3;k --)
        {
            if(num.numstr[k - 1] != '0'){num.numstr[k] = ' ';find_nonzero = 1;}
            if(find_nonzero == 0){num.numstr[k] = ' ';continue;}
            num.numstr[k] = num.numstr[k - 1];
        }
        num.numstr[k] = '.';
		if(find_nonzero == 0)
        {
            num.numstr[k + 1] = '0';
            num.numstr[k + 2] = ' ';
            num.numstr[k + 3] = ' ';
        }
    }

    else
    {
        signed char k = 0;
        signed int tmp = tmp_1 * 1000;
        for(k = 2;k >= 0; tmp = tmp / 10,k --)
        {num.numstr[k] = tmp % 10 + '0';}
        for(k = 3; k > 0;k --)
        {
            if(num.numstr[k - 1] != '0'){num.numstr[k + 1] = ' ';find_nonzero = 1;}
            if(find_nonzero == 0){ num.numstr[k + 1] = ' ';continue;}
            num.numstr[k + 1] = num.numstr[k - 1];
        }      
        num.numstr[0] = '0';
        num.numstr[5] = ' ';
        num.numstr[6] = ' ';
        num.numstr[k + 1] = '.';         
    }
    if(neg == 1)
    {
        for(k = 10;k >= 0;k --)
        {
            num.numstr[k + 1] = num.numstr[k];
        }
        num.numstr[0] = '-';
    }
}
void float2str_6()
{
    signed char k = 0;
    signed char find_nonzero = 0;
    char neg = 0;
    num.init();
    float tmp_1 = num.floatvalue;

	   if(tmp_1 == 0)
		{
			num.numstr[0] = '0';
			num.numstr[1] = '.';
			num.numstr[2] = '0';
			num.numstr[3] = 0;
			return;
		}
    if(tmp_1 < 0)
    {
        tmp_1 = - tmp_1;
        neg = 1;
    }
		float tmp_2 = tmp_1 * 1000000;
    num.intvalue = tmp_2;
    if(tmp_1 > 1)
    {
        num.int2str();
        for(k = num.index; k > num.index - 6;k --)
        {
            if(num.numstr[k - 1] != '0'){find_nonzero = 1;}
            if(find_nonzero == 0){ num.numstr[k] =' ';continue;}
            num.numstr[k] = num.numstr[k - 1];
        }
        num.numstr[k] = '.';
    }
    else
    {
        signed char k = 0;
        signed int tmp = tmp_1 * 1000000;
        for(k = 5;k >= 0; tmp = tmp / 10,k --)
        {num.numstr[k] = tmp % 10 + '0';}
        for(k = 6; k > 0;k --)
        {
            if(num.numstr[k - 1] != '0'){find_nonzero = 1;}
            if(find_nonzero == 0){ num.numstr[k + 1] = ' '; continue;}
            num.numstr[k + 1] = num.numstr[k - 1];
        }      
        num.numstr[0] = '0';
        num.numstr[k + 1] = '.';         
    }
    if(neg == 1)
    {
        for(k = 10;k >= 0;k --)
        {
            num.numstr[k + 1] = num.numstr[k];
        }
        num.numstr[0] = '-';
    }
}
void str2int()
{
    signed char k = 0;
		num.intvalue = 0;
    for(k = 0;k < num.index;k ++)
    {   
        if(num.numstr[k] == '.'){continue;}
        if(num.numstr[k] == '-'){continue;}
        num.intvalue *= 10;
        num.intvalue += num.numstr[k] - '0';
    }
    num.numstr[num.index] = 0;
}

void str2float()
{
    signed char k = 0;
    float s = 1;
	  num.floatvalue = 0;
	  if(num.dotpos > 0)
	  {
      for(k = 0;k < num.index - num.dotpos - 1; k++)
      {s = s * 10;}
	  }
		num.str2int();
    num.floatvalue = num.intvalue / (s);
    if(num.numstr[0] == '-'){num.floatvalue = - num.floatvalue;}
}
