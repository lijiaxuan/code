#include "stm32f4xx.h"
#include "math.h"
#include "vison.h"
#include "can_database.h"
#include "MT9M111.h"
#include "ILI9481.h"
#include "ext_sram.h"
#include "stdio.h"

#ifdef Detect_Circle

////////////////////////////////////
#define get_b_value(rgb) ((unsigned char)(rgb&0x1f))
#define get_g_value(rgb) ((unsigned char)((rgb>>5)&0x3f))
#define get_r_value(rgb) ((unsigned char)((rgb>>11)&0x1f))
#define make_rgb565(r,g,b) ((r<<11)|(g<<5)|(b))
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)
//////////////////////////////////////

#define R_CIRCLE 45
#define THRESH 2

#define LCD_OUTPUT 1
#define LCD_DEBUG 0

#define Mult_10 1

#define ROWS 120
#define COLS 160
#define BFS_SIZE 19200
#define POINT_SIZE 2000

#define SCALE_FACTOR 1//0.7

//HSV_Decompose
s16 himg[ROWS][COLS]={0};
u8 Field_Color=0;
float Center_Row,Center_Col;

//FloodFill
u8 	Flag_FF[ROWS][COLS];
u8 	QRow[BFS_SIZE],QCol[BFS_SIZE];
s16 QHead,QTail; 
s8 dir_row[8]={-1,-1,-1,0 ,1 ,1 ,1 ,0 };
s8 dir_col[8]={-1,0 ,1 ,1 ,1 ,0 ,-1,-1};

//AxisTransform
float mat_inv[3][3]={{5.8719315839,-0.1151745630,-472.9051605697},{-0.0131421586,7.3140866707,-418.7892691836},{-0.0000562186,0.0113870347,2.0588986315}};
float x_cam[POINT_SIZE],y_cam[POINT_SIZE];

//GetCircle
s16 Px[POINT_SIZE],Py[POINT_SIZE];
s16 x_in[POINT_SIZE],y_in[POINT_SIZE];
s32	Pnum,Result_Num;
u16 Result_List[POINT_SIZE];
s16 Result_Flag=0;

extern u8 Flag_BlueGround;
extern u8 Flag_RedGround;

extern void LCD_DrawPixel(u16 x, u16 y,u16 color);

u16 time_flag=0; 

void can1_send_data(s16 x,s16 y)
{
	MISO_CAMERA_DATA.i16_data[0] = x;
	MISO_CAMERA_DATA.i16_data[1] = y;
	MISO_CAMERA_DATA.i16_data[2] = 0xff;
	MISO_CAMERA_DATA.i16_data[3] = 0xff;	
	Write_Database(S_CAMERA_ID);
}

void Draw_Circle(u16 x0,u16 y0,u8 r,u16 cl){
	s16 x,y;
	for (x=0;x<=r;x++)
	{
 		y=(s16)sqrt(r*r-x*x);
		LCD_DrawPixel(x0-x,y0-y,cl);
		LCD_DrawPixel(x0+x,y0-y,cl);
		LCD_DrawPixel(x0-x,y0+y,cl);
		LCD_DrawPixel(x0+x,y0+y,cl);
	}
}


void HSV_Decompose(void){
	u32 ReadAddr=0x0000;

	u16 color = 0;
	s16 r,g,b,h=0,s,v;	
	u16 row,col;
	u16 val_r,val_b,val_y,minval;
		
	u16 Cnt_R,Cnt_B;
	float Center_R_Row,Center_B_Row,Center_R_Col,Center_B_Col;
	
	
	for(row=0;row<ROWS;row++) 
		for (col=0;col<COLS;col++)
			Flag_FF[row][col]=0;
	Cnt_R=0; Center_R_Row=0; Center_R_Col=0;
	Cnt_B=0; Center_B_Row=0; Center_B_Col=0;
	
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
				h/=6; //0~31
								
				h-=10;				
				if (h<0)
					h=31+h;

				val_r=abs(h-22);
				val_b=abs(h-11);
				val_y=abs(h-27);
				minval=min(min(val_r,val_b),val_y);
				
				if (s<3)
					h=15;
				else
				{
					if (minval==val_y)
						h=15;
					if (minval==val_r)
					{
						h=0;
						if (row>20&&row<220&&col>40&&col<280)if (row>20&&row<220&&col>40&&col<280)
						{
							Cnt_R++;
							Center_R_Row+=row/2;
							Center_R_Col+=col/2;
						}
					}
					if (minval==val_b)
					{
						h=31;
						if (row>20&&row<220&&col>40&&col<280)
						{
							Cnt_B++;
							Center_B_Row+=row/2;
							Center_B_Col+=col/2;
						}
					}
				}				
				himg[row/2][col/2]=h;									
			}
		}	
	if (Flag_RedGround)
	//if (Cnt_R>Cnt_B)
	{
		Field_Color=0;
		Center_Row=Center_R_Row/Cnt_R;
		Center_Col=Center_R_Col/Cnt_R;
	}
	else if (Flag_BlueGround)
	{
		Field_Color=31;
		Center_Row=Center_B_Row/Cnt_B;
		Center_Col=Center_B_Col/Cnt_B;
	}
	else 
	{
		Center_Row=-1;
		Center_Col=-1;
	}		

}


