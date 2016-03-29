#ifndef LCD_DRIVER_H
/*
    include
 */
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#define DISPLAY      0xf2
#define CLEAR        0xf3
#define FRAME_LEN     14

/*
    define
 */

#define LCD_LEN  160
#define INPUT    1

/*
    global
 */

extern unsigned int     USART3_TxBuffer[12];
extern unsigned char    keyvalue;
extern unsigned char    KeyReady;
extern unsigned char    ChangK ;
#define WHITE            0xFFFF
#define BLACK            0x0000   
#define BLUE             0x001F  
#define BRED             0XF81F
#define GRED             0XFFE0
#define GBLUE            0X07FF
#define RED              (~0xF800)
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN            0XBC40 //趹伎
#define BRRED            0XFC07 //趹綻伎
#define GRAY             0X8430 //閡伎
//GUI晇伎

#define DARKBLUE         0X01CF //旮懦伎
#define LIGHTBLUE        0X7D7C //シ懦伎  
#define GRAYBLUE         0X5458 //閡懦伎
//眕奻ʊ伎峈PANEL腔晇伎 
 
#define LIGHTGREEN       0X841F //シ蟯伎
#define LIGHTGRAY        0XEF5B //シ閡伎(PANNEL)
#define LGRAY            0XC618 //シ閡伎(PANNEL),敦极掖劓伎

#define LGRAYBLUE        0XA651 //シ閡懦伎(笢潔脯晇伎)
#define LBBLUE           0X2B12 //シ趹懦伎(恁寁沭醴腔毀伎)

void UART3_init(u32 bound);
void TFT_Clear( void );
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode);
void lcd_timer_cfg(void);
#define LCD_DRIVER_H
#endif
