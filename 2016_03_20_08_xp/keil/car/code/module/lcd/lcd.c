/*
* @Author: obsidian
* @Date:   2016-03-01 09:19:32
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-16 20:31:22
*/

#include "lcd.h"
#include "lcd_driver.h"
#include "lcd_global.h"
#include "driver/math/inc/math.h"
#include "numstr.h"
#include "menu/menu_pid.h"
#include "menu/menu_steer.h"
#include "menu/menu_brake.h"
#include "menu/menu_gps.h"
#include "menu/menu_gpspath.h"
#include "control/car.h"
#include "control/store/store_global.h"
void show_menu(void);

void save(void);
void send(void){}

void lcd_init()
{
	//lcd.p_status->p_current = &pid_n; //
    UART3_init(115200);
    lcd_timer_cfg();
	root_n.init(0);
    LCD_Clear();
    show_menu();
    pid_n.init(0);
	steer_n.init(0);
	brake_n.init(0);
	gps_n.init(0);
    gpspath_n.init(0);
//     refresh();
// 		LCD_Clear();
//     LCD_WriteStringXY("lijiaxuan",0,4);
}

void root_init(void *parg)
{
    char i = 0;
    for(i = 1;i < 7; i ++)
    {
        root_n.child[i]->node_id = i;
    }
}

void LCD_Clear(void)
{ 
	  unsigned char carid[3] = "0";
      TFT_Clear();
    //LCD_ShowString(352,0,  "|~~~~~~|",WHITE,1);
    // ili9481_DrawLine(300,10,300,310,WHITE,2);
    // ili9481_DrawLine(450,10,450,310,WHITE,2);
    // ili9481_DrawLine(300,10,450,10,WHITE,2);
    // ili9481_DrawLine(300,310,450,310,WHITE,2);
    // ili9481_DrawLine(300,160,450,160,WHITE,1);
    // //ili9481_DrawLine(300,210,450,210,WHITE,1);
    // ili9481_DrawLine(375,10,375,310,WHITE,1);
    LCD_ShowString(12, 1, "For Car ", WHITE,1);
	  carid[0] = car.version + '0';
    LCD_ShowString(20, 1, carid, WHITE,1);
    //LCD_ShowChar_BIG(120,280,'S',12,WHITE,1);
}

void show_menu()
{
    struct Node *p_tmp = lcd.p_status->p_current;
    unsigned char index = 0;
    unsigned char tmpid = 0;
    LCD_Clear();
    for(index = 0; index < PAGE_SIZE && (index < p_tmp->itemnum - p_tmp->page_index * PAGE_SIZE); index ++)
    {
        num.intvalue = p_tmp->p_item[index].id;
        num.int2str();
        tmpid = (p_tmp->p_item[index].id - 1) % PAGE_SIZE;
        LCD_ShowString(1 + 18 * ((tmpid) * 2 / PAGE_SIZE),  3 + LINE_DIS * ((( tmpid)) % (PAGE_SIZE / 2)), num.numstr,WHITE,1);
        LCD_ShowString(3 + 18 * ((tmpid) * 2 / PAGE_SIZE) , 3 + LINE_DIS * (((tmpid)) % (PAGE_SIZE / 2)), ". ", WHITE,1);
        LCD_ShowString(4 + 18 * ((tmpid) * 2 / PAGE_SIZE) , 3 + LINE_DIS * (((tmpid)) % (PAGE_SIZE / 2)), p_tmp->p_item[index].ctx, WHITE,1);
        if(lcd.p_status->p_current->p_item[index].flag == FILE)
        {
            if(lcd.p_status->p_current->p_item[index].is_int == 0)
            {
                num.floatvalue = *(lcd.p_status->p_current->p_item[index].p_float);
                num.float2str_3();
                LCD_ShowString(5 + 18 * ((tmpid) * 2 / PAGE_SIZE) + lcd.p_status->p_current->p_item[index].ctx_len , 3 + LINE_DIS * (((tmpid)) % (PAGE_SIZE / 2)), num.numstr, WHITE,1);
                num.init();
            }
            else
            {
                num.intvalue = *(lcd.p_status->p_current->p_item[index].p_short);
                num.int2str();
                LCD_ShowString(5 + 18 * ((tmpid) * 2 / PAGE_SIZE) + lcd.p_status->p_current->p_item[index].ctx_len , 3 + LINE_DIS * (((tmpid)) % (PAGE_SIZE / 2)), num.numstr, WHITE,1);
                num.init();                
            }
        }
				delay_ms(5);
    }

}

void menu_show_run()
{
    struct Node *p_tmp = lcd.p_status->p_current;
    unsigned char index = 0;
    unsigned char tmpid = 0;
    for(index = 0; index < PAGE_SIZE && (index < p_tmp->itemnum - p_tmp->page_index * PAGE_SIZE); index ++)
    {
        num.intvalue = p_tmp->p_item[index].id;
        num.int2str();
        tmpid = (p_tmp->p_item[index].id - 1) % PAGE_SIZE;
        if(lcd.p_status->p_current->p_item[index].flag == FILE)
        {
            if(lcd.p_status->p_current->p_item[index].is_int == 0)
            {
                num.floatvalue = *(lcd.p_status->p_current->p_item[index].p_float);
                num.float2str_3();
                LCD_ShowString(5 + 18 * ((tmpid) * 2 / PAGE_SIZE) + lcd.p_status->p_current->p_item[index].ctx_len , 3 + LINE_DIS * (((tmpid)) % (PAGE_SIZE / 2)), num.numstr, WHITE,1);
                num.init();
            }
            else
            {
                num.intvalue = *(lcd.p_status->p_current->p_item[index].p_short);
                num.int2str();
                LCD_ShowString(5 + 18 * ((tmpid) * 2 / PAGE_SIZE) + lcd.p_status->p_current->p_item[index].ctx_len , 3 + LINE_DIS * (((tmpid)) % (PAGE_SIZE / 2)), num.numstr, WHITE,1);
                num.init();                
            }
        }
        delay_ms(5);
    }
}


