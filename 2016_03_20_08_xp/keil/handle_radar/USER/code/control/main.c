/* 
* @Author: obsidian (xjtu)
* @Date:   2015-12-07 15:44:35
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-23 17:26:05
*/

//#include "control/handle_control/inc/handle_control_global.h"
#include "driver/delay/inc/delay.h"
#include "module/bluetooth/inc/bluetooth_global.h"
#include "module/infrared/inc/infrared_global.h"
#include "math.h"
#include "driver/can/inc/can.h"

int main()
{
	/* code */
	SYSTICK_Init();
    infrared.infrared_init();
	bluetooth.bluetooth_init();
	bluetooth.status = 1;
   while(1)
	 {
		//radar.send_gs();
	 }

	return 0;
}
