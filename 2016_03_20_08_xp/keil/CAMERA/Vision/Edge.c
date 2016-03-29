#include "stm32f4xx.h"
#include "math.h"
#include "vison.h"
#include "can_database.h"
#include "MT9M111.h"
#include "ILI9481.h"
#include "ext_sram.h"
#include "stdio.h"

#ifdef Detect_Edge

////////////////////////////////////
#define get_b_value(rgb) ((unsigned char)(rgb&0x1f))
#define get_g_value(rgb) ((unsigned char)((rgb>>5)&0x3f))
#define get_r_value(rgb) ((unsigned char)((rgb>>11)&0x1f))
#define make_rgb565(r,g,b) ((r<<11)|(g<<5)|(b))
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)
//////////////////////////////////////


#define SIZE_H 240
#define SIZE_W 320


#define Himg Bank1_SRAM3_ADDR+SIZE_H*SIZE_W*2

#define LCD_OUTPUT 1
#define LCD_DEBUG 0

#define Mult_10 1

s16 himg[120][160]={0};
s16 fimg[60][80]={0};
s16 bwimg[60][80]={0};
u32 Px[4800],Py[4800],Pnum,Result_Num;
s16 Result_Flag=0;
s16 Result_Angle,Result_Dist;

u16 time_flag=0; 

void  com1_send_data(s16 angle,s16 dist)
{
	union s16_u8
	{
		s16 data_s16;
		u8 data_u8[2];
	}data;
	s16 sum=0;
	
	
	data.data_s16=angle;
	USART_SendData(USART1, (0xAB));
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	
	USART_SendData(USART1, (0xCD));
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	


	USART_SendData(USART1, data.data_u8[1]);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	
	USART_SendData(USART1, data.data_u8[0]);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	
	
	sum+=data.data_u8[0];
	sum+=data.data_u8[1];
	
	data.data_s16=dist;
	USART_SendData(USART1, data.data_u8[1]);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	
	USART_SendData(USART1, data.data_u8[0]);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	

	sum+=data.data_u8[0];
	sum+=data.data_u8[1];

	/*time_flag=(time_flag+1)&(0xFFF);
	USART_SendData(USART1, (u8)(time_flag>>8));
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	
	USART_SendData(USART1, (u8)(time_flag));
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 		*/
	
	data.data_s16=sum;
	USART_SendData(USART1, data.data_u8[1]);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	
	USART_SendData(USART1, data.data_u8[0]);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 	

}

void can1_send_data(s16 angle,s16 dist)
{
	MISO_CAMERA_DATA.i16_data[0] = angle;
	MISO_CAMERA_DATA.i16_data[1] = dist;
	MISO_CAMERA_DATA.i16_data[2] = 0xff;
  MISO_CAMERA_DATA.i16_data[3] = 0xff;	
	Write_Database(S_CAMERA_ID);
}

void send_data(s16 angle,s16 dist)
{
	#ifdef CAN_TRANSFER
		can1_send_data(angle,dist);
	#endif
	
	#ifdef UART_TRANSFER
		com1_send_data(angle,dist);
	#endif
}

