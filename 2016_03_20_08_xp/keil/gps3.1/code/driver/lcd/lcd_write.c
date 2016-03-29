#ifndef LCD_WRITE_C
#define LCD_WRITE_C

#include "lcd.h"
//
uint8_t LCD_Table[LCD_LEN];
uint8_t LCD_Index=0;




void LCD_PushByte(uint8_t data)		//发送一个字符
{
	LCD_Table[LCD_Index]=data;
	if(LCD_Index >= (LCD_LEN-1))
		LCD_Index=0;
	else
		LCD_Index++;
}

void LCD_PushString(char *data)		//发送一个字符串
{
	while(*data)
	{
		LCD_PushByte(*data++);
	}
}


void LCD_SetXY(int x,int y)
{
	LCD_Index=x+y*20;
}

void LCD_WriteChar(char data)
{
	LCD_PushByte(data);
}

void LCD_WriteString(char *data)
{
	LCD_PushString(data);
}

void LCD_WriteInt(int data)//写整数，从低位向高位写
{
	char answer[100];
	int index=98;
	int pose=0;//正负号标志符
	if(data<0)
	{
		pose=1;
		data=-data;
	}
	answer[99]='\0';//最低位存储结束标志
	while(data>9)//存储整数位
	{
		answer[index--]=data%10+'0';
		data=data/10;
	}
	answer[index]=data+'0';
	if(pose)//存储正负号
	{
		answer[--index]='-';
	}
	LCD_PushString(answer+index);//写整个数组
}

void LCD_WriteDouble(double data,int m)//写double型数，m为小数点后位数，从高位向低位写
{
	if(data<0&&data>-1)
		LCD_WriteChar('-');//写负号
	LCD_WriteInt((int)data);//写整数部分
	if(m>0)
	{
		int i;
		LCD_WriteChar('.');//写小数点
		if(data<0)
			data=-data;
		data=data-(int)data;
		for(i=0;i<m;i++)//写小数部分
		{
			data=data*10;
			LCD_WriteChar((int)data+'0');
			data=data-(int)data;
		}
		
	}
	
}


void LCD_WriteNum(double data)//写double型数，小数点后保留6位，从高位向低位写
{
	char str[5];
	int flag=0,i;
	if(data<0)
	{
		LCD_WriteChar('-');//写负号
		data=-data;
	}
	LCD_WriteInt((int)data);//写整数部分
	data=data-(int)data;
	for(i=0;i<5;i++)//将小数部分保留在一个字符串内
	{
		data=data*10;
		str[i]=(int)data+'0';
		if(str[i]!='0')
		{
			flag=i+1;
		}
		data=data-(int)data;
		
	}
	if(flag!=0)
		LCD_WriteChar('.');//写小数点
	for(i=0;i<flag;i++)
	{
		LCD_WriteChar(str[i]);//写小数部分
	}	
	for(i=flag;i<6;i++)
	{
		LCD_WriteChar(' ');//小数点后不够6位则补空格
	}
}

void LCD_WriteCharXY(char data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_PushByte(data);
}

void LCD_WriteStringXY(char *data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_PushString(data);
}

void LCD_WriteIntXY(int data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_WriteInt(data);
}

void LCD_WriteDoubleXY(double data,int m,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_WriteDouble(data, m);
}

void LCD_WriteNumXY(double data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_WriteNum(data);
}

void LCD_Clear(void)//清屏时会同时将光标置(0,0)
{
	//LCD_WriteChar(12);//不用命令清屏时为了LCD_PushByte()函数可以记录LCD内容
	LCD_WriteStringXY("                                                                                ",0,0); 
	LCD_WriteStringXY("                                                                                ",0,4); 

	LCD_SetXY(0,0);
}

void LCD_Clear_Line(u8 C_line)
{
	LCD_WriteStringXY("                    ",0,C_line); 
}

void LCD_Clear_Forward(u8 L_count)
{
	char i=0;
	for(i=0;i<L_count;i++)
	{
		LCD_Clear_Line(i);
	}
}


/*********************************************
*	*	uart向电脑发送数据
********************************************/

void WriteChar(char data)
{
	UART3_SendByte(data);
}

void WriteString(char *data)		//发送一个字符串
{
	while(*data)
	{
		WriteChar(*data++);
	}
}

void WriteInt(int data)//写整数，从低位向高位写
{
	char answer[100];
	int index=98;
	int pose=0;//正负号标志符
	if(data<0)
	{
		pose=1;
		data=-data;
	}
	answer[99]='\0';//最低位存储结束标志
	while(data>9)//存储整数位
	{
		answer[index--]=data%10+'0';
		data=data/10;
	}
	answer[index]=data+'0';
	if(pose)//存储正负号
	{
		answer[--index]='-';
	}
	WriteString(answer+index);//写整个数组
}

void WriteDouble(double data,int m)//写double型数，m为小数点后位数，从高位向低位写
{
	if(data<0&&data>-1)
		WriteChar('-');//写负号
		WriteInt((int)data);//写整数部分
	if(m>0)
	{
		int i;
		WriteChar('.');//写小数点
		if(data<0)
			data=-data;
		data=data-(int)data;
		for(i=0;i<m;i++)//写小数部分
		{
			data=data*10;
			WriteChar((int)data+'0');
			data=data-(int)data;
		}
		
	}
	
}

//4行20列共80单元，每10个单元为一组刷新
//液晶可写20列，8行;从0列开始，从0行开始

void refresh(void)
{
//		static	char send_flag=0;
		static	int m=0,n=0;
	
// 		if(send_flag%2==0)
// 		{
			UART3_SendByte(0xff);
			UART3_SendByte(30*(m/2)+(m%2)*10);
			delay(1);
			for(n=0;n<=9;n++)
			{
				UART3_SendByte (LCD_Table[10*m+n]);
			}
			if(m>=15)
				m= 0;
			else
				m++;
// 		}
// 		send_flag++;
}

void refresh_slow(void)
{
		static	char send_flag=0;
		static	int m=0,n=0;
	
 		if(send_flag%4==0)
 		{
			UART3_SendByte(0xff);
			UART3_SendByte(30*(m/2)+(m%2)*10);
			delay(1);
			for(n=0;n<=9;n++)
			{
				UART3_SendByte (LCD_Table[10*m+n]);
			}
			if(m>=15)
				m= 0;
			else
				m++;
 		}
 		send_flag++;
}
#endif






