#include "all.h"

char filename[20]="1.txt";
void choose_file(void)
{
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("CHOOSE_FILE");
		LCD_SetXY(0,2);
		LCD_WriteString("1.txt  2.txt");
		LCD_SetXY(0,3);
		LCD_WriteString("3.txt  4.txt");
		LCD_SetXY(0,4);
		LCD_WriteString("5.txt  6.txt");
		LCD_SetXY(0,5);
		LCD_WriteString("7.txt  8.txt");
		LCD_SetXY(0,6);
		LCD_WriteString("9.txt");
		
		refresh();	
		if(KeyReady==1)
		{
			KeyReady=0;
			switch(KeyValue)
			{
				case key1:
					filename[0]='1';
					return;
				case key2:
					filename[0]='2';
					return;
				case key3:
					filename[0]='3';
					return;
				case key4:
					filename[0]='4';
					return;
				case key5:
					filename[0]='5';
					return;
				case key6:
					filename[0]='6';
					return;
				case key7:
					filename[0]='7';
					return;
				case key8:
					filename[0]='8';
					return;
				case key9:
					filename[0]='9';
					return;
				
			}
	}
   delay(LCD_DELAY);
	}
}
