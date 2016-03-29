#include "stm32f4xx.h"
#include "math.h"
#include "vision.h"
#include "can_database.h"
#include "MT9M111.h"
#include "ILI9481.h"
#include "ext_sram.h"
#include "stdio.h"


#ifdef Detect_Edge_And_WhiteTape_V2


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

//#define EDGE_THRESH 100
#define POINT_NUM 1000
#define TRY_TIMES 30

#define WHITE_THRESH 2.2
#define BLACK_THRESH 2.2
#define LINE_THRESH 0.8

#define MAX_CNT 10
//窗口边界留出5
//New Robot
//Side
// #define T_RANGE 5 //0~60
// #define B_RANGE 55 //0~60
// #define L_RANGE 10 //0~80
// #define R_RANGE 60 //0~80
//Top
// #define T_RANGE 10
// #define B_RANGE 50
// #define L_RANGE 25
// #define R_RANGE 60

//Old Robot 
//Side
// #define T_RANGE 5
// #define B_RANGE 55
// #define L_RANGE 10
// #define R_RANGE 60

// //Top
#define T_RANGE 10
#define B_RANGE 50
#define L_RANGE 5
#define R_RANGE 55



#define LIST_LEN 3

char lcd_str[20];

u8 himg[120][160];
u8 simg[120][160];
s16 fimg[60][80];
s16 bwimg[60][80];

u8 Maxs=0,Mins=100,Maxv=0,Minv=100;
u8 h_thresh=31;
u8 col_sum[80],result_list[80];
float mat_inv[3][3]={{5.8719315839,-0.1151745630,-472.9051605697},{-0.0131421586,7.3140866707,-418.7892691836},{-0.0000562186,0.0113870347,2.0588986315}};

s16 LINE_WIDTH=3;
u32 Px[POINT_NUM],Py[POINT_NUM],Pnum,Result_Num;
s16 Result_Flag=0;
s16 Result_Angle,Result_Dist,Result_Angle_Last=0,Result_Dist_Last=0;
u16 time_flag=0; 

u8 Transmit_Cnt=0;
s16 Angle_List[LIST_LEN],Dist_List[LIST_LEN];

u8 Flag_Edge=0;
u8 Flag_WhiteTape=1;

void can1_send_data(s16 angle,s16 dist){
	MISO_CAMERA_DATA.i16_data[0] = angle;
	MISO_CAMERA_DATA.i16_data[1] = dist;
	MISO_CAMERA_DATA.i16_data[2] = 0xff;
  MISO_CAMERA_DATA.i16_data[3] = 0xff;	
	Write_Database(S_CAMERA_ID);
}


void HSV_Decompose(void){
	u32 ReadAddr=0x0000;

	u16 color = 0;
	s16 r,g,b,h=0,s,v;	
	u16 row,col;
	u16 val_r,val_b,val_y,minval;
	u16 cnt_ny=0;
		
	Maxs=0,Mins=100;
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
		for (col=0;col<320;col++)//扫描原图320*240
		{                                                                              
			color = *(vu16*)(Bank1_SRAM3_ADDR+ReadAddr);   
			ReadAddr+=2;
					
			if (row%2&&col%2) //降采样
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
					h=31+h; //算HSV分量

				val_r=abs(h-22);//看圆用 红色对应色相
				val_b=abs(h-11);//看圆用 蓝色对应色相
				val_y=abs(h-27);//看圆用 黄色对应色相
				
				minval=min(min(val_r,val_b),val_y); //看哪个颜色距离小
				if (minval==val_y)
					h=15;
				else
				{
					if (s>8)
						cnt_ny++;
					if (minval==val_r)
						h=0;
					if (minval==val_b)
						h=31;
				}
				
				if (Flag_Edge)
					himg[row/2][col/2]=h; //保存色相
				else if (Flag_WhiteTape)
					himg[row/2][col/2]=v/2;//保存亮度
				else 
					himg[row/2][col/2]=0; //卧槽
				
				simg[row/2][col/2]=s;
				bwimg[row/4][col/4]=0; //black-white img 60*80
				
				if (col>L_RANGE*4&&col<R_RANGE*4&&row>T_RANGE*4&&row<B_RANGE*4)//窗口!!!!!!!!!!!!!!!
				{
					Maxs=max(Maxs,s);//
					Mins=min(Mins,s);
					Maxv=max(Maxv,v/2);
					Minv=min(Minv,v/2);
				}
			}
		}				
		
		
	//simg
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
				color=simg[row][col]/1;
				color=make_rgb565(color,color*63/31,color);
				*(volatile u16 *) (LCD_DATA) = color;			
			}
			if (Flag_WhiteTape)
				fimg[row/2][col/2]+=simg[row][col];
		}	
		
	//himg
	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y+120,IMG_STA_X+160-1,IMG_STA_Y+120+120-1);
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
			if (Flag_Edge)
				fimg[row/2][col/2]+=himg[row][col];
		}	
	
}

