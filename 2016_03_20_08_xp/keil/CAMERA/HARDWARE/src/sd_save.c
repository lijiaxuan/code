#include "stm32f4xx.h"
#include "string.h"
#include "can_database.h"

/** @variables---------------------------------- */

int normal_flag = 1;            /* default mode */   
u8 cam_trsimg_fh[] =            /* camera transmit image frame head */
    {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, };

void cmd_rx(char data[])
{
    if(!strncmp((const char *)&cam_trsimg_fh[0], (const char *)&data[0], sizeof(cam_trsimg_fh)))
    {
        normal_flag = 0;
    }
    else
    {/* show err here */}
}

/*---------------------------------------------------
 | 摄像头发送数据                                  |
 *--------------------------------------------------*/
const static u32 img_size = (320*240*2);  /* 大小和主控那边要一致 */

static void delay(int i)
{while(i-->0);}

u8 image[1024*5] = {'a', 'b', 'c', 'd'};
void send_img(vu8 img[])
{
    u32 i=0, j=0, k=0;
    u32 n_frame = img_size/8;
    int img_index = 0;
    CanTxMsg txmsg;
    
  txmsg.StdId=S_CAM1_TRANS;
	txmsg.ExtId=0;
	txmsg.IDE=CAN_ID_STD;
	txmsg.RTR=CAN_RTR_DATA;
	txmsg.DLC=8;
    
    /* start frame */
    txmsg.DLC=8;
    for(j=0; j<8; j++)
        txmsg.Data[j] = cam_trsimg_fh[j];
    while(CAN_Transmit(CAN1, &txmsg)==CAN_NO_MB);
    
    txmsg.DLC=8;
    /* transmit image */
        for(i=0; i<img_size/8; i++)
        {
            for(j=0; j<8; j++)
                txmsg.Data[j] = *(vu8*)(img + img_index++);
            
            while(CAN_Transmit(CAN1, &txmsg)==CAN_NO_MB);
        }
    /* end frame */
    txmsg.DLC=8;
    for(j=0; j<8; j++)
        txmsg.Data[j] = cam_trsimg_fh[j];
    while(CAN_Transmit(CAN1, &txmsg)==CAN_NO_MB);
    
    /* recoverv to normal mode */
    normal_flag=1;
}
