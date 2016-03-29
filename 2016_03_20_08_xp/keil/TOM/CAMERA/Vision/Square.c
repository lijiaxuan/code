#include "stm32f4xx.h"
#include "math.h"
#include "vison.h"
#include "can_database.h"
#include "MT9M111.h"
#include "ILI9481.h"
#include "ext_sram.h"
#include "stdio.h"

#ifdef Detect_Square


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
#define W_SIZE 3
#define Block_Num 10


#define Himg Bank1_SRAM3_ADDR+SIZE_H*SIZE_W*2

#define LCD_OUTPUT 1
#define LCD_DEBUG 0

s16 himg[120][160]={0};
s16 fimg[60][80]={0};
s16 bwimg[60][80]={0};

u16 time_flag=0;

s16 Result_Y,Result_X;


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

void Filter(void)
{
	u16 color = 0;
	s16 row,col,i,j;
	u8 flag_r,flag_b,flag_y;
	u8 wsize=W_SIZE;
	
	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y+120,IMG_STA_X+80-1-2*wsize,IMG_STA_Y+120+60-1-2*wsize);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}
	
	for(row=0+wsize;row<60-wsize;row++) 
		for (col=0+wsize;col<80-wsize;col++)
		{
			flag_r=0;flag_b=0;flag_y=0;
			for (i=-wsize;i<=wsize;i++)
				for (j=-wsize;j<=wsize;j++)
						{
							color=fimg[row+i][col+j];
							if (color==0)
								flag_r++;
							if (color==15*4)
								flag_y++;
							if (color==31*4)
								flag_b++;							
						}
			if (flag_y<10&&(flag_b>10||flag_r>10))
				bwimg[row][col]=31;
			else 
					bwimg[row][col]=0;
			
			if (LCD_OUTPUT)
			{
				color=bwimg[row][col];
				color=make_rgb565(color,color*63/31,color);
				*(volatile u16 *) (LCD_DATA) = color;
			}
		}
}

u16 Flag_FF[80][90],Tot_FF;

s16 dir_row[4]={0,1,0,-1};
s16 dir_col[4]={-1,0,1,0};
u16 Stack[80*80][3],Stack_Top; //row, col, dir

extern void LCD_DrawPixel(u16 x, u16 y,u16 color);
void dfs(s16 row,s16 col,u16 id)	
{
	u8 dir;

	Stack_Top=1; 
	Stack[1][0]=row; Stack[1][1]=col; Stack[1][2]=0; 
	
	while (Stack_Top>0)
	{
		dir=Stack[Stack_Top][2];
		if (dir==4)
		{	
			Stack_Top--;
			continue;
		}
		row=Stack[Stack_Top][0]+dir_row[dir]; 
		col=Stack[Stack_Top][1]+dir_col[dir];
		Stack[Stack_Top][2]++;
		
		if (row<0||col<0||row>=60||col>=80)
				continue;
		if (bwimg[row][col]==31&&Flag_FF[row][col]==0)
		{
			Stack_Top++;
			Stack[Stack_Top][0]=row;
			Stack[Stack_Top][1]=col;
			Stack[Stack_Top][2]=0;
			
			Flag_FF[row][col]=id;
		}
	}
		
	
}


s32	Dist_FF[Block_Num],Num_FF[Block_Num],index_FF[Block_Num];
s32	SumRow[Block_Num],SumCol[Block_Num];
float	score[Block_Num];

void FloodFill()
{
	s16 row,col,id,i,j,t;
	char str[20];
	float tmp;
	
	Tot_FF=0;
	for(row=0;row<60;row++) 
		for (col=0;col<80;col++)
			Flag_FF[row][col]=0;
		
	for(row=W_SIZE;row<60-W_SIZE;row++) 
		for (col=W_SIZE;col<80-W_SIZE;col++)
			if (bwimg[row][col]==31&&Flag_FF[row][col]==0)
				{
					Tot_FF++; 
					
					if (Tot_FF>Block_Num)
						break;
					
					index_FF[Tot_FF]=Tot_FF;
					SumRow[Tot_FF]=0;
					SumCol[Tot_FF]=0;
					Dist_FF[Tot_FF]=0;
					Num_FF[Tot_FF]=0;
					
					dfs(row,col,Tot_FF);
						
					sprintf(str,"Blocks:%d ",Tot_FF);	
					LCD_WriteString16_ili9481(200, 8, GREEN,DARKBLUE,str);
				}
				
	for(row=W_SIZE;row<60-W_SIZE;row++) 
		for (col=W_SIZE;col<80-W_SIZE;col++)
		{
			id=Flag_FF[row][col];
			Num_FF[id]++;
			Dist_FF[id]+=abs(row-30)+abs(col-40);
			SumRow[id]+=row;
			SumCol[id]+=col;
		}
	for(i=1;i<=Tot_FF;i++)
		if (Num_FF[i]>0)
			score[i]=(float)(Dist_FF[i])/(float)(Num_FF[i]);
		else
			score[i]=0;
		
	for(i=1;i<Tot_FF;i++)
		for(j=i+1;j<=Tot_FF;j++)
			if (score[i]>score[j])
			{
				tmp=score[i]; score[i]=score[j]; score[j]=tmp;
				id=index_FF[i]; index_FF[i]=index_FF[j]; index_FF[j]=id;
			}
	
	if (Num_FF[index_FF[1]]==0)
		return;
	row=(SumRow[index_FF[1]]/Num_FF[index_FF[1]]);
	col=(SumCol[index_FF[1]]/Num_FF[index_FF[1]]);
		
	Result_Y=row-30;
	Result_X=col-40;
			
	sprintf(str,"X:%d   Y:%d        ",Result_X,Result_Y);	
	LCD_WriteString16_ili9481(8, 280, GREEN,DARKBLUE,str);
	/*sprintf(str,"Dist:%d       ",Dist_FF[1]);	
	LCD_WriteString16_ili9481(8, 300, GREEN,DARKBLUE,str);
		
	for(row=W_SIZE;row<60-W_SIZE;row++) 
		for (col=W_SIZE;col<80-W_SIZE;col++)
			if (Flag_FF[row][col]==index_FF[1])
				LCD_DrawPixel(IMG_STA_X+col-W_SIZE,IMG_STA_Y+row+120-W_SIZE,RED);	
		*/	
	send_data(Result_X,Result_Y);

//	com2_send_data(0x11,0x22);
}

#endif



