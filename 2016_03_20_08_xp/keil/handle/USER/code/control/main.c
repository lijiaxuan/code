/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 15:44:35
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-24 09:35:31
*/

#include "control/handle_control/inc/handle_control_global.h"
#include "driver/delay/inc/delay.h"
#include "module/GPS/inc/GPS_global.h"
#include "module/steer/inc/steer_global.h"
#include "control/GPSpath/inc/GPSpath_global.h"
#include "module/bluetooth/inc/bluetooth_global.h"
#include "module/radar/inc/radar_global.h"

int main()
{
	/* code */
	SYSTICK_Init();
	//bluetooth.bluetooth_init(9600);
	//steer.steer_init();
	//gps.GPS_init();
	radar.init(&gps.point.x, &gps.point.y, &gps.radian);
	//
	
	//steer.set_angle(0);
	//gpspath.status = 1;
	while(1)
	{
		//handle_control.m_handle_control();
	}

	return 0;
}
