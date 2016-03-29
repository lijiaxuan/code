#include "stm32f4xx.h"
#include "math.h"
#include "vision.h"
#include "can_database.h"
#include "MT9M111.h"
#include "ILI9481.h"
#include "ext_sram.h"
#include "stdio.h"

void MedianFilter(unsigned char *pImgSrc,unsigned char *pImgDst,int nLength);

#ifdef WhiteTape

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

//#define EDGE_THRESH 80
#define EDGE_THRESH   50
#define POINT_NUM 1000
#define TRY_TIMES 20
#define LINE_WIDTH 3

u8 himg[120][160]={0};
u8 simg[120][160]={0};

//中值滤波后的数据
u8 fhimg[120][160]={0};
u8 fsimg[120][160]={0};

s16 fimg[60][80]={0};
s16 bwimg[60][80]={0};
u32 Px_u[POINT_NUM],Py_u[POINT_NUM],Pnum_u,Result_Num_u;
u32 Px_d[POINT_NUM],Py_d[POINT_NUM],Pnum_d,Result_Num_d;
s16 Result_Flag=0;
s16 Result_Angle_u,Result_Dist_u;
s16 Result_Angle_d,Result_Dist_d;
s16 Result_Angle,Result_Dist,Result_Angle_Last=0,Result_Dist_Last=0;
u16 time_flag=0; 



void can1_send_data(s16 angle,s16 dis)
{
	MISO_CAMERA_DATA.i16_data[0] = angle;
	MISO_CAMERA_DATA.i16_data[1] = dis;
	MISO_CAMERA_DATA.i16_data[2] = 0xff;
	MISO_CAMERA_DATA.i16_data[3] = 0xff;	
//	Write_Database(S_CAMERA_ID);
}

void HSV_Decompose(void){
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
		LCD_SetBox(IMG_STA_X,IMG_STA_Y,IMG_STA_X+159,IMG_STA_Y+119);//box1
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
				//simg[row/2][col/2]=s;
                simg[row/2][col/2]=b;
			}
		}		
        
// 	MedianFilter(&himg[0][0],&fhimg[0][0],120,160);
//     MedianFilter(&simg[0][0],&fsimg[0][0],120,160);          
        for(row=0; row<120; row++)
        {
            MedianFilter(&himg[row][0],&fhimg[row][0],160);
            MedianFilter(&simg[row][0],&fsimg[row][0],160);          
        }
            
		
	//simg

	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X+160,IMG_STA_Y,IMG_STA_X+160+159,IMG_STA_Y+119);//box2
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}

    
	for(row=0;row<120;row++) 
		for (col=0;col<160;col++)
		{
			if (LCD_OUTPUT)
			{
				color=fsimg[row][col]/1;
				color=make_rgb565(color,color*63/31,color);
				*(volatile u16 *) (LCD_DATA) = color;			
			}
			fimg[row/2][col/2]+=fsimg[row][col];
		}	
		
	//himg
	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y+120,IMG_STA_X+160-1,IMG_STA_Y+120+120-1);//box3
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
			himg[row/2][col/2]=fhimg[row][col];
		}	
	
}

