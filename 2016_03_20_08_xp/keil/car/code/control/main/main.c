/*
* @Author: obsidian
* @Date:   2016-03-01 20:39:05
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-14 22:26:12
*/
#include "stm32f10x.h"
#include "module/lcd/lcd_global.h"
#include "driver/delay/inc/delay.h"
#include "driver/led/inc/led.h"
#include "driver/sd/file.h"
#include "module/lcd/lcd_driver.h"
#include "module/lcd/menu/menu_pid.h"
#include "module/steer/steer_global.h"
#include "module/brake/brake_global.h"
#include "module/GPS/GPS_global.h"
#include "module/pid/pid_global.h"
#include "control/GPSpath/GPSpath_global.h"
#include "control/car.h"
#include "module/camera/camera_global.h"
#include "stm32f10x_flash.h"


void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //�����ⲿ���پ�������״̬ö�ٱ���
  RCC_DeInit();                                    //��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
  RCC_HSEConfig(RCC_HSE_ON);                       //���ⲿ���پ���
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //�ȴ��ⲿ����ʱ��׼����
  if(HSEStartUpStatus == SUCCESS)                  //�ⲿ����ʱ���Ѿ�׼���
  {
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //����FLASHԤ�����幦�ܣ�����FLASH�Ķ�ȡ�����г����б�����÷�.λ�ã�RCC��ʼ���Ӻ������棬ʱ������֮��
    FLASH_SetLatency(FLASH_Latency_2);                    //flash��������ʱ
      	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //����AHB(HCLK)ʱ�ӵ���==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //����APB2(PCLK2)��==AHBʱ��
    RCC_PCLK1Config(RCC_HCLK_Div1);                //����APB1(PCLK1)��==AHB1/2ʱ��
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //����PLLʱ�� == �ⲿ���پ���ʱ�� * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //ʹ��PLLʱ��
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //�ȴ�PLLʱ�Ӿ���
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //����ϵͳʱ�� = PLLʱ��
    while(RCC_GetSYSCLKSource() != 0x08)                  //���PLLʱ���Ƿ���Ϊϵͳʱ��
    {
    }
  }

}

int main()
{
	RCC_Configuration();
	  SYSTICK_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
	 delay_ms(100);
	//lcd.p_status->p_current = &pid_n;
    //send();
	/* code */
    LED_Configuration();
    SD_Initialize();
    f_mount_on();
    car.init();
	lcd.init();
	steer.init();
	gps.init();
	gpspath.init();
    brake.init();
    pid.init();
	camera.init();
   gps.status = ON;
    steer.status = ON;
   gpspath.status = ON;
		gpspath.timer_driver();
    while(1)
	{
// 		if(recv_flag == 1)
// 		{
// 			parse();
// 			recv_flag = 0;
// 		}
        if(lcd.update_flag == 1)
        {
            lcd.show_run();
            lcd.update_flag = 0;
        }
        delay_ms(200);
	}
}