void menu_gps_enter(void* parg)
{
    //update();
}
void menu_pid_enter(void* parg)
{
    //update();
}
void menu_steer_enter(void* parg)
{
    //update();
}
void menu_brake_enter(void* parg)
{
    //update();
}
void menu_run_enter(void* parg)
{
    TIM_Cmd(TIM6, ENABLE);
    lcd.p_status->is_run = 1;
    store.init();
    //sendstart();
}
void menu_run_back(void* parg)
{
    TIM_Cmd(TIM6, DISABLE);
    lcd.p_status->is_run = 0;
    store.close();
    //sendstart();
}


void input()
{
    //ili9481_fill(0, 0, 400,240,BLACK);
    TFT_Clear();
    if(lcd.p_status->p_item->flag == FILE)
    {
        if(lcd.p_status->p_item->is_int == 0)
        {
            num.floatvalue = *(lcd.p_status->p_item->p_float);
            num.float2str_6();
            LCD_ShowString(15 , 3 , num.numstr, WHITE,1);
            num.init();
        }
        else
        {
            num.intvalue = *(lcd.p_status->p_item->p_short);
            num.int2str();
            LCD_ShowString(15, 3, num.numstr, WHITE,1);
            num.init();                
        }
    }
    //ili9481_drawrect(30,40,400,230,WHITE,3);
    LCD_ShowString(6, 5, "change to: ", WHITE,1);
    lcd.p_status->is_input = 1;
}


void key_process(char key)
{
    if(lcd.p_status->is_input == 1)
    {
        if(key == 0x12)//enter
        {
            if(lcd.p_status->p_item->flag == FILE)
            {
                if(lcd.p_status->p_item->is_int == 0)
                {
                    num.str2float();
                    *(lcd.p_status->p_item->p_float) = num.floatvalue;
                    num.init();
                }
                else
                {
                    num.str2int();
                    *(lcd.p_status->p_item->p_short) = num.intvalue;
                    num.init();              
                }
            }
            lcd.p_status->p_item->func_input_callback(0);
			//lcd.p_status->p_current->save();
            lcd.p_status->is_input = 0;
			show_menu();
            num.init();
        }
        else if(key == 0x0c) //back
        {
            lcd.p_status->p_item->func_back(0);
            lcd.p_status->is_input = 0;
            num.init(); 
            show_menu();
        }
        else if((key >= 0 && key <= 9) || key == 0xa || key == 0x0b)
        {
            num.input(key);
            num.show(4);
        }
    }
    else 
    {
        if(key == 0x11){save();}
        if(key == 0x0c) //back
        {
//             if(lcd.p_status->p_current->node_id == 5)
//             {
//                 sendselect(0);
//             }
					unsigned char s = lcd.p_status->p_current->node_id;
			lcd.p_status->p_current = lcd.p_status->p_current->parent;
			//lcd.p_status->p_item->func_back(0);
            lcd.p_status->p_item = lcd.p_status->p_current->p_item;
			lcd.p_status->p_item[s - 1].func_back(0);
			show_menu();
        }
        if(key == 0xb) //->
        {
            if(lcd.p_status->p_current->page_index + 1 <= lcd.p_status->p_current->page_num)
            {
                lcd.p_status->p_current->page_index++;
                lcd.p_status->p_current->p_item += PAGE_SIZE;
                show_menu();
							  
            }
        }
        if(key == 0xa)//<-
        {
            if(lcd.p_status->p_current->page_index + 1 > 1)
            {
                lcd.p_status->p_current->page_index --;
                lcd.p_status->p_current->p_item -= PAGE_SIZE;
                show_menu();
							
            }
        }
        else if(key >= 1 && key < lcd.p_status->p_current->itemnum + 1 && (key - 1 < lcd.p_status->p_current->itemnum - lcd.p_status->p_current->page_index * PAGE_SIZE))
        {
            
            if(lcd.p_status->p_current->p_item[key - 1].flag == FOLDER)
            {
				lcd.p_status->p_current->p_item[key - 1].func_enter(0);
                lcd.p_status->p_current = lcd.p_status->p_current->child[key];
                lcd.p_status->p_item = lcd.p_status->p_current->p_item;
			     show_menu();	  		
            }
            else if(lcd.p_status->p_current->p_item[key - 1].flag == FILE)
            {
                lcd.p_status->p_item = &lcd.p_status->p_current->p_item[key - 1];
                lcd.p_status->p_current->p_item[key - 1].func_exec(0);
                num.init();
            }
        }
    }
}
void save()
{
    unsigned char k = 0;
    for(k = 1;k < 7;k ++)
    {
        if(root_n.child[k]->save != 0x00)
        {
            root_n.child[k]->save();
        }
    }
    // gps_n.save();
    // pid_n.save();
    // steer_n.save();
    // brake_n.save();
    // gpspath_n.save();
}
