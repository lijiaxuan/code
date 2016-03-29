/* 
* @Author: obsidian
* @Date:   2015-12-13 22:08:20
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-01 08:52:34
*/
#include "camera.h"
#include "camera_driver.h"
#include "camera_global.h"

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    uint8_t i;
    CanRxMsg   RxMessage;
    float a = 0;
    float b = 0;

    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
    a = *((short *)RxMessage.Data);

    camera.offset = a;
}
