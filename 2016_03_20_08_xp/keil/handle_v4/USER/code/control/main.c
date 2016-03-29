/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 15:44:35
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-28 14:30:31
*/

//#include "control/handle_control/inc/handle_control_global.h"
#include "driver/delay/inc/delay.h"
#include "module/GPS/inc/GPS_global.h"
#include "module/steer/inc/steer_global.h"
#include "module/brake/inc/brake_global.h"
#include "control/GPSpath/inc/GPSpath_global.h"
#include "module/bluetooth/inc/bluetooth_global.h"
#include "module/infrared_s/inc/infrared_global.h"
#include "module/radar/inc/radar_global.h"
#include "module/pid/inc/pid_global.h"
#include "math.h"
#include "driver/led/inc/led.h"
#include "stm32f10x_can.h"
#include "stm32f10x_flash.h"
#include "driver/can/inc/canljx.h"
#include "module/camera/inc/camera_global.h"
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

void gps_init()
{

//     gps.rear_point.x = 0;
//     gps.rear_point.y = 0;
//     gps.base_radian = 0;
// 	gpspath.current_index = 0;	
// 	
//     gps.rear_point.x = 0;
//     gps.rear_point.y = 700;
//     gps.base_radian = 0;
// 	gpspath.current_index = 100;	
// 	
    gps.rear_point.x = -565;
    gps.rear_point.y = 7265 - 80;
    gps.base_radian = -45 / 57.3;
	gpspath.current_index = 2100;	

//     gps.rear_point.x = 1331;
//     gps.rear_point.y = 11027;
//     gps.base_radian = 52 / 57.3;
// 	gpspath.current_index = 3035;	
	  gps.radian = gps.base_radian;
	  gps.front_point.x = gps.rear_point.x + gps.car_len * sin(gps.radian);
	  gps.front_point.y = gps.rear_point.y + gps.car_len * cos(gps.radian);
    gpspath.status = ON;
		gps.GPS_init();
}

int main()
{
	/* code */
	RCC_Configuration();
	SYSTICK_Init();
	steer.steer_init();
  brake.brake_init();
	brake.timer_cfg();
	gpspath.timer_cfg();
 	
  //radar.init();
  bluetooth.bluetooth_init();
  bluetooth.status = 1;
	gps_init();
  infrared.infrared_init();
   gps.status = ON; //
    delay_ms(200);
    //radar.status = ON;
	 camera.can_init();
    pid.pid_clear();
    steer.status = ON;	

    LED2_off;
    while(1)
	{
	}

	return 0;
}
