/*
每加一个MOTOR CAN总线设备
	要在Can_Database列表中修改DATA_ID信息总线信息
*/

#include "stm32f4xx.h"
#include "can_database.h"
#include "can.h"
#include "command.h"

CAMERA_CMD CAM_CMD = FIND_NO_SEND;

uint8_t Can_Data_Num=0;
union fi64_to_u8 MISO_CAMERA_DATA;

//for transmiting image --added by fxh_bool
uint8_t MISO_CAM_TRANS[8];

//新加ID时，在ID_NUMDEF中定义相应ID的意义
Can_Data Can_Database[]=
{
	//Data_type		Data_ID				*Data_ptr					Data_length		*MenuFunc			Channel		Fifo_num
//主控&&MOTOR通信ID*/
	//MISO
	{READ_ONLY,		S_CAMERA1_ID,		(uint8_t *)(MISO_CAMERA_DATA.u8_data)		,8,		NULL,			1,		CAN_Filter_FIFO1},
	//MOSI
	{WRITE_ONLY,	W_CAMERA1_ID,		(uint8_t *)(&CAM_CMD)					,1,		W_CAMERA1_FUNC,	1,		CAN_Filter_FIFO0},
    //for transmit image --added by fxh_bool
  //{READ_ONLY,     S_CAM1_TRANS,            (uint8_t *)(&MISO_CAM_TRANS)            ,8,     NULL,   1,          CAN_Filter_FIFO0},
};

//HASH_TABLE初始值为255，认为Can_Data_Num值小于256，即设备数少于256个
uint8_t HASH_TABLE[256]={255};

void Hash_Table_init(void)
{
	int i;
	
	Can_Data_Num = sizeof(Can_Database) / sizeof(Can_Database[0]);
	for(i=0;i<256;i++)
	{
		HASH_TABLE[i] = 255;
	}
	for(i=0;i<Can_Data_Num;i++)
	{
		HASH_TABLE[Can_Database[i].Data_ID] = i;
	}
}

void Write_Database(ID_NUMDEF ID_NUM)
{
	uint8_t j;
	CanTxMsg TxMessage;
	/* Check the parameters */
	if((HASH_TABLE[ID_NUM] >= Can_Data_Num)&&(Can_Database[HASH_TABLE[ID_NUM]].Data_type!=READ_ONLY))
	{
// 		LED4_on;
		return;
	}
	
	TxMessage.StdId=Can_Database[HASH_TABLE[ID_NUM]].Data_ID;
	TxMessage.ExtId=0;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=Can_Database[HASH_TABLE[ID_NUM]].Data_length;
	
	for(j=0;j<Can_Database[HASH_TABLE[ID_NUM]].Data_length;j++)
	{
		TxMessage.Data[j]=(*(Can_Database[HASH_TABLE[ID_NUM]].Data_ptr+j));
	}
	
	Can_SendData(&TxMessage,Can_Database[HASH_TABLE[ID_NUM]].Channel);
	
}



