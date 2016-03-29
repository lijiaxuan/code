#ifndef _CAN_DATABASE_H
#define _CAN_DATABASE_H
	#include "stm32f4xx.h"
	
	#ifndef NULL
	#define NULL ((void *)0)
	#endif
	
	#define fp64 double
	#define fp32  float

	/*Data_Type*/
	#define READ_ONLY  0	//���ض�������д
	#define WRITE_ONLY 1	//����д�������
	
	
/*���������Ӧ��ID���б�*/
/*
IDλ��	[	(GS:GPS Slave  MS:Motor Slave)							]
NO.7	[			0--GS			|		1--Others				]
NO.6	[	0--MOSI		|	1--MISO	|	0--MS MOSI	|	1--Others	]
NO.5~0	[	ID numbers				|	ID numbers  &  Others		]
*/
	typedef enum
	{
//B11-0x1-xxx ����&&cameraͨ��ID*/
	//MISO   B11-001-xxx     ��xxx��camera
		
		S_CAMERA1_ID	= 0xc8,
		S_CAMERA2_ID	= 0xc9,
		S_CAMERA3_ID	= 0xca,
		S_CAMERA4_ID	= 0xcb,
		S_CAMERA5_ID	= 0xcc,
		
	//MISO   B11-011-xxx     ��xxx��camera
		W_CAMERA1_ID	= 0xd8,
		W_CAMERA2_ID	= 0xd9,
		W_CAMERA3_ID	= 0xda,
		W_CAMERA4_ID	= 0xdb,
		W_CAMERA5_ID	= 0xdc,
        
		S_CAM1_TRANS  = 0xe8,
		S_CAM2_TRANS  = 0xe9,
		S_CAM3_TRANS  = 0xea,
		S_CAM4_TRANS  = 0xeb,
		S_CAM5_TRANS  = 0xec,

	}ID_NUMDEF;	
	
	/*���غ�GPSЭ����֮��������ʽ*/
	typedef enum
	{
		NO_COMMAND			= 0x00,
		//M_G_CMD_ID�µ�����
		GPS_READ			= 0x01,		//��ȡGPS��Ϣ����
		CHECK_FLOAT			= 0x02,		//֪ͨGPSЭ����У׼��������Ư����
		GYRO_INIT			= 0x03,		//֪ͨGPSЭ���ر궨����������
		ENC_L_INIT			= 0x04,		//֪ͨGPSЭ���ر궨��������תϵ������
		ENC_R_INIT			= 0x05,		//֪ͨGPSЭ���ر궨������ת�뾶����
		//G_M_CMD_ID�µ�����
		CF_NORMAL			= 0x06,
		CF_CHANGED			= 0x07,
		CF_ERROR			= 0x08
	}COM_TYPE;
	
	typedef struct
	{
		uint8_t  Data_type;
		ID_NUMDEF  Data_ID;
		uint8_t* Data_ptr;
		uint8_t  Data_length;
		
		//��can���߽ӵ����ݺ󣬵��õĺ���
		void (*MenuFunc)(void);			//��ں���		
		uint8_t  Channel;
		uint8_t  Fifo_num;			//�ڽ��շ�����ID���õ�fifo��
	}Can_Data;

	union fi64_to_u8     //fp32�����ݣ���int32_t������ת��u8�����ݽ���can���߷���
	{
		fp64	f64_data;
		int64_t i64_data;
		int16_t i16_data[4];
		uint8_t u8_data[8];
	};
	
	typedef enum
	{
		FIND_ONCE = 0x01,
		FIND_CONTINIOUS = 0x02,
		FIND_NO_SEND = 0x03,
		FIND_STOP = 0x00
	}CAMERA_CMD;
	
	extern uint8_t Can_Data_Num;
	extern Can_Data Can_Database[];
	extern uint8_t HASH_TABLE[256];

	extern union fi64_to_u8 MISO_CAMERA_DATA;
	extern CAMERA_CMD CAM_CMD;
	void Hash_Table_init(void);
	void Write_Database(ID_NUMDEF ID_NUM);

#endif

