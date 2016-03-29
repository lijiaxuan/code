#ifndef  KEY_H
#define  KEY_H


/*IO defination for CH452*/
//PE3->DCLK
#define CH452_DCLK_PERIPH   RCC_APB2Periph_GPIOE
#define CH452_DCLK_PORT     GPIOE
#define CH452_DCLK_PIN      GPIO_Pin_3
//PE4->DIN
#define CH452_DIN_PERIPH    RCC_APB2Periph_GPIOE
#define CH452_DIN_PORT      GPIOE
#define CH452_DIN_PIN       GPIO_Pin_4
//PE6->LOAD
#define CH452_LOAD_PERIPH   RCC_APB2Periph_GPIOE
#define CH452_LOAD_PORT     GPIOE
#define CH452_LOAD_PIN      GPIO_Pin_6
//PE5->DOUT
#define CH452_DOUT_PERIPH   RCC_APB2Periph_GPIOE
#define CH452_DOUT_PORT     GPIOE
#define CH452_DOUT_PIN      GPIO_Pin_5

#define CH452_CMD_NOP       0x000 //4
#define CH452_CMD_RESET     0x201 //12
#define CH452_CMD_SLEEP     0x202 //12
#define CH452_CMD_SYSCFG    0x400 //12
  #define CH452_SYSCFG_GPOE 0x040
  #define CH452_SYSCFG_INTM 0x020
  #define CH452_SYSCFG_SSPD 0x010
  #define CH452_SYSCFG_DPLR 0x008
  #define CH452_SYSCFG_WDOG 0x004
  #define CH452_SYSCFG_KEYB 0x002
  #define CH452_SYSCFG_DISP 0x001
#define CH452_CMD_GETKEY    0x700 //4

#define  KEY_1				0x5B
#define  KEY_2				0x5A
#define  KEY_3    		0x59
#define  KEY_MAINPG   0x42
#define  KEY_4    		0x58
#define  KEY_5				0x53
#define  KEY_6    		0x52
#define  KEY_DEL      0x41
#define  KEY_7    		0x51
#define  KEY_8    		0x50
#define  KEY_9    		0x4B
#define  KEY_UPMAIN   0x43
#define  KEY_0     		0x4A
#define  KEY_UPPAGE   0x49
#define  KEY_NEXTPAGE 0x48 
#define  KEY_OK   		0x40



#endif