void bfs(s16 row,s16 col){
	s16 row0,col0,i,code=1,cnt;

	QHead=0; QTail=1; 
	QRow[1]=row; QCol[1]=col; 
	Flag_FF[row][col]=code;

	while (QHead!=QTail)
	{
		QHead++;
		row0=QRow[QHead]; 	col0=QCol[QHead];
		cnt=0;
		for (i=0;i<8;i++)
		{
			row=row0+dir_row[i];
			col=col0+dir_col[i];
			if (row<0||col<0||row>=ROWS||col>=COLS)
			{
				cnt++;
				continue;
			}
			if (himg[row][col]==Field_Color)
				cnt++;
			if (Flag_FF[row][col]==0&&himg[row][col]==Field_Color)
			{
				Flag_FF[row][col]=code;
				QTail++;
				if (QTail>=BFS_SIZE)
					return;
		
				QRow[QTail]=row;
				QCol[QTail]=col;
			}
		}
		if (cnt<=7)
		{
			x_cam[Pnum]=col0;
			y_cam[Pnum]=row0;
			Pnum++;
			if (Pnum>POINT_SIZE)
				return;
		}

	}
}


void FloodFill(){
	u16 color = 0;
	s16 row,col;
	
	if (Center_Row<0||Center_Col<0)
		return;
	
 	Pnum=0;
 	bfs((s16)Center_Row,(s16)Center_Col);
	
// 	for (row=-2;row<=2;row++)
// 		for (col=-2;col<=2;col++)
// 			LCD_DrawPixel(IMG_STA_X+Center_Col+row,IMG_STA_Y+120+Center_Row+col,GREEN);

	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X+160,IMG_STA_Y,IMG_STA_X+160+159,IMG_STA_Y+119);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}
	
	for(row=0;row<ROWS;row++) 
		for (col=0;col<COLS;col++)
		{
			if (LCD_OUTPUT)
			{
				color=himg[row][col]/1;
				if (Flag_FF[row][col])
					color=(Field_Color==31)?(BLUE):(RED);
				else
					color=make_rgb565(color,color*63/31,color);
				*(volatile u16 *) (LCD_DATA) = color;			
			}
		}		
}

void AxisTransform(){
	s16 i;
	float x_world,y_world,factor;
	
	if (Center_Row<0||Center_Col<0)
		return;
	
	for (i=0;i<Pnum;i++)
	{
		x_world=	x_cam[i]*mat_inv[0][0]+y_cam[i]*mat_inv[0][1]+mat_inv[0][2];
		y_world=	x_cam[i]*mat_inv[1][0]+y_cam[i]*mat_inv[1][1]+mat_inv[1][2];
		factor=		x_cam[i]*mat_inv[2][0]+y_cam[i]*mat_inv[2][1]+mat_inv[2][2];
		x_world/=factor;
		y_world/=factor;
		Px[i+1]=(s16)(x_world)/3+80;
		Py[i+1]=(s16)(y_world)/3+60;
// 		Px[i+1]=(s16)x_cam[i];
// 		Py[i+1]=(s16)y_cam[i];
	}
	
}

char str[20];	
extern void Draw_Circle_ili9481(u16 x0,u16 y0,u8 r,u8 Si,u16 cl);
	
