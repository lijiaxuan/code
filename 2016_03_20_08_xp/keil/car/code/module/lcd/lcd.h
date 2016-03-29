#ifndef LCD_H

#include "lcd_driver.h"
#include "stm32f10x.h"
void lcd_init(void);
#define LCD_DELAY 20
void LCD_Clear(void);
void menu_gps_enter(void* parg);
void menu_pid_enter(void* parg);
void menu_steer_enter(void* parg);
void menu_brake_enter(void* parg);
void menu_run_enter(void* parg);
void menu_run_back(void* parg);
void key_process(char key);
void input(void);
void numinput_init(void);
void numinput_input(char key);
void numinput_show(unsigned char pos);
void root_init(void *parg);

void menu_show_run(void);


#define LCD_H
#endif
