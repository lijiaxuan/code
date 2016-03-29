#include "stm32f4xx.h"
#include "math.h"
#include "vision.h"
#include "can_database.h"
#include "MT9M111.h"
#include "ILI9481.h"
#include "ext_sram.h"
#include "stdio.h"

void MedianFilter(unsigned char *pImgSrc,unsigned char *pImgDst,int nLength);

#ifdef Tape

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
//#define EDGE_THRESH   50
//#define EDGE_THRESH   50/16
#define EDGE_THRESH   15
#define POINT_NUM 1000
#define TRY_TIMES 20
#define LINE_WIDTH 3

//#define process line
#define FIRSTLINE       20      //前瞻
#define SECONDLINE      110
#define THRIDLINE       200    //前轮

// u8 himg[120][160]={0};
// u8 simg[120][160]={0};

// //ÖÐÖµÂË²¨ºóµÄÊý¾Ý
// u8 fhimg[120][160]={0};
// u8 fsimg[120][160]={0};

extern u32 frames;

u8 rimg[9][320]={0};
u8 gimg[9][320]={0};
u8 bimg[9][320]={0};
u8 rowCount=0;

u8 fimg[9][320]={0};
s16 sobleimg[9][320]={0};
u16 bwimg[9][320]={0};
u32 Px_u[POINT_NUM],Py_u[POINT_NUM],Pnum_u,Result_Num_u;
u32 Px_d[POINT_NUM],Py_d[POINT_NUM],Pnum_d,Result_Num_d;
s16 Result_Flag=0;
s16 Result_Angle_u,Result_Dist_u;
s16 Result_Angle_d,Result_Dist_d;
s16 Result_Angle,Result_Dist,Result_Angle_Last=0,Result_Dist_Last=0;
s16 Dist[9];
u16 time_flag=0; 
u16 distError=0;
u16 ErrorLast=0;

u16 find=0;



void can1_send_data(s16 angle,s16 dis)
{
	MISO_CAMERA_DATA.i16_data[0] = dis;
	MISO_CAMERA_DATA.i16_data[1] = angle;
	MISO_CAMERA_DATA.i16_data[2] = 0xff;
	MISO_CAMERA_DATA.i16_data[3] = 0xff;	
//	Write_Database(S_CAMERA_ID);
}

u16 color = 0;

void Decompose(void){
    u16 i=0;
	u32 ReadAddr=0x0000;

	
	s16 r,g,b;	
	u16 row,col;
			
	for(row=0;row<9;row++) 
	{
		for (col=0;col<320;col++)
		{
			bwimg[row][col]=0;
			fimg[row][col]=0;
		}
	}

//显示图像
       
ReadAddr=0x0000+(frames%10)*(0x9600)*4;

//    ReadAddr=0x0000;
     if (LCD_OUTPUT&&( frames%7 == 0) )
	{
        
		LCD_SetBox(IMG_STA_X,IMG_STA_Y,IMG_STA_X+160-1,IMG_STA_Y+120-1);//box1
		*(volatile u16 *) (LCD_CMD)=(0x002c);
        
        for(row=0;row<240;row++) 
        {
            if(row%2) 
            {
                ReadAddr+=320*2;
                continue;
            }
            
            for (col=0;col<160;col++)
            {                                                                              
                *(volatile u16 *) (LCD_DATA) = *(vu16*)(Bank1_SRAM3_ADDR+ReadAddr);   
                ReadAddr+=4;
            }
        }
	}
    
//取B分量 3*3行 分别为前轮处 最远前瞻 中间 存储在bimg[9][320]中
//图片格式为行下标小的对应着远方
//每个像素在内存中对应的地址为 (y*320+x)*2+Bank1_SRAM3_ADDR
    

//     ReadAddr=FIRSTLINE*320*2+(frames%5)*0x9600*4;


    
    
// for(row=FIRSTLINE;row<FIRSTLINE+3;row++)
// {
//     for(col=0;col<320;col++)
//     {
//         color = *(vu16*)(Bank1_SRAM3_ADDR+ReadAddr);
//         ReadAddr+=2;
//         
//         r = get_r_value(color)*2;
//         g = get_g_value(color);
//         b = get_b_value(color)*2;

//         bimg[rowCount][col]=b;
//     }
//     rowCount++;
// }   
//     

rowCount=0;
ReadAddr=SECONDLINE*320*2+(frames%10)*0x9600*4;

if (LCD_OUTPUT && ( frames%7 == 0) )
{    
    LCD_SetBox(IMG_STA_X,IMG_STA_Y+130,IMG_STA_X+320-1,IMG_STA_Y+130+9-1);//box1
    *(volatile u16 *) (LCD_CMD)=(0x002c);
        
    for(row=SECONDLINE;row<SECONDLINE+9;row++)
    {
        for(col=0;col<320;col++)
        {
            color = *(vu16*)(Bank1_SRAM3_ADDR+ReadAddr);
            ReadAddr+=2;
            
            r = get_r_value(color)*2;
            g = get_g_value(color);
            b = get_b_value(color)*2;

            bimg[rowCount][col]=b;               
            color=make_rgb565(b/2,b,b/2);
            *(volatile u16 *) (LCD_DATA) = color;	
        }
        rowCount++;
    } 

}


//     ReadAddr=THRIDLINE*320*2+(frames%5)*0x9600*4;

// for(row=THRIDLINE;row<THRIDLINE+3;row++)
// {
//     for(col=0;col<320;col++)
//     {
//         color = *(vu16*)(Bank1_SRAM3_ADDR+ReadAddr);
//         ReadAddr+=2;
//         
//         r = get_r_value(color)*2;
//         g = get_g_value(color);
//         b = get_b_value(color)*2;

//         bimg[rowCount][col]=b;
//     }
//     rowCount++;
// }  
//  在需要sobel算子时候再中值滤波 还是全部中值滤波一次呢
//  选择中值滤波还是均值滤波呢
//  中值滤波
// for(row=0;row<9;row++)
// {
//     MedianFilter(&bimg[row][0],&fimg[row][0],320);
// }

}