void GetCircle(){
	s16 i,j,x1,y1,x2,y2,p1,p2,cnt,num_in;
	float x_mid,y_mid,k,rad,tdist,dist,x_center,y_center;
	float AA,AB,AC,BB,BC,CC,A,B,C,x_fit,y_fit,r_fit;
	s16 Result_X=0,Result_Y=0;
	s16 Result_X_Last=0,Result_Y_Last=0;

	if (Center_Row<0||Center_Col<0)
		return;
	
	
	FillSolidRect(IMG_STA_X,IMG_STA_Y+120,160,120, BLACK);//Graph Window
  
	for (i=0;i<Pnum;i++)
		LCD_DrawPixel(IMG_STA_X+Px[i],IMG_STA_Y+120+Py[i],WHITE);

	if (Pnum<20||Pnum>POINT_SIZE)
		return;
	Result_Num=0;

	for (cnt=0;cnt<50;cnt++)
	{
		p1=rand()%Pnum; p2=rand()%Pnum;
		if (p1==p2) continue;
		x1=Px[p1]; y1=Py[p1];  x2=Px[p2]; y2=Py[p2];

		x_mid=(float)(x1+x2)/2.0; y_mid=(y1+y2)/2.0;
		k=-(float)(x2-x1)/(float)(y2-y1);
		//b=(float)y_mid-k*(float)x_mid;
		
		rad=atan(k); if (sin(rad)<0) rad=3.1415926+rad; 
		tdist=sqrt(pow(x_mid-x1,2)+pow(y_mid-y1,2));
		if (tdist>R_CIRCLE)
			continue;
		x_center=x_mid+cos(rad)*sqrt(pow(R_CIRCLE,2)-pow(tdist,2)); //edited;
		y_center=y_mid+sin(rad)*sqrt(pow(R_CIRCLE,2)-pow(tdist,2));
		
		num_in=0;
		for (i=0;i<Pnum;i++)
		{
			dist=fabs(sqrt((Px[i]-x_center)*(Px[i]-x_center)+(Py[i]-y_center)*(Py[i]-y_center))-R_CIRCLE);
			if (dist<THRESH)
			{
				x_in[num_in]=Px[i];
				y_in[num_in]=Py[i];
				num_in++;
			}
		}

		if (num_in<10)
			continue;
/*
		AA=0; AB=0; AC=0; BB=0; BC=0; CC=0; r_fit=0;
		for (i=1;i<num_in;i++)
		{
			A=2*(x_in[i]-x_in[i-1]);
			B=2*(y_in[i]-y_in[i-1]);
			C=x_in[i]*x_in[i]+y_in[i]*y_in[i]-x_in[i-1]*x_in[i-1]-y_in[i-1]*y_in[i-1];

			AA+=A*A; AB+=A*B; AC+=A*C; BB+=B*B; BC+=B*C; CC+=C*C;
			
		}
		x_fit=(BB*AC-BC*AB)/(AA*BB-AB*AB);
		y_fit=(AA*BC-AC*AB)/(AA*BB-AB*AB);
				
		for (i=0;i<num_in;i++)
			r_fit+=sqrt((x_in[i]-x_fit)*(x_in[i]-x_fit)+(y_in[i]-y_fit)*(y_in[i]-y_fit));
		r_fit/=num_in;

		if (fabs(r_fit-R_CIRCLE)>THRESH)
		   continue;
		r_fit=R_CIRCLE; x_fit=x_center; y_fit=y_center;


		num_in=0;
		for (i=0;i<Pnum;i++)
		{
			dist=fabs(sqrt((Px[i]-x_fit)*(Px[i]-x_fit)+(Py[i]-y_fit)*(Py[i]-y_fit))-R_CIRCLE);
			if (dist<THRESH)
				num_in++;
		}
*/
		
		if (num_in<Result_Num)
			continue;
		
		Result_Num=num_in;		
		Result_X=(s16)(x_center*10);
		Result_Y=(s16)(y_center*10);
// 		Result_X=(s16)(x_fit*10);
// 		Result_Y=(s16)(y_fit*10);
	
	}
	
	if (Result_Num<10)
		return;

	Draw_Circle(IMG_STA_X+Result_X/10,IMG_STA_Y+120+Result_Y/10,R_CIRCLE,GREEN);

	sprintf(str,"X:%d.%d",Result_X/10,Result_X%10);	
	LCD_WriteString16_ili9481(8, 280, GREEN,DARKBLUE,str);
	sprintf(str,"Y:%d.%d",Result_Y/10,Result_Y%10);	
	LCD_WriteString16_ili9481(8, 300, GREEN,DARKBLUE,str);		

//  	if (abs(Result_X-Result_X_Last)<20&&abs(Result_Y-Result_Y_Last)<20)
		can1_send_data(Result_X,Result_Y);	
	
	Result_X_Last=Result_X;
	Result_Y_Last=Result_Y;

}

#endif