void HSV_Decompose(void)
{
	u32 ReadAddr=0x0000;

	u16 color = 0;
	s16 r,g,b,h=0,s,v;	
	u16 row,col;
	u16 val_r,val_b,val_y,minval;
		
	
	
	for(row=0;row<60;row++) 
		for (col=0;col<80;col++)
		{
			bwimg[row][col]=0;
			fimg[row][col]=0;
		}
	
	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y,IMG_STA_X+159,IMG_STA_Y+119);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}
	
	for(row=0;row<240;row++) 
		for (col=0;col<320;col++)
		{                                                                              
			color = *(vu16*)(Bank1_SRAM3_ADDR+ReadAddr);   
			ReadAddr+=2;
					
			if (row%2&&col%2)
			{
				r = get_r_value(color)*2;
				g = get_g_value(color);
				b = get_b_value(color)*2;

				if (LCD_OUTPUT)
				{
					color=make_rgb565(r/2,g,b/2);
					*(volatile u16 *) (LCD_DATA) = color;
				}
			
				v=max(max(r,g),b);
				s=v-min(min(r,g),b);	
				
				if (v==r)
					h=(31*(g-b))/s;
				if (v==g)
					h=64+(31*(b-r))/s;
				if (v==b)
					h=128+(31*(r-g))/s;
				h/=6;
								
				h-=10;				
				if (h<0)
					h=31+h;

				val_r=abs(h-22);
				val_b=abs(h-11);
				val_y=abs(h-27);
				minval=min(min(val_r,val_b),val_y);
				if (minval==val_r)
					h=0;
				if (minval==val_y)
					h=15;
				if (minval==val_b)
					h=31;
				
				
				himg[row/2][col/2]=h;					
				if (s<5)
					bwimg[row/4][col/4]=1;
			}
		}			
	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X+160,IMG_STA_Y,IMG_STA_X+160+159,IMG_STA_Y+119);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}
	
	for(row=0;row<120;row++) 
		for (col=0;col<160;col++)
		{
			if (LCD_OUTPUT)
			{
				color=himg[row][col]/1;
				color=make_rgb565(color,color*63/31,color);
				*(volatile u16 *) (LCD_DATA) = color;			
			}
			fimg[row/2][col/2]+=himg[row][col];
		}	
}