//边缘检测

int i,j,n;
char str[20];

void Sobel(void){
	
	s16 row,col;
	s8 flag_y_u,flag_y_d;
	u8 wsize=4;

    CanTxMsg  TxM;    

	if (LCD_OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y+120,IMG_STA_X+320-1,IMG_STA_Y+120+9);//box3 9*320的框
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}

	Pnum_u=0; 	Pnum_d=0; 
	for(row=0;row<9;row++) 
    {
		for (col=0;col<320;col++)
		{
			if (row<=0||row>=7||col<=wsize||col>=320-wsize)
			{
				bwimg[row][col]=0;
			}
			else
			{
				//bwimg[row][col]=fimg[row-1][col-1]+2*fimg[row-1][col]+fimg[row-1][col+1]-fimg[row+1][col-1]-2*fimg[row+1][col]-fimg[row+1][col+1];
                sobleimg[row][col]=bimg[row-1][col-2]+2*bimg[row][col-2]+bimg[row+1][col-2]-bimg[row-1][col+2]-2*bimg[row][col+2]-bimg[row+1][col+2];
                //bwimg[row][col]=fimg[row][col]-fimg[row][col+1];
				if (sobleimg[row][col]>EDGE_THRESH)
				{
					bwimg[row][col]=15;
					Pnum_u++;
					if (Pnum_u>=POINT_NUM)
						return;
					Px_u[Pnum_u]=col; Py_u[Pnum_u]=row;
				}
				else if (sobleimg[row][col]<-EDGE_THRESH)
				{
					bwimg[row][col]=31;
					Pnum_d++;
					if (Pnum_d>=POINT_NUM)
						return;
					Px_d[Pnum_d]=col; Py_d[Pnum_d]=row;
				} else bwimg[row][col]=0;
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
    
            //利用白线有上升和下降来过滤掉一些边缘
            //利用摄像头找白线 肯定是 黄 白  黄 组合
            //soble算子得到的边缘肯定是 上升 固定距离 下降
            //在上升的右边缘一定距离找 下降 找到为白线 找不到 不是白线
            //找到的白线的位置为上升与下降的中点处
            //白线在图片上的位置映射到小车坐标下的相对位置
    
    //下降是红色  白线应该为上升 下降  蓝色 加 红色
    
   //找白线
static    u16 startPoint=0;
static    u16 stopPoint=0;
static    u8  startFlg=0;
 static   u16  loseCount=0;
    
    startPoint=0;
    stopPoint=0;
    startFlg=0;
    
		for(n=1;n<8;n++)  //第2到第8行 下标为1到7
        {    
            for(j=1; j<320; j++)//取第行做为关键行
            {
                if(bwimg[n][j]== 31)
                {
                        startPoint=j;
                        startFlg=1;
                    
                }
                else if(bwimg[n][j]== 15)
                {
                    if(startFlg==1)
                    {
                        stopPoint=j;
                        
                        if((stopPoint-startPoint)>20&& (stopPoint-startPoint)<70)//白线长度
                        {
                            
                            distError=(startPoint+stopPoint)/2;
                            
                            if(find==1)
                            {
                                if(abs(distError-ErrorLast)<100)//白线连续性
                                {
                                    find=1;
                                    loseCount=0;
                                    ErrorLast=distError;
                                    
                                    TxM.RTR = CAN_RTR_Data;
                                    TxM.IDE = CAN_Id_Standard;
                                    TxM.DLC = 8;
                                    TxM.StdId = 0x2;
                                    *(u16 *)TxM.Data = distError;
                                    CAN_Transmit(CAN1,&TxM);
                                    
                                    
                                    sprintf(str,"GOOD Dist_d:%d  %d->%d   ",distError,startPoint,stopPoint);
                                    LCD_WriteString16_ili9481(120, 8, GREEN,DARKBLUE,str);
                                    return;
                                }
                                
                            }
                            else
                            {
                                    find=1;
                                    loseCount=0;
                                    ErrorLast=distError;
                                    
                                    TxM.RTR = CAN_RTR_Data;
                                    TxM.IDE = CAN_Id_Standard;
                                    TxM.DLC = 8;
                                    TxM.StdId = 0x2;
                                    *(u16 *)TxM.Data = distError;
                                    CAN_Transmit(CAN1,&TxM);
                                    
                                    
                                    sprintf(str,"GOOD Dist_d:%d  %d->%d   ",distError,startPoint,stopPoint);	
                                    LCD_WriteString16_ili9481(120, 8, GREEN,DARKBLUE,str);
                                    return;
                            }

                        }
                    }
                    else
                    {
                        startFlg=0;
                    }
                }
            }
        }
        
        find=0;
        loseCount++;
        
//         if(ErrorLast<20)
//         {
//             TxM.RTR = CAN_RTR_Data;
//             TxM.IDE = CAN_Id_Standard;
//             TxM.DLC = 8;
//             TxM.StdId = 0x2;
//             *(u16 *)TxM.Data = 0;
//             CAN_Transmit(CAN1,&TxM);
//             
//             sprintf(str,"BAD    Dist_d:%d   ",0);	
//             LCD_WriteString16_ili9481(120, 8,RED,DARKBLUE,str);
//         }
//         else if(ErrorLast>300)
//         {
//             TxM.RTR = CAN_RTR_Data;
//             TxM.IDE = CAN_Id_Standard;
//             TxM.DLC = 8;
//             TxM.StdId = 0x2;
//             *(u16 *)TxM.Data = 320;
//             CAN_Transmit(CAN1,&TxM);
//             sprintf(str,"BAD    Dist_d:%d   ",320);	
//             LCD_WriteString16_ili9481(120, 8, RED,DARKBLUE,str);
//             
//         }
//         else
//         {
            TxM.RTR = CAN_RTR_Data;
            TxM.IDE = CAN_Id_Standard;
            TxM.DLC = 8;
            TxM.StdId = 0x2;
            *(u16 *)TxM.Data = ErrorLast;
            CAN_Transmit(CAN1,&TxM);
            LCD_WriteString16_ili9481(120, 8, RED,DARKBLUE,"BAD                          ");
            LCD_WriteString16_ili9481(120, 8, RED,DARKBLUE,str);
//        }
        
        if(loseCount>5)
        {
            TxM.RTR = CAN_RTR_Data;
            TxM.IDE = CAN_Id_Standard;
            TxM.DLC = 8;
            TxM.StdId = 0x2;
            *(u16 *)TxM.Data = 888;
            CAN_Transmit(CAN1,&TxM);
            
//             loseCount=0;
            LCD_WriteString16_ili9481(120, 8, RED,DARKBLUE,"BAD                          ");
            //关闭寻线模式，重启
        }   
        else
        {         
            TxM.RTR = CAN_RTR_Data;
                TxM.IDE = CAN_Id_Standard;
                TxM.DLC = 8;
                TxM.StdId = 0x2;
                *(u16 *)TxM.Data = ErrorLast;
                CAN_Transmit(CAN1,&TxM);
                LCD_WriteString16_ili9481(120, 8, RED,DARKBLUE,"BAD                          ");
                LCD_WriteString16_ili9481(120, 8, RED,DARKBLUE,str);
        }
            //丢线有两种情况，一种偶然的丢线，一种走到极限，不可能找到白线
            //丢线第一反应当成偶然丢线，
            //丢线计数到5，认为不是偶然丢线
}


//用二次曲线拟合直线
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
    
    ReadAddr=0x0000+(frames%5)*0x9600*4;
    
    if (LCD_OUTPUT && ( frames%4 == 0) )
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y,IMG_STA_X+320-1,IMG_STA_Y+240-1);//box1
		*(volatile u16 *) (LCD_CMD)=(0x002c);
        
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



}

void sendData(void)
{
    
}

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

	
#endif

