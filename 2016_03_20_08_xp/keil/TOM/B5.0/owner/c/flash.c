#include "all.h"

union fi64_to_u8 flash_save[11];

uint16_t flashread(uint32_t Address)
{
	uint16_t data;
	
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
	data= *((u16 *)Address);
	
	return data;
}
	
void Data_Save(void)
{
// 	uint8_t count;
// 	uint8_t i,flag;
// 	LED4_on;
// 	flash_save[0].i32_data[0] = Gyro_Float;
// 	flash_save[0].i32_data[1] = 0x00000000;
// 	flash_save[1].f64_data = Gyro_Convert1;
// 	flash_save[2].f64_data = Gyro_Convert2;
// 	flash_save[3].f64_data = Encoders[0].Convert1;
// 	flash_save[4].f64_data = Encoders[0].Convert2;
// 	flash_save[5].f64_data = Encoders[0].Radius;
// 	flash_save[6].f64_data = Encoders[0].radian;
// 	flash_save[7].f64_data = Encoders[1].Convert1;
// 	flash_save[8].f64_data = Encoders[1].Convert2;
// 	flash_save[9].f64_data = Encoders[1].Radius;
// 	flash_save[10].f64_data = Encoders[1].radian;
// 	
// 	FLASH_Unlock();
// 	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
// 	
// 	FLASH_ErasePage(PAGE_ADDRESS);
// 	for(count=0;count < SAVEDATA_NUM;count++)
// 	{
// 		for(i=0;i<4;i++)
// 		{
// 			flag = 0;
// 			while(flag==0)
// 			{
// 				FLASH_ProgramHalfWord((PAGE_ADDRESS +count*8+i*2),flash_save[count].u16_data[i]);  //flash  为一个字节存储，16位数据必须地址加2
// 				if(flashread((PAGE_ADDRESS +count*8+i*2)) == (flash_save[count].u16_data[i]))
// 				{
// 					flag = 1;
// 					LED2_off;
// 				}
// 				else
// 					LED2_on;
// 			}
// 		}
// 	}
// 	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);

// 	FLASH_Lock();
// 	
// 	LED4_off;

	uint8_t count;
	uint8_t i,flag;
	LED4_on;
	flash_save[0].i32_data[0] = Gyro_Float;
	flash_save[0].i32_data[1] = 0x00000000;
	flash_save[1].f64_data = Gyro_Convert1;
	flash_save[2].f64_data = Gyro_Convert2;
	flash_save[3].f64_data = Encoders[0].Convert;
// 	flash_save[4].f64_data = Encoders[0].Convert2;
// 	flash_save[5].f64_data = Encoders[0].Radius;
// 	flash_save[6].f64_data = Encoders[0].radian;
	flash_save[4].f64_data = Encoders[1].Convert;
// 	flash_save[8].f64_data = Encoders[1].Convert2;
// 	flash_save[9].f64_data = Encoders[1].Radius;
// 	flash_save[10].f64_data = Encoders[1].radian;
// 	
	FLASH_Unlock();
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
	
	FLASH_ErasePage(PAGE_ADDRESS);
	for(count=0;count < SAVEDATA_NUM;count++)
	{
		for(i=0;i<4;i++)
		{
			flag = 0;
			while(flag==0)
			{
				FLASH_ProgramHalfWord((PAGE_ADDRESS +count*8+i*2),flash_save[count].u16_data[i]);  //flash  为一个字节存储，16位数据必须地址加2
				if(flashread((PAGE_ADDRESS +count*8+i*2)) == (flash_save[count].u16_data[i]))
				{
					flag = 1;
					LED2_off;
				}
				else
					LED2_on;
			}
		}
	}
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);

	FLASH_Lock();
	
	LED4_off;

}

void Data_Load(void)
{
// 	uint8_t count;
// 	uint8_t i;
// 	FLASH_Unlock();
// 	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
// 	
// 	for(count=0;count < SAVEDATA_NUM;count++)
// 	{
// 		for(i=0;i<4;i++)
// 		{
// 			flash_save[count].u16_data[i] = flashread((PAGE_ADDRESS +count*8+i*2));    //flash  为一个字节存储，16位数据必须地址加2
// 		}
// 	}
// 	
// 	Gyro_Float				= flash_save[0].i32_data[0];
// 	Gyro_Convert1			= flash_save[1].f64_data;
// 	Gyro_Convert2			= flash_save[2].f64_data;
// 	Encoders[0].Convert1	= flash_save[3].f64_data;
// 	Encoders[0].Convert2	= flash_save[4].f64_data;
// 	Encoders[0].Radius		= flash_save[5].f64_data;
// 	Encoders[0].radian		= flash_save[6].f64_data;
// 	Encoders[1].Convert1	= flash_save[7].f64_data;
// 	Encoders[1].Convert2	= flash_save[8].f64_data;
// 	Encoders[1].Radius		= flash_save[9].f64_data;
// 	Encoders[1].radian		= flash_save[10].f64_data;
// 	
// 	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);

// 	FLASH_Lock();

	uint8_t count;
	uint8_t i;
	FLASH_Unlock();
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);
	
	for(count=0;count < SAVEDATA_NUM;count++)
	{
		for(i=0;i<4;i++)
		{
			flash_save[count].u16_data[i] = flashread((PAGE_ADDRESS +count*8+i*2));    //flash  为一个字节存储，16位数据必须地址加2
		}
	}
	
	Gyro_Float				= flash_save[0].i32_data[0];
	Gyro_Convert1			= flash_save[1].f64_data;
	Gyro_Convert2			= flash_save[2].f64_data;
	Encoders[0].Convert		= flash_save[3].f64_data;
// 	Encoders[0].Convert2	= flash_save[4].f64_data;
// 	Encoders[0].Radius		= flash_save[5].f64_data;
// 	Encoders[0].radian		= flash_save[6].f64_data;
	Encoders[1].Convert		= flash_save[4].f64_data;
// 	Encoders[1].Convert2	= flash_save[8].f64_data;
// 	Encoders[1].Radius		= flash_save[9].f64_data;
// 	Encoders[1].radian		= flash_save[10].f64_data;
	
	while(FLASH_GetFlagStatus(FLASH_FLAG_BSY)==1);

	FLASH_Lock();

}