void Sobel(void){
	u16 color = 0;
	s16 row,col,j;
	s8 flag_y_u,flag_y_d;
	u8 wsize=4;
	

    
	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X+160,IMG_STA_Y+120,IMG_STA_X+160+79,IMG_STA_Y+120+59);//box4
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}

	
	Pnum_u=0; 	Pnum_d=0; 
	for(row=0;row<60;row++) 
    {
		for (col=0;col<80;col++)
		{
			if (row<=wsize||row>=59-wsize||col<=wsize||col>=79-wsize)
			{
				bwimg[row][col]=0;
			}
			else
			{
				//bwimg[row][col]=fimg[row-1][col-1]+2*fimg[row-1][col]+fimg[row-1][col+1]-fimg[row+1][col-1]-2*fimg[row+1][col]-fimg[row+1][col+1];
                bwimg[row][col]=fimg[row-1][col-1]+2*fimg[row][col-1]+fimg[row+1][col-1]-fimg[row-1][col+1]-2*fimg[row][col+1]-fimg[row+1][col+1];
				if (bwimg[row][col]>EDGE_THRESH)
				{
					bwimg[row][col]=15;
					Pnum_u++;
					if (Pnum_u>=POINT_NUM)
						return;
					Px_u[Pnum_u]=col; Py_u[Pnum_u]=row;
				}
				else if (bwimg[row][col]<-EDGE_THRESH)
				{
					bwimg[row][col]=31;
					Pnum_d++;
					if (Pnum_d>=POINT_NUM)
						return;
					Px_d[Pnum_d]=col; Py_d[Pnum_d]=row;
				} else bwimg[row][col]=0;
// 				if (abs(bwimg[row][col])>30)
// 				{
// 					flag_y_u=0;flag_y_d=0;
// 					for (j=-wsize;j<=wsize;j+=2)
// 					{
// 						color=himg[row-wsize][col+j];
// 						if (color==15)
// 							flag_y_u++;
// 						else
// 							flag_y_u--;
// 						
// 						color=himg[row+wsize][col+j];
// 						if (color==15)
// 							flag_y_d++;
// 						else
// 							flag_y_d--;
// 					}
// 					
// 					if (flag_y_u>=wsize-1)
// 					{
// 						bwimg[row][col]=15;
// 						Pnum_u++;
// 						if (Pnum_u>=POINT_NUM)
// 							return;
// 						Px_u[Pnum_u]=col; Py_u[Pnum_u]=row;
// 					}
// 					else if(flag_y_d>=wsize-1)
// 					{
// 						bwimg[row][col]=31;
// 						Pnum_d++;
// 						if (Pnum_d>=POINT_NUM)
// 							return;
// 						Px_d[Pnum_d]=col; Py_d[Pnum_d]=row;
// 					}						
// 					else
// 						bwimg[row][col]=0;					
// 				}
// 				else 
// 					bwimg[row][col]=0;
			}
			
			if (LCD_OUTPUT)
			{
				color=bwimg[row][col];
				if (color==15)
					color=make_rgb565(31,0,0);
				if (color==31)
					color=make_rgb565(0,0,31);
				*(volatile u16 *) (LCD_DATA) = color;
			}
		}
    }
}

