/*
每加一个CAN总线设备
	1、更改Can_Data_Num定义值
	2、要在Can_Database列表中添加总线信息，特别注意不要搞错
	3、void Hash_Table_init(void)中，添加新加设备在Can_Database列表中的序号
	4、添加ID_NUMDEF枚举定义中加入新添ID代表的命令意义
*/

#include "all.h"

uint8_t Can_Data_Num=0;

uint8_t MOSI_CMD;
uint8_t MISO_CMD;

union fi64_to_u8 MISO_CAMER_OFFSET;
union fi64_to_u8 MISO_CAMER_ANGLE;

union fi64_to_u8 MOSI_PWM;

//新加ID时，在ID_NUMDEF中定义相应ID的意义

Can_Data Can_Database[]=
{
	//Data_type		Data_ID				*Data_ptr					Data_length		*MenuFunc		Channel		Fifo_num
//主控&&GPS协主控通信ID*/
    //data_type 是相对于主控来说的
    //主控是只写 那么协主控就是要读 那么就要加入它的过滤器
    //********************************************************************/
    //我改变了  对自己来说 是READ 就加入接收过滤器中 时间       2016/2/16
    //******************************************************************/
      {READ_ONLY,	W_PWM_ID,			(uint8_t *)(MOSI_PWM.u8_data)		,8,		W_PWM_FUNC,			1,			CAN_Filter_FIFO0},

	//{WRITE_ONLY,	M_G_CMD_ID,			(uint8_t *)(&MOSI_CMD)				,1,		M_G_CMD_FUNC,		1,			CAN_Filter_FIFO0},
	//{WRITE_ONLY,	W_GPS_X_ID,			(uint8_t *)(MOSI_GPS_X.u8_data)		,8,		W_GPS_X_FUNC,		1,			CAN_Filter_FIFO1},

       {WRITE_ONLY,		G_M_CMD_ID,			(uint8_t *)(&MISO_CMD)				,1,		NULL,				1,			CAN_Filter_FIFO0},
	//{READ_ONLY,		G_M_CMD_ID,			(uint8_t *)(&MISO_CMD)				,1,		NULL,				1,			CAN_Filter_FIFO0},
	//{READ_ONLY,		S_GPS_X_ID,			(uint8_t *)(MISO_GPS_X.u8_data)		,8,		NULL,				1,			CAN_Filter_FIFO0},
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
//向can写数据 先初始华 Data_ID对就 candatabase的数据 
//调用这个函数  参数为 Data_ID
//前提为使用了hash_table
void Write_Database(ID_NUMDEF ID_NUM)
{
	uint8_t j;
	CanTxMsg TxMessage;
	/* Check the parameters */
	if((HASH_TABLE[ID_NUM] >= Can_Data_Num)||(Can_Database[HASH_TABLE[ID_NUM]].Data_type!=WRITE_ONLY))
	{
		LED4_on;
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
	
	Can_SendData(&TxMessage);
	
}