void Hough_WhiteTape(void){
	u16 color = 0;
	s16 row,col;
	u8 thresh=(Maxs+Mins)/WHITE_THRESH;//!!!!!!!饱和度的阈值
	u16 result_num=0,result_sum=0,col_thresh=0;
	u8 i,j,t;
	
		
	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X+160,IMG_STA_Y+120,IMG_STA_X+160+79,IMG_STA_Y+120+59);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}
	for (col=0;col<80;col++)
		col_sum[col]=0; //行累加清零
	for(row=T_RANGE;row<B_RANGE;row++) 
		for (col=L_RANGE;col<R_RANGE;col++)
		{
			if (simg[row*2][col*2]<thresh&&himg[row*2][col*2]>(Maxv+Minv)/BLACK_THRESH) //!!!!!!!!!!!!如果饱和度小 且亮度不小
			{
				bwimg[row][col+3-(u8)(row*0.05)]=31; //bwimg 黑白的图  白=31
				col_sum[col+3-(u8)(row*0.05)]++; //列求和
				bwimg[row][col-3+(u8)(row*0.05)]=31;
				col_sum[col-3+(u8)(row*0.05)]++;
				bwimg[row][col]=31;
				col_sum[col]++;
			}
		} //此时黑白图求得
		
	for (col=0;col<80;col++)
		col_thresh=max(col_thresh,col_sum[col]);
	col_thresh*=0.7; //!!!!!!!是否为白线的常数
	col_thresh=max(col_thresh,25);//长度不能小于25 (最大60)
		
	for (col=0;col<80;col++)
		if (col_sum[col]>col_thresh)
		{
			result_list[result_num]=col; //被选中的列的数组
			result_num++;//被选中的列的个数
			result_sum+=col;
		}//准备排序
	
	if (result_num<2)		
		Result_Angle=45;
	else if (result_num>MAX_CNT)//！！！！！！！
		Result_Angle=90; //出错啦！
	else
	{
		Result_Angle=0;
		for (i=0;i<result_num;i++)
			for (j=i+1;j<result_num;j++)
				if (result_list[i]<result_list[j])
				{
					t=result_list[i];
					result_list[i]=result_list[j];
					result_list[j]=t;
				}
		//Result_Dist=result_sum/result_num;
		Result_Dist=result_list[result_num/2]; //取中值
	}
	
	for(row=0;row<60;row++) 
		for (col=0;col<80;col++)
		{
			if (LCD_OUTPUT)
			{
				color=bwimg[row][col];
				if (bwimg[row][col]>0&&col_sum[col]>col_thresh)
					color=make_rgb565(31,0,0); //红线
				else	
					color=make_rgb565(color,2*color,color);
				
				if (Result_Angle!=45&&col==Result_Dist)
					color=make_rgb565(0,0,31); //蓝线
				
 				*(volatile u16 *) (LCD_DATA) = color;
			}
		}
		
	sprintf(lcd_str,"Max:%d Min:%d   ",Maxs,Mins);	
	LCD_WriteString16_ili9481(20, 280, GREEN,DARKBLUE,lcd_str);
	
}