void GetLine(){
	s16 i,x1,y1,x2,y2,p1,p2,cnt;
	s32 num_in,xx,yy,xy,sx,sy;
	float k,b,dist,result_k_u=0,result_b_u=0,result_k_d=0,result_b_d=0;
	char str[20];
	
	if (Pnum_u<20)
		return;

	Result_Num_u=0;
	for (cnt=0;cnt<TRY_TIMES;cnt++)
	{
		p1=rand()%Pnum_u; p2=rand()%Pnum_u;
		if (p1==p2) continue;
		x1=Px_u[p1]; y1=Py_u[p1];  x2=Px_u[p2]; y2=Py_u[p2];

		k=((float)(y2)-(float)(y1))/((float)(x2)-(float)(x1));
		b=(float)(y1)-k*(float)(x1);
				
		num_in=0;
		for (i=0;i<Pnum_u;i++)
		{
			dist=fabs((k*(float)(Px_u[i])-(float)(Py_u[i])+b)/sqrt(k*k+1));
			if (dist<LINE_WIDTH)
				num_in++;
		}

		if (num_in<10)
			continue;
				
		//------------------------------
		xx=0; yy=0; xy=0; sx=0; sy=0; num_in=0;
		for (i=0;i<Pnum_u;i++)
		{
			dist=fabs((k*(float)(Px_u[i])-(float)(Py_u[i])+b)/sqrt(k*k+1));
			if (dist>LINE_WIDTH)
				continue;
			xx=xx+Px_u[i]*Px_u[i];  yy=yy+Py_u[i]*Py_u[i]; xy=xy+Px_u[i]*Py_u[i];
			sx=sx+Px_u[i]; sy+=Py_u[i];
			num_in++;	
		}
		
		if (num_in<Result_Num_u)
			continue;
		
		k=(float)(num_in*xy-sx*sy)/(float)(num_in*xx-sx*sx);
		b=(float)(xx*sy-sx*xy)/(float)(num_in*xx-sx*sx);				

		dist=(k*(float)(40)-(float)(30)+b)/sqrt(k*k+1);

		Result_Angle_u=(s16)(-atan(k)/3.1415926*1800+3600)%3600;
		Result_Dist_u=(s16)(dist*10);

		result_k_u=k;
		result_b_u=b;		
	}
	
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	if (Pnum_d<20)
		return;

	Result_Num_d=0;
	for (cnt=0;cnt<TRY_TIMES;cnt++)
	{
		p1=rand()%Pnum_d; p2=rand()%Pnum_d;
		if (p1==p2) continue;
		x1=Px_d[p1]; y1=Py_d[p1];  x2=Px_d[p2]; y2=Py_d[p2];

		k=((float)(y2)-(float)(y1))/((float)(x2)-(float)(x1));
		b=(float)(y1)-k*(float)(x1);
				
		num_in=0;
		for (i=0;i<Pnum_d;i++)
		{
			dist=fabs((k*(float)(Px_d[i])-(float)(Py_d[i])+b)/sqrt(k*k+1));
			if (dist<LINE_WIDTH)
				num_in++;
		}

		if (num_in<10)
			continue;
				
		//------------------------------
		xx=0; yy=0; xy=0; sx=0; sy=0; num_in=0;
		for (i=0;i<Pnum_d;i++)
		{
			dist=fabs((k*(float)(Px_d[i])-(float)(Py_d[i])+b)/sqrt(k*k+1));
			if (dist>LINE_WIDTH)
				continue;
			xx=xx+Px_d[i]*Px_d[i];  yy=yy+Py_d[i]*Py_d[i]; xy=xy+Px_d[i]*Py_d[i];
			sx=sx+Px_d[i]; sy+=Py_d[i];
			num_in++;	
		}
		
		if (num_in<Result_Num_d)
			continue;
		
		k=(float)(num_in*xy-sx*sy)/(float)(num_in*xx-sx*sx);
		b=(float)(xx*sy-sx*xy)/(float)(num_in*xx-sx*sx);				

		dist=(k*(float)(40)-(float)(30)+b)/sqrt(k*k+1);

		Result_Angle_d=(s16)(-atan(k)/3.1415926*1800+3600)%3600;
		Result_Dist_d=(s16)(dist*10);

		result_k_d=k;
		result_b_d=b;		
	}
	
	Result_Angle=(Result_Angle_u+Result_Angle_d)/2;
	Result_Dist=(Result_Dist_u+Result_Dist_d)/2;
	
	if (LCD_OUTPUT)
	{
		sprintf(str,"Angle_u:%d.%d   ",Result_Angle_u/10,abs(Result_Angle_u%10));	
		LCD_WriteString16_ili9481(8, 280, GREEN,DARKBLUE,str);
		sprintf(str,"Dist_u:%d.%d   ",Result_Dist_u/10,abs(Result_Dist_u%10));	
		LCD_WriteString16_ili9481(8, 300, GREEN,DARKBLUE,str);
		LCD_DrawLine_ili9481(IMG_STA_X+160,IMG_STA_Y+120+(u16)result_b_u,IMG_STA_X+160+80,IMG_STA_Y+120+(u16)(result_k_u*60+result_b_u),GREEN);

		sprintf(str,"Angle_d:%d.%d   ",Result_Angle_d/10,abs(Result_Angle_d%10));	
		LCD_WriteString16_ili9481(120, 280, GREEN,DARKBLUE,str);
		sprintf(str,"Dist_d:%d.%d   ",Result_Dist_d/10,abs(Result_Dist_d%10));	
		LCD_WriteString16_ili9481(120, 300, GREEN,DARKBLUE,str);
		LCD_DrawLine_ili9481(IMG_STA_X+160,IMG_STA_Y+120+(u16)result_b_d,IMG_STA_X+160+80,IMG_STA_Y+120+(u16)(result_k_d*60+result_b_d),GREEN);

		sprintf(str,"Angle:%d.%d   ",Result_Angle/10,abs(Result_Angle%10));	
		LCD_WriteString16_ili9481(250, 280, GREEN,DARKBLUE,str);
		sprintf(str,"Dist:%d.%d   ",Result_Dist/10,abs(Result_Dist%10));	
		LCD_WriteString16_ili9481(250, 300, GREEN,DARKBLUE,str);
		
 	  if (abs(Result_Angle-Result_Angle_Last)<10&&abs(Result_Dist-Result_Dist_Last)<50)		
		can1_send_data(Result_Angle,Result_Dist);
	}
	


	
	Result_Angle_Last=Result_Angle;
	Result_Dist_Last=Result_Dist;
	
}

void imshow(void)
{
    u32 ReadAddr=0x0000;
    
    u16 row,col;
    s16 r,g,b;	
    u16 color = 0;
    
    
    if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y,IMG_STA_X+320-1,IMG_STA_Y+240-1);//box1
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}

    for(row=0;row<240;row++) 
    {
		for (col=0;col<320;col++)
		{                                                                              
			color = *(vu16*)(Bank1_SRAM3_ADDR+ReadAddr);   
			ReadAddr+=2;
					
            r = get_r_value(color)*2;
            g = get_g_value(color);
            b = get_b_value(color)*2;

            if (LCD_OUTPUT)
            {
                color=make_rgb565(r/2,g,b/2);
                *(volatile u16 *) (LCD_DATA) = color;
            }
		}	
    }

}

	
#endif

