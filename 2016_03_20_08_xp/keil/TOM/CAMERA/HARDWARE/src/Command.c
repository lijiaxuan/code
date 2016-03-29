#include "command.h"
#include "can_database.h"

void S_CAMERA_FUNC(void)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		MISO_CAMERA_DATA.u8_data[i] = 0xaa;
	}
	Write_Database(S_CAMERA1_ID);
}

void W_CAMERA1_FUNC(void)
{
  
}