void Sobel(void)
{
	u16 color = 0;
	s16 row,col,i,j;
	s8 flag_r,flag_b,flag_y;
	u8 wsize=4;
	
	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y+120,IMG_STA_X+79,IMG_STA_Y+120+59);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}
	
	Pnum=0; Result_Num=0;
	for(row=0;row<60;row++) 
		for (col=0;col<80;col++)
		{
			flag_r=0;flag_b=0;flag_y=0;
			if (bwimg[row][col]==1||row<=wsize||row>=59-wsize||col<=wsize||col>=79-wsize)
			{
				bwimg[row][col]=0;
			}
			else
			{
				bwimg[row][col]=fimg[row-1][col-1]+2*fimg[row-1][col]+fimg[row-1][col+1]-fimg[row+1][col-1]-2*fimg[row+1][col]-fimg[row+1][col+1];
				
				if (abs(bwimg[row][col])>31)
				{
					flag_r=0; flag_b=0; flag_y=0;
					for (j=-wsize;j<=wsize;j+=2)
					{
						color=fimg[row+wsize][col+j];
						if (color==0)
							flag_r++;
						if (color==15*4)
							flag_y--;
						if (color==31*4)
							flag_b++;
						
						color=fimg[row-wsize][col+j];
						if (color==15*4)
							flag_y++;
					}
					
					if (flag_y>3&&(flag_r>3||flag_b>3))
					{
						bwimg[row][col]=31;
						Pnum++;
						Px[Pnum]=col; Py[Pnum]=row;
					}
					else 
						bwimg[row][col]=0;
					
					/*
					for (i=-1;i<=1;i++)
						for (j=-1;j<=1;j++)
						{
							color=fimg[row+i][col+j];
							if (color==0)
								flag_r=1;
							if (color==15*4)
								flag_y=1;
							if (color==31*4)
								flag_b=1;							
						}
					if (flag_y&&(flag_r||flag_b))
					{
						bwimg[row][col]=31;
						Pnum++;
						Px[Pnum]=col; Py[Pnum]=row;
					}
					else 
						bwimg[row][col]=0;
					*/
					
				}
				else 
					bwimg[row][col]=0;
			}
			
			if (LCD_OUTPUT)
			{
				color=bwimg[row][col];
				color=make_rgb565(color,color*63/31,color);
				*(volatile u16 *) (LCD_DATA) = color;
			}
		}
}
void GetLine()
{
	s16 i,x1,y1,x2,y2,p1,p2,cnt;
	s32 num_in,xx,yy,xy,sx,sy;
	double k,b,dist,result_k=0,result_b=0;
	char str[20];
	u32 t;
	
//	for (t=0;t<4100000;t++);
	//com2_send_data((s16)(0x11),(s16)(0xc5));
	//return;
	
	if (Pnum<20||Pnum>1000)
		return;
	Result_Num=0;
	
	for (cnt=0;cnt<20;cnt++)
	{
		p1=rand()%Pnum; p2=rand()%Pnum;
		if (p1==p2) continue;
		x1=Px[p1]; y1=Py[p1];  x2=Px[p2]; y2=Py[p2];

		k=((float)(y2)-(float)(y1))/((float)(x2)-(float)(x1));
		b=(float)(y1)-k*(float)(x1);
				
		num_in=0;
		for (i=0;i<Pnum;i++)
		{
			dist=fabs((k*(float)(Px[i])-(float)(Py[i])+b)/sqrt(k*k+1));
			if (dist<5)
				num_in++;
		}

		if (num_in<20)
			continue;
				
		//------------------------------
		xx=0; yy=0; xy=0; sx=0; sy=0; num_in=0;
		for (i=0;i<Pnum;i++)
		{
			dist=fabs((k*(float)(Px[i])-(float)(Py[i])+b)/sqrt(k*k+1));
			if (dist>5)
				continue;
			xx=xx+Px[i]*Px[i];  yy=yy+Py[i]*Py[i]; xy=xy+Px[i]*Py[i];
			sx=sx+Px[i]; sy+=Py[i];
			num_in++;
			
		}
		
		if (num_in<Result_Num)
			continue;
		Result_Num=num_in;
		
		k=(double)(num_in*xy-sx*sy)/(double)(num_in*xx-sx*sx);
		b=(double)(xx*sy-sx*xy)/(double)(num_in*xx-sx*sx);		
		

		dist=fabs((k*(double)(40)-(double)(60)+b)/sqrt(k*k+1));
		if (Mult_10)
		{
			Result_Angle=(s16)(-atan(k)/3.1415926*1800+3600)%3600;
			Result_Dist=(s16)(dist*10-300);
		}
		else
		{		
			Result_Angle=(s16)(-atan(k)/3.1415926*180+360)%360;
			Result_Dist=(s16)(dist*1-30);
		}
		
		result_k=k;
		result_b=b;
		
		if (LCD_DEBUG)
		{
			sprintf(str,"p1:%d p2:%d cnt:%d num:%d  ",p1,p2,cnt,num_in);	
			LCD_WriteString16_ili9481(8, 8, GREEN,DARKBLUE,str);
			sprintf(str,"xx:%d k:%.1f b:%.1f",xx,k,b);	
			LCD_WriteString16_ili9481(8, 280, GREEN,DARKBLUE,str);

			for (t=0;t<30000000;t++);
			continue;
		}
	}
	
	send_data(Result_Angle,Result_Dist);
	
	if (Mult_10)
	{
		sprintf(str,"k:%.2f    Angle:%d.%d   ",result_k,Result_Angle/10,abs(Result_Angle%10));	
		LCD_WriteString16_ili9481(8, 280, GREEN,DARKBLUE,str);
		sprintf(str,"b:%.2f    Dist:%d.%d   ",result_b,Result_Dist/10,abs(Result_Dist%10));	
		LCD_WriteString16_ili9481(8, 300, GREEN,DARKBLUE,str);
	}
	else
	{
		sprintf(str,"k:%.2f    Angle:%d  ",result_k,Result_Angle);	
		LCD_WriteString16_ili9481(8, 280, GREEN,DARKBLUE,str);
		sprintf(str,"b:%.2f    Dist:%d   ",result_b,Result_Dist);
		LCD_WriteString16_ili9481(8, 300, GREEN,DARKBLUE,str);
	}
	if (LCD_OUTPUT)
	{
		LCD_DrawLine_ili9481(IMG_STA_X,IMG_STA_Y+119+(u16)result_b,IMG_STA_X+79,IMG_STA_Y+119+(u16)(result_k*60+result_b),GREEN);
	}
		
	
// 	void LCD_DrawLine_ili9481(u16 x1,u16 y1,u16 x2,u16 y2,u16 cl)
// 	LCD_DrawPixel(u16 x, u16 y,u16 color)
// 	LCD_WriteString16_ili9481(8, 8, GREEN,panelcolor,str);
}

#endif