void Sobel_Edge(void){
	u16 color = 0;
	s16 row,col,j;
	s8 flag_r,flag_b,flag_y;
	u8 wsize=2;
	
	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X+160,IMG_STA_Y+120,IMG_STA_X+160+79,IMG_STA_Y+120+59);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}
	
	Pnum=0;
	for(row=0;row<60;row++) 
		for (col=0;col<80;col++) //判断[row][col]是否为边缘点
		{
			flag_r=0;flag_b=0;flag_y=0;
			if (row<15||row>45||col<25||col>78) //！！！！！！看圆时的窗口 要保留5的余量
			{
				bwimg[row][col]=0;
			}
			else
			{
				//X
				//bwimg[row][col]=fimg[row-1][col-1]+2*fimg[row-1][col]+fimg[row-1][col+1]-fimg[row+1][col-1]-2*fimg[row+1][col]-fimg[row+1][col+1];
				//Y
				bwimg[row][col]=fimg[row-1][col-1]+2*fimg[row][col-1]+fimg[row+1][col-1]-fimg[row-1][col+1]-2*fimg[row][col+1]-fimg[row+1][col+1];
				
				if (abs(bwimg[row][col])>h_thresh) //!!!!!大于阈值 一般不管
				{
					flag_r=0; flag_b=0; flag_y=0; //特别注意！！！！！！！！！
					for (j=-wsize;j<=wsize;j+=2)//取邻域 j循环的是行
					{
						//X
						//color=fimg[row+wsize][col+j];
						color=fimg[row-j][col-wsize]; //fimg=himg -wsize意味着左边
						if (color==0)
							flag_r++;
						if (color==15*4)
							flag_y--;
						if (color==31*4)
							flag_b++;
						//X
						//color=fimg[row-wsize][col+j];
						color=fimg[row-j][col+wsize]; //+wsize意味着右边  ！！！！！注意加减号
	
						if (color==15*4)
							flag_y++;
					}
					
					if (flag_y>2&&(flag_r>2||flag_b>2))
					{
						bwimg[row][col]=31;
						if (Pnum==POINT_NUM-1)
							return;
						Pnum++;
						//X			
						//Px[Pnum]=col; Py[Pnum]=row;
						Px[Pnum]=row; Py[Pnum]=col; //后来找直线用 x y 个数 行列倒置 与之前兼容
					}
					else 
						bwimg[row][col]=0;								
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

void Filter(void){
	if (Flag_Edge)
	{
		Sobel_Edge();
		LINE_WIDTH=5;
	}
	else
	{
		Hough_WhiteTape();
		LINE_WIDTH=3;
	}
}
void GetLine_WhiteTape(){
	
	if (LCD_OUTPUT)
	{
		sprintf(lcd_str,"Angle:%d   ",Result_Angle);	
		LCD_WriteString16_ili9481(250, 280, GREEN,DARKBLUE,lcd_str);
		sprintf(lcd_str,"Dist:%d   ",Result_Dist);	
		LCD_WriteString16_ili9481(250, 300, GREEN,DARKBLUE,lcd_str);
	}
	
 	can1_send_data(Result_Angle*10,Result_Dist*10);
}

void GetLine_Edge(){
	s16 i,x1,y1,x2,y2,p1,p2,cnt;
	s32 num_in,xx,yy,xy,sx,sy;
	float k,b,dist;
	

	if (Pnum<10)
		return;

	Result_Num=0;
	for (cnt=0;cnt<TRY_TIMES;cnt++) //尝试次数TRY_TIMES
	{
		p1=rand()%Pnum; p2=rand()%Pnum;
		if (p1==p2) continue;
		x1=Px[p1]; y1=Py[p1];  x2=Px[p2]; y2=Py[p2];

		k=((float)(y2)-(float)(y1))/((float)(x2)-(float)(x1));
		b=(float)(y1)-k*(float)(x1);
				
		if(fabs(k)>0.25)
			continue;
		num_in=0;
		for (i=0;i<Pnum;i++)
		{
			dist=fabs((k*(float)(Px[i])-(float)(Py[i])+b)/sqrt(k*k+1));
			if (dist<LINE_WIDTH)
				num_in++;
		}

		if (num_in<10)
			continue;
				
		//------------------------------
		xx=0; yy=0; xy=0; sx=0; sy=0; num_in=0;
		for (i=0;i<Pnum;i++)
		{
			dist=fabs((k*(float)(Px[i])-(float)(Py[i])+b)/sqrt(k*k+1));
			if (dist>LINE_WIDTH)
				continue;
			xx=xx+Px[i]*Px[i];  yy=yy+Py[i]*Py[i]; xy=xy+Px[i]*Py[i];
			sx=sx+Px[i]; sy+=Py[i];
			num_in++;	
		}
		
		if (num_in<Result_Num)
			continue;
		
		k=(float)(num_in*xy-sx*sy)/(float)(num_in*xx-sx*sx);
		b=(float)(xx*sy-sx*xy)/(float)(num_in*xx-sx*sx);				

		dist=(k*(float)(40)-(float)(30)+b)/sqrt(k*k+1);

		Result_Angle=(s16)(-atan(k)/3.1415926*1800);
		Result_Dist=(s16)(dist*10);

	}
	
	if (abs(Angle_List[0]-Result_Angle)<30)
	{
		cnt=1;
		Transmit_Cnt++;
	}
	else 
	{
		cnt=0;
		Transmit_Cnt=0;
	}
	
	if (Transmit_Cnt>2)
	{
		Transmit_Cnt=0;
		cnt=0;
	}

	if (cnt==0)
	{
		for (i=0;i<LIST_LEN-1;i++)
			Angle_List[i]=Angle_List[i+1];
		Angle_List[0]=Result_Angle;
		
		for (i=0;i<LIST_LEN-1;i++)
			Dist_List[i]=Dist_List[i+1];
		Dist_List[0]=Result_Dist;
	}
	
		
	Result_Angle=0; Result_Dist=0;	
	for (i=0;i<LIST_LEN;i++)
	{
		Result_Angle+=Angle_List[i]; 
		Result_Dist+=Dist_List[i]; 
	}
	Result_Angle/=LIST_LEN;
	Result_Dist/=LIST_LEN;
	
	if (LCD_OUTPUT)
	{
		sprintf(lcd_str,"Angle:%d.%d   ",Result_Angle/10,abs(Result_Angle%10));	
		LCD_WriteString16_ili9481(250, 280, GREEN,DARKBLUE,lcd_str);
		sprintf(lcd_str,"Dist:%d.%d   ",Result_Dist/10,abs(Result_Dist%10));	
		LCD_WriteString16_ili9481(250, 300, GREEN,DARKBLUE,lcd_str);
	}

	
   can1_send_data(Result_Angle,Result_Dist);
	
	
}

void AxisTransform(){
	s16 i;
	float x_world,y_world,factor;
	
	for (i=1;i<=Pnum;i++)
	{
		x_world=	(float)Px[i]*mat_inv[0][0]+(float)Py[i]*mat_inv[0][1]+mat_inv[0][2];
		y_world=	(float)Px[i]*mat_inv[1][0]+(float)Py[i]*mat_inv[1][1]+mat_inv[1][2];
		factor=		(float)Px[i]*mat_inv[2][0]+(float)Py[i]*mat_inv[2][1]+mat_inv[2][2];
		x_world/=factor;
		y_world/=factor;
		Px[i]=(s16)(x_world)/3+80;
		Py[i]=(s16)(y_world)/3+60;
	}	
	
}

void GetLine(){
	if (Flag_WhiteTape)
		GetLine_WhiteTape();
	else
	{
//		AxisTransform();
		GetLine_Edge();	
	}
}

	
#endif

