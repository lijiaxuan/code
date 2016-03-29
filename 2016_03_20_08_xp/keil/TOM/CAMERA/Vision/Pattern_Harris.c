#include "stm32f4xx.h"
#include "math.h"
#include "vison.h"
#include "can_database.h"
#include "MT9M111.h"
#include "ILI9481.h"
#include "ext_sram.h"
#include "stdio.h"

#ifdef Detect_Pattern_Harris

////////////////////////////////////
#define get_b_value(rgb) ((unsigned char)(rgb&0x1f))
#define get_g_value(rgb) ((unsigned char)((rgb>>5)&0x3f))
#define get_r_value(rgb) ((unsigned char)((rgb>>11)&0x1f))
#define make_rgb565(r,g,b) ((r<<11)|(g<<5)|(b))
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)
#define SIZE_H 240
#define SIZE_W 320
#define Himg Bank1_SRAM3_ADDR+SIZE_H*SIZE_W*2
//////////////////////////////////////


//add "--diag_suppress 1035" to Flash -> Configure Flash Tools -> C/C++  -> Misc Controls 
//to suppress warning #1035 (implicit double conversion)

#define SUCCESS 0
#define FAIL 		1

// #define BLACK_THRESH 	38
#define ROWS 					120
#define COLS 					160

#define BW_BLOCK_CNT	1

#define PATTERN_X 		25.0
#define PATTERN_Y 		25.0
#define PATTERN_ROW 	13
#define PATTERN_COL 	19
#define QR_THRESH 		5

#define BFS_SIZE			300

#define HARRIS_ARRAYSIZE 			13 //Must be odd  
#define HARRIS_FACTOR 				0.06
#define HARRIS_FACTOR_RESULT 	1
#define HARRIS_WSIZE 					2

#define MAT_SIZE		30
#define MAT_EPS			1e-12
#define EIG_MAX_ITER	10
#define EIG_EPS				1e-3

#define DEBUG 			0
#define OUTPUT 			1
#define GLOBAL_AXIS 0

typedef u8 STATE;
typedef struct
{
	s16 rows,cols;
	float val[MAT_SIZE][MAT_SIZE];
}MAT;


//RGB2BW
u8 vimg[ROWS][COLS];
u8 Block_Max[BW_BLOCK_CNT][BW_BLOCK_CNT],Block_Min[BW_BLOCK_CNT][BW_BLOCK_CNT];

//Find_QR  (BFS)
u8 	Flag_FF[ROWS][COLS];
s16 QRow[BFS_SIZE],QCol[BFS_SIZE];
s16 QHead,QTail; 
s16 dir_row[4]={0,1,0,-1};
s16 dir_col[4]={-1,0,1,0};
s16 Result_Num=0;
float Result_Row[10],Result_Col[10];

//MyHarris
float Harris_I[HARRIS_ARRAYSIZE][HARRIS_ARRAYSIZE];
float Ixx[HARRIS_ARRAYSIZE][HARRIS_ARRAYSIZE],Iyy[HARRIS_ARRAYSIZE][HARRIS_ARRAYSIZE],Ixy[HARRIS_ARRAYSIZE][HARRIS_ARRAYSIZE];
float Harris_Result[HARRIS_ARRAYSIZE][HARRIS_ARRAYSIZE];
float Harris_Exp[HARRIS_ARRAYSIZE][HARRIS_ARRAYSIZE];

//Calc_Position
float pos_row[PATTERN_ROW][PATTERN_COL],pos_col[PATTERN_ROW][PATTERN_COL];

//Find_KeyPoint
u8 pos_list[20][2]={{2,3},{2,5},{4,1},{4,3},{4,5},{4,7},{6,1},{6,3},{6,5},{6,7},{8,1},{8,3},{8,5},{8,7}};
u8 pos_list_len=14;
u16 Frames=0;
MAT Mat_M;
MAT Mat_m;
MAT Mat_L={28,9,{{-12.50000,37.50000,1.00000,0.00000,0.00000,0.00000,400.00000,-1200.00000,-32.00000},{0.00000,0.00000,0.00000,-12.50000,37.50000,1.00000,1062.50000,-3187.50000,-85.00000},{12.50000,37.50000,1.00000,0.00000,0.00000,0.00000,-387.50000,-1162.50000,-31.00000},{0.00000,0.00000,0.00000,12.50000,37.50000,1.00000,-900.00000,-2700.00000,-72.00000},{-37.50000,12.50000,1.00000,0.00000,0.00000,0.00000,1987.50000,-662.50000,-53.00000},{0.00000,0.00000,0.00000,-37.50000,12.50000,1.00000,3675.00000,-1225.00000,-98.00000},{-12.50000,12.50000,1.00000,0.00000,0.00000,0.00000,662.50000,-662.50000,-53.00000},{0.00000,0.00000,0.00000,-12.50000,12.50000,1.00000,1050.00000,-1050.00000,-84.00000},{12.50000,12.50000,1.00000,0.00000,0.00000,0.00000,-650.00000,-650.00000,-52.00000},{0.00000,0.00000,0.00000,12.50000,12.50000,1.00000,-875.00000,-875.00000,-70.00000},{37.50000,12.50000,1.00000,0.00000,0.00000,0.00000,-1912.50000,-637.50000,-51.00000},{0.00000,0.00000,0.00000,37.50000,12.50000,1.00000,-2137.50000,-712.50000,-57.00000},{-37.50000,-12.50000,1.00000,0.00000,0.00000,0.00000,2775.00000,925.00000,-74.00000},{0.00000,0.00000,0.00000,-37.50000,-12.50000,1.00000,3600.00000,1200.00000,-96.00000},{-12.50000,-12.50000,1.00000,0.00000,0.00000,0.00000,912.50000,912.50000,-73.00000},{0.00000,0.00000,0.00000,-12.50000,-12.50000,1.00000,1037.50000,1037.50000,-83.00000},{12.50000,-12.50000,1.00000,0.00000,0.00000,0.00000,-900.00000,900.00000,-72.00000},{0.00000,0.00000,0.00000,12.50000,-12.50000,1.00000,-862.50000,862.50000,-69.00000},{37.50000,-12.50000,1.00000,0.00000,0.00000,0.00000,-2662.50000,887.50000,-71.00000},{0.00000,0.00000,0.00000,37.50000,-12.50000,1.00000,-2100.00000,700.00000,-56.00000},{-37.50000,-37.50000,1.00000,0.00000,0.00000,0.00000,3562.50000,3562.50000,-95.00000},{0.00000,0.00000,0.00000,-37.50000,-37.50000,1.00000,3562.50000,3562.50000,-95.00000},{-12.50000,-37.50000,1.00000,0.00000,0.00000,0.00000,1175.00000,3525.00000,-94.00000},{0.00000,0.00000,0.00000,-12.50000,-37.50000,1.00000,1012.50000,3037.50000,-81.00000},{12.50000,-37.50000,1.00000,0.00000,0.00000,0.00000,-1150.00000,3450.00000,-92.00000},{0.00000,0.00000,0.00000,12.50000,-37.50000,1.00000,-850.00000,2550.00000,-68.00000},{37.50000,-37.50000,1.00000,0.00000,0.00000,0.00000,-3412.50000,3412.50000,-91.00000},{0.00000,0.00000,0.00000,37.50000,-37.50000,1.00000,-2062.50000,2062.50000,-55.00000}}};

//Extract_Extrinsic
MAT Mat_A= {3,3,{{228.92947,0.00000,84.82093},{0.00000,232.11298,56.08162},{0.00000,0.00000,1.00000}}}; //intrinsic param
MAT Mat_LT,Mat_LTL,Mat_H,Mat_invA,Mat_Temp,Mat_R;
MAT Vect_Eig;//={9,1,{{-0.0000232748},{-0.0084040845},{0.6324579683},{-0.0050383689},{0.0003905946},{0.7744663058},{0.0000055669},{-0.0000017406},{0.0101337290}}};
MAT	Vect_T,Vect_H1,Vect_H2,Vect_H3;//,Vect_R1,Vect_R2,Vect_R3;
MAT invM,V_last;



//LCD && UART
extern void LCD_DrawPixel(u16 x, u16 y,u16 color);
extern void Com2_Puts(char * str);
char str[255];
u32 delay;

//----------------- 
s16 max4(s16 a,s16 b,s16 c,s16 d){
	s16 result=a;
	result=max(result,b);
	result=max(result,c);
	result=max(result,d);
	return result;
}

s16 min4(s16 a,s16 b,s16 c,s16 d){
	s16 result=a;
	result=min(result,b);
	result=min(result,c);
	result=min(result,d);
	return result;
}
s16 get_bw(s16 row,s16 col){
	s16 brow,bcol,thresh;
	
	brow=row/(ROWS/BW_BLOCK_CNT);
	bcol=col/(COLS/BW_BLOCK_CNT);
	if (Block_Max[brow][bcol]-Block_Min[brow][bcol]<5)
		thresh=31;
	else
		thresh=(2*Block_Max[brow][bcol]+Block_Min[brow][bcol])/3;
	return (vimg[row][col]>thresh?1:0);
	
}
STATE myHarris(s16* result_row,s16 * result_col){
	float ix,iy,ixx,iyy,ixy,C;
	float pos_row=0,pos_col=0;
	s16 row,col,trow,tcol,count=0,n,m,val;
	u16 color;

//  	*result_row=(HARRIS_ARRAYSIZE-1)/2;
//  	*result_col=(HARRIS_ARRAYSIZE-1)/2;
// 	return SUCCESS;
  
	
 	if (OUTPUT)
	{
		LCD_SetBox(IMG_STA_X+COLS,IMG_STA_Y+ROWS,IMG_STA_X+COLS+HARRIS_ARRAYSIZE-1,IMG_STA_Y+ROWS+HARRIS_ARRAYSIZE-1);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
		
		for (row=0;row<HARRIS_ARRAYSIZE;row++)
			for (col=0;col<HARRIS_ARRAYSIZE;col++)
			{
				color=make_rgb565((u8)(Harris_I[row][col]*31+0.5),(u8)(Harris_I[row][col]*63+0.5),(u8)(Harris_I[row][col]*31+0.5));
				*(volatile u16 *) (LCD_DATA) = color;
			}
	}	
	
	
	n=HARRIS_ARRAYSIZE; m=HARRIS_ARRAYSIZE;

 	for (row=2;row<n-2;row++)
 		for (col=2;col<m-2;col++)
 		{
 			ix=2*Harris_I[row][col+2]+1*Harris_I[row][col+1]-1*Harris_I[row][col-1]-2*Harris_I[row][col-2];
 			iy=2*Harris_I[row+2][col]+1*Harris_I[row+1][col]-1*Harris_I[row-1][col]-2*Harris_I[row-2][col];
 			Ixx[row][col]=ix*ix; Iyy[row][col]=iy*iy; Ixy[row][col]=ix*iy;
 		}

	for (row=HARRIS_WSIZE;row<n-HARRIS_WSIZE;row++)
		for (col=HARRIS_WSIZE;col<m-HARRIS_WSIZE;col++)
		{
			ixx=0; iyy=0; ixy=0;

			for (trow=-HARRIS_WSIZE;trow<=HARRIS_WSIZE;trow++)
				for (tcol=-HARRIS_WSIZE;tcol<=HARRIS_WSIZE;tcol++)
				{
					//C=exp((float)(-trow*trow-tcol*tcol)/2.0);
					C=Harris_Exp[abs(trow)][abs(tcol)];
					ixx+=C*Ixx[row+trow][col+tcol];
					ixy+=C*Ixy[row+trow][col+tcol];
					iyy+=C*Iyy[row+trow][col+tcol];					
				}

 			Harris_Result[row][col]=(ixx*iyy-ixy*ixy)-HARRIS_FACTOR*(ixx+iyy)*(ixx+iyy);
 			if (Harris_Result[row][col]>HARRIS_FACTOR_RESULT)
 			{
 				count++;
 				pos_row+=row;
 				pos_col+=col;
			}
		}
		
	if (OUTPUT)
	{
		LCD_SetBox(IMG_STA_X+COLS+HARRIS_ARRAYSIZE,IMG_STA_Y+ROWS,IMG_STA_X+COLS+2*HARRIS_ARRAYSIZE-1,IMG_STA_Y+ROWS+HARRIS_ARRAYSIZE-1);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
		
		for (row=0;row<HARRIS_ARRAYSIZE;row++)
			for (col=0;col<HARRIS_ARRAYSIZE;col++)
			{
				val=(s16)(31.0*Harris_Result[row][col]/HARRIS_FACTOR_RESULT+0.5);
				if (val>31)
					val=31;
				if (val<0)
					val=0;				
				color=make_rgb565(val,val*63/31,val);
				*(volatile u16 *) (LCD_DATA) = color;
			}
	}	
		
	*result_row=(s16)(pos_row/count+0.5);
	*result_col=(s16)(pos_col/count+0.5);

	return SUCCESS;

}




STATE bfs(s16 row,s16 col,s16 code){
	s16 row0,col0,i,brow,bcol,thresh;
	float avrow=0,avcol=0;

	QHead=0; QTail=1; 
	QRow[1]=row; QCol[1]=col; 
	Flag_FF[row][col]=code;

	while (QHead!=QTail)
	{
		QHead++;
		if (QHead>BFS_SIZE)
			return FAIL;
		row0=QRow[QHead]; avrow+=row0;
		col0=QCol[QHead]; avcol+=col0;
		for (i=0;i<4;i++)
		{
			row=row0+dir_row[i];
			col=col0+dir_col[i];
			if (row<0||col<0||row>=ROWS||col>=COLS)
				continue;

 			brow=row/(ROWS/BW_BLOCK_CNT);
 			bcol=col/(COLS/BW_BLOCK_CNT);
			if (Block_Max[brow][bcol]-Block_Min[brow][bcol]<10)
				thresh=31;
			else
				thresh=(Block_Max[brow][bcol]+Block_Min[brow][bcol])/2;

			if (Flag_FF[row][col]==0&&vimg[row][col]<thresh)
			{
				Flag_FF[row][col]=code;
				QTail++;
				QRow[QTail]=row;
				QCol[QTail]=col;
			}
		}
	}
	Result_Row[code]=avrow/QTail;
	Result_Col[code]=avcol/QTail;
	if (QTail>10)
		return SUCCESS;
	else return FAIL;
}



STATE calc_position(s16 i,s16 j,float *row,float *col){
	float row_u,col_u,row_d,col_d,row_l,col_l,row_r,col_r;
	float k1,k2,b1,b2;

	row_u=pos_row[0][j];	col_u=pos_col[0][j];
	row_d=pos_row[11][j];	col_d=pos_col[11][j];
	row_l=pos_row[i][0];	col_l=pos_col[i][0];
	row_r=pos_row[i][8];	col_r=pos_col[i][8];

	if (col_u-col_d==0)
		k1=1e10;
	else
		k1=(row_u-row_d)/(col_u-col_d); 
	b1=row_u-col_u*k1;
	if (col_r-col_l==0)
		k2=1e10;
	else
		k2=(row_r-row_l)/(col_r-col_l); b2=row_r-col_r*k2;

	*col=(b2-b1)/(k1-k2);
	*row=k1*(*col)+b1;
	return SUCCESS;
}

//-----------------
STATE MAT_output(MAT *M,char name[]){
	s16 row,col;
	
	sprintf(str,"%s=[",name);
	Com2_Puts(str);
	for (row=0;row<M->rows;row++)
	{
		for (col=0;col<M->cols;col++)
		{
				sprintf(str,"%.10f",M->val[row][col]);
				Com2_Puts(str);
				if (col!=M->cols-1)
					Com2_Puts(",");
		}
		if (row!=M->rows-1)
				Com2_Puts(";\r\n\t");		
	}
	Com2_Puts("];\r\n\r\n");
	return SUCCESS;
}
float MAT_Norm(MAT *M){
	double result=0;
	s16 i,j;
	for (i=0;i<M->rows;i++)
		for (j=0;j<M->cols;j++)
			result+=(double)(M->val[i][j])*(double)(M->val[i][j]);
	result=sqrt(result);
	return result;
}
STATE MAT_VertCat(MAT *M1,MAT *M2,MAT* R){
	s16 i,j,n;
	if (M1->cols!=M2->cols)
		return FAIL;
	
	R->rows=M1->rows; R->cols=M1->cols;
	for (i=0;i<M1->rows;i++)
		for (j=0;j<M1->cols;j++)
			R->val[i][j]=M1->val[i][j];	
	
	n=R->rows;
	for (i=0;i<M2->rows;i++)
	{
		n++; R->rows=n;
		for (j=0;j<M2->cols;j++)
			R->val[n][j]=M2->val[i][j];
	}
	return SUCCESS;
}
STATE MAT_HorizCat(MAT *M1,MAT *M2,MAT* R){
	s16 i,j,n;
	if (M1->rows!=M2->rows)
		return FAIL;

	R->rows=M1->rows; R->cols=M1->cols;
	for (i=0;i<M1->rows;i++)
		for (j=0;j<M1->cols;j++)
			R->val[i][j]=M1->val[i][j];	

	n=R->cols;
	for (j=0;j<M2->cols;j++)
	{
		n++; R->cols=n;
		for (i=0;i<M2->rows;i++)
			R->val[i][n]=M2->val[i][j];
	}
	return SUCCESS;
}
STATE MAT_Transpose(MAT *M,MAT* R){
	s16 i,j;
	if (M->cols==0||M->rows==0)
	{
		R->rows=0;
		R->cols=0;
		return FAIL;
	}
	R->rows=M->cols;
	R->cols=M->rows;
	
	for (i=0;i<M->rows;i++)
		for (j=0;j<M->cols;j++)
			R->val[j][i]=M->val[i][j];
	return SUCCESS;
}
STATE MAT_Multiply(MAT *M1,MAT *M2,MAT* R){
	s16 i,j,k;

	if (M1->cols!=M2->rows)
	{
		R->rows=0;
		R->cols=0;
		return FAIL;
	}
	
	
	R->rows=M1->rows;
	R->cols=M2->cols;

	for (i=0;i<R->rows;i++)
		for (j=0;j<R->cols;j++)
		{	
			R->val[i][j]=0;
 			for (k=0;k<M1->cols;k++)
 				R->val[i][j]+=M1->val[i][k]*M2->val[k][j];
		}
	return SUCCESS;
}
STATE MAT_Inverse(MAT *M,MAT* R){
	s16 i,j,k,n,flag;
	double t;
	
	if (M->rows!=M->cols||M->cols==0||M->rows==0)
	{
		R->rows=0;
		R->cols=0;
		return FAIL;
	}
	n=M->rows;
	R->rows=n;
	R->cols=n;
	
	Mat_Temp.rows=n; Mat_Temp.cols=n;
	
	//Method : Gauss-Jordan
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			R->val[i][j]=0;
			Mat_Temp.val[i][j]=M->val[i][j];
		}
		R->val[i][i]=1;
	}

	M=&Mat_Temp;

	//Row Reduction
	for(i=0;i<n;i++)
	{
		//Swap
		flag=0; t=0;
		for(j=i;j<n;j++)
			if (fabs(M->val[j][i])>t)
			{
				flag=j; 
				t=fabs(M->val[j][i]);
			}
		if (t<MAT_EPS) return FAIL;
		for(k=0;k<n;k++)
		{	
			t=M->val[i][k]; M->val[i][k]=M->val[flag][k];		M->val[flag][k]=t; 
			t=R->val[i][k];	R->val[i][k]=R->val[flag][k]; 	R->val[flag][k]=t; 
		}
		//Elimination
		for(j=i+1;j<n;j++)
		{
			t=M->val[j][i]/M->val[i][i];
			for(k=0;k<n;k++)
			{
				M->val[j][k]-=t*M->val[i][k];
				R->val[j][k]-=t*R->val[i][k];
			}
		}
	}
	//Back-Substitution
	for(i=n-1;i>=0;i--)
	{
		t=M->val[i][i]; M->val[i][i]=1;
		for(k=0;k<n;k++)
			R->val[i][k]/=t;
		for(j=i-1;j>=0;j--)
		{
			t=M->val[j][i]/M->val[i][i];
			for(k=0;k<n;k++)
			{
				M->val[j][k]-=t*M->val[i][k];
				R->val[j][k]-=t*R->val[i][k];
			}
		}
	}
	return SUCCESS;
}
STATE MAT_minEig(MAT *M,float* result,MAT* V){
	s16 i,n,cnt,row,col;
	double norm,lambda_last=0,lambda;
	
		
	if (M->rows!=M->cols||M->cols==0||M->rows==0)
	{
		V->rows=0; 	V->cols=0;
		return FAIL;
	}
	
	n=M->rows;
	V->rows=n;  V->cols=1;
	
	for (row=0;row<M->rows;row++)
		for (col=0;col<M->cols;col++)
			M->val[row][col]/=1000*1000;

	
	//Method : Inversed power iteration 
	if (MAT_Inverse(M,&invM)==FAIL) return FAIL;
	
	for (row=0;row<invM.rows;row++)
		for (col=0;col<invM.cols;col++)
			invM.val[row][col]/=1000*1000;

	if (DEBUG)
		MAT_output(&invM,"invM");
		
	
	V_last.rows=n; V_last.cols=1;
	
	for (i=0;i<n;i++)
	{
		V_last.val[i][0]=1;
		V->val[i][0]=0;
	}
	
	cnt=0; lambda_last=0;
	
	while (cnt<EIG_MAX_ITER)
	{
		cnt++;
		
		if (MAT_Multiply(&invM,&V_last,V)==FAIL) return FAIL;
		
		lambda=0;
		for (i=0;i<n;i++)
			if (fabs(V->val[i][0])>lambda)
				lambda=fabs(V->val[i][0]);
		for (i=0;i<n;i++)
			V_last.val[i][0]=V->val[i][0]/lambda;

		if (fabs(lambda-lambda_last)<EIG_EPS)
			break;
		lambda_last=lambda;		
	}
	if (cnt==EIG_MAX_ITER)
	{
		V->rows=0; 	V->cols=0;
		return FAIL;
	}

	*result=1/lambda;
	norm=MAT_Norm(&V_last);
	for (i=0;i<n;i++)
		V->val[i][0]=V_last.val[i][0]/norm;
	return SUCCESS;
}
//-----------------
STATE RGB2BW(){
	u32 ReadAddr=0x0000;

	u16 color = 0;
	s16 r,g,b,v=0;
	u16 row,col,brow,bcol;
		
	if (OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y,IMG_STA_X+159,IMG_STA_Y+119);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}
	
	for(row=0;row<BW_BLOCK_CNT;row++) 
		for (col=0;col<BW_BLOCK_CNT;col++)
		{
			Block_Max[row][col]=0;
			Block_Min[row][col]=64;
		}
	
	
	for(row=0;row<SIZE_H;row++) 
		for (col=0;col<SIZE_W;col++)
		{                                                                              
			ReadAddr+=2;
			if ((row&1)||(col&1))
				continue;
			color = *(vu16*)(Bank1_SRAM3_ADDR+ReadAddr-2);   
		
			r = get_r_value(color)*2;
			g = get_g_value(color);
			b = get_b_value(color)*2;
			
			v=max(max(r,g),b);
			
			vimg[row/2][col/2]=v;
			Flag_FF[row/2][col/2]=0;
			
			brow=row/(SIZE_H/BW_BLOCK_CNT);
			bcol=col/(SIZE_W/BW_BLOCK_CNT);
			
			if (v<Block_Min[brow][bcol])
				Block_Min[brow][bcol]=v;
			
			if (v>Block_Max[brow][bcol])
				Block_Max[brow][bcol]=v;
			
			if (OUTPUT)
			{
				color=make_rgb565(r/2,g,b/2);
				*(volatile u16 *) (LCD_DATA) = color;
			}
		}
		return SUCCESS;
}


STATE Find_QR(){
	s16 b2w[3]={0,0,0},w2b[3]={0,0,0};
	s16 row,col,i;
	s16 pos[5];
	u16 color = 0;
	u8 now=0,last=0;
	Result_Num=0;


	for (row=0;row<ROWS;row++)
	{
		b2w[0]=0; b2w[1]=0; b2w[2]=0;
		w2b[0]=0; w2b[1]=0; w2b[2]=0;
		now=0; last=0;
		for (col=0;col<COLS;col++)
		{
			last=now; 
 			now=get_bw(row,col);
			
	    if (last==0&&now==1) 
			{
				b2w[0]=b2w[1]; b2w[1]=b2w[2]; b2w[2]=col;
			}
			else if (last==1&&now==0) 
			{
				w2b[0]=w2b[1]; w2b[1]=w2b[2]; w2b[2]=col;
			}
			else continue;
      
			if (b2w[0]==0||b2w[1]==0||b2w[2]==0||w2b[0]==0||w2b[1]==0||w2b[2]==0)
				continue;
	        
			pos[0]=b2w[0]-w2b[0];
			pos[1]=w2b[1]-b2w[0];
			pos[2]=b2w[1]-w2b[1];
			pos[3]=w2b[2]-b2w[1];
			pos[4]=b2w[2]-w2b[2];

			if (max4(pos[0],pos[1],pos[3],pos[4])-min4(pos[0],pos[1],pos[3],pos[4])<QR_THRESH)
				if (fabs(3.0*(float)(pos[0]+pos[1]+pos[3]+pos[4])/4.0-pos[2])<QR_THRESH)
				{
					for (i=w2b[1];i<b2w[1];i++)
					{
						if (Flag_FF[row][i]==0)
						{
 							Result_Num++; 
							if (bfs(row,i,Result_Num)==FAIL) Result_Num--;
 							if (Result_Num>10)
 								return FAIL;
						}
					}
				}
		}
	}
	
	if (OUTPUT)
	{
		LCD_SetBox(IMG_STA_X+COLS,IMG_STA_Y,IMG_STA_X+COLS+COLS-1,IMG_STA_Y+ROWS-1);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
		
		for (row=0;row<ROWS;row++)
			for (col=0;col<COLS;col++)
			{
				now=get_bw(row,col);
				if (Flag_FF[row][col]>0)
					color=make_rgb565(31,0,0);
				else
					color=make_rgb565(now*31,now*63,now*31);
				*(volatile u16 *) (LCD_DATA) = color;
			}
		
		sprintf(str,"QR_Cnt:%d  ",Result_Num);	
		LCD_WriteString16_ili9481(8, 280, GREEN,DARKBLUE,str);

	}
	return SUCCESS;	
}



STATE Check_Pattern(){
	u16 color;
	s16 tl,tr,bl,br,i,j,trow,tcol;
	float row0,col0,row,col;
	
	if (OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y+ROWS,IMG_STA_X+COLS-1,IMG_STA_Y+ROWS+ROWS-1);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
		
		for (trow=0;trow<ROWS;trow++)
			for (tcol=0;tcol<COLS;tcol++)
			{
				color=make_rgb565(vimg[trow][tcol]/2,vimg[trow][tcol],vimg[trow][tcol]/2);
				*(volatile u16 *) (LCD_DATA) = color;
			}
	}	
	
	for (tl=1;tl<=Result_Num;tl++)
		for (tr=1;tr<=Result_Num;tr++)
			for (bl=1;bl<=Result_Num;bl++)
				for (br=1;br<=Result_Num;br++)
				{
					if (tl==tr||tl==bl||tl==br||tr==bl||tr==br||bl==br)
						continue;
				
					pos_row[0][1]=Result_Row[tl];		pos_col[0][1]=Result_Col[tl];
					pos_row[0][7]=Result_Row[tr];		pos_col[0][7]=Result_Col[tr];
					pos_row[11][1]=Result_Row[bl];		pos_col[11][1]=Result_Col[bl];
					pos_row[11][7]=Result_Row[br];		pos_col[11][7]=Result_Col[br];

					row0=Result_Row[tr]-Result_Row[tl]; col0=Result_Col[tr]-Result_Col[tl];
					row=Result_Row[br]-Result_Row[tr];	col=Result_Col[br]-Result_Col[tr];
					if(row0*col-row*col0>0)
						continue;

					row0=row; col0=col;
					row=Result_Row[bl]-Result_Row[br];	col=Result_Col[bl]-Result_Col[br];
					if(row0*col-row*col0>0)
						continue;

					row0=row; col0=col;
					row=Result_Row[tl]-Result_Row[bl];	col=Result_Col[tl]-Result_Col[bl];
					if(row0*col-row*col0>0)
						continue;

					for (i=0;i<PATTERN_COL;i++)
					{
						pos_row[0][i]=(pos_row[0][7]-pos_row[0][1])*(i-1.0)/6.0+pos_row[0][1];
						pos_col[0][i]=(pos_col[0][7]-pos_col[0][1])*(i-1.0)/6.0+pos_col[0][1];
						pos_row[11][i]=(pos_row[11][7]-pos_row[11][1])*(i-1.0)/6.0+pos_row[11][1];
						pos_col[11][i]=(pos_col[11][7]-pos_col[11][1])*(i-1.0)/6.0+pos_col[11][1];
					}
					
					for (i=1;i<PATTERN_ROW-1;i++)
					{
						pos_row[i][0]=(pos_row[11][0]-pos_row[0][0])*i/11.0+pos_row[0][0];
						pos_col[i][0]=(pos_col[11][0]-pos_col[0][0])*i/11.0+pos_col[0][0];
						pos_row[i][8]=(pos_row[11][8]-pos_row[0][8])*i/11.0+pos_row[0][8];
						pos_col[i][8]=(pos_col[11][8]-pos_col[0][8])*i/11.0+pos_col[0][8];
					}
					
					
					for (i=1;i<PATTERN_ROW-1;i++)
						for (j=1;j<PATTERN_COL-1;j++)
						{
							calc_position(i,j,&row,&col);
							pos_row[i][j]=row;
							pos_col[i][j]=col;
						}

					//------------------------
					i=(int)(pos_row[5][4]+0.5); 
					j=(int)(pos_col[5][4]+0.5);
					if (get_bw(i,j)==1)
						continue;

					i=(int)(pos_row[9][4]+0.5);
					j=(int)(pos_col[9][4]+0.5);
					if (get_bw(i,j)==1)
						continue;

					i=(int)(pos_row[10][4]+0.5);
					j=(int)(pos_col[10][4]+0.5);
					if (get_bw(i,j)==1)
						continue;

					i=(int)(pos_row[11][4]+0.5);
					j=(int)(pos_col[11][4]+0.5);
					if (get_bw(i,j)==1)
						continue;

					i=(int)(pos_row[5][0]+0.5);
					j=(int)(pos_col[5][0]+0.5);
					if (get_bw(i,j)==1)
						continue;

					i=(int)(pos_row[5][8]+0.5);
					j=(int)(pos_col[5][8]+0.5);
					if (get_bw(i,j)==1)
						continue;
					//------------------------					
					
					for (i=2;i<=8;i+=2)
						for (j=1;j<=3;j+=2)
						{
							trow=(s16)(pos_row[i][j]+0.5);
							tcol=(s16)(pos_col[i][j]+0.5);
							if (trow>=ROWS||tcol>=COLS)
								return FAIL;
// 							if (OUTPUT&&tcol>0&&trow>0)
// 							{
// 								for (ti=-2;ti<=2;ti++)
// 									for (tj=-2;tj<=2;tj++)
// 										LCD_DrawPixel(IMG_STA_X+tcol+tj,IMG_STA_Y+ROWS+trow+ti,RED); 									
// 							}
						}
					return SUCCESS;
				}
	return FAIL;
}



STATE FindKeyPoint(){
	s16 result_row,result_col,row,col,i,j;
	s16	row0,col0;
	s16 wsize=(HARRIS_ARRAYSIZE-1)/2;

	
	for(i=0;i<=HARRIS_WSIZE;i++)
		for(j=0;j<=HARRIS_WSIZE;j++)
			Harris_Exp[i][j]=exp((float)(-i*i-j*j)/2.0);
	
	Mat_m.rows=3;		Mat_m.cols=14; 
	Mat_M.rows=3;		Mat_M.cols=14; 		
	Mat_L.rows=28;	Mat_L.cols=9; 
		
	for(i=0;i<pos_list_len;i++)
	{
		row=pos_list[i][0]; col=pos_list[i][1];
		row0=(s16)(pos_row[row][col]+0.5)-wsize;
		col0=(s16)(pos_col[row][col]+0.5)-wsize;
		
		if (OUTPUT&&col0>0&&row0>0)
		{
			for (j=0;j<2*wsize+1;j++)
			{
				LCD_DrawPixel(IMG_STA_X+col0,IMG_STA_Y+ROWS+row0+j,RED);
				LCD_DrawPixel(IMG_STA_X+col0+2*wsize,IMG_STA_Y+ROWS+row0+j,RED);
			}
			for (j=0;j<2*wsize+1;j++)
			{
				LCD_DrawPixel(IMG_STA_X+col0+j,IMG_STA_Y+ROWS+row0,RED);
				LCD_DrawPixel(IMG_STA_X+col0+j,IMG_STA_Y+ROWS+row0+2*wsize,RED);
			}
		}
		
		if (row0<0||col0<0||row0+2*wsize-1>=ROWS||col0+2*wsize-1>=COLS)
			return FAIL;
		
		for (row=0;row<HARRIS_ARRAYSIZE;row++)
			for (col=0;col<HARRIS_ARRAYSIZE;col++)
				Harris_I[row][col]=(float)(vimg[row0+row][col0+col])/63.0;
		
		result_row=-1;	 result_col=-1;
		myHarris(&result_row,&result_col);
 		if (result_row<0||result_row>=ROWS||result_col<0||result_col>=COLS)
 			return FAIL;
		
 	
		if (OUTPUT)
			for (row=-1;row<=1;row++)
				for (col=-1;col<=1;col++)
					LCD_DrawPixel(IMG_STA_X+col0+result_col+col,IMG_STA_Y+ROWS+row0+result_row+row,RED);
		
		
 		row=pos_list[i][0]; col=pos_list[i][1];
		
		Mat_M.val[0][i]=(col-4.0)*(PATTERN_X/2.0);	
		Mat_M.val[1][i]=(row-5.0)*(-PATTERN_Y/2.0);
		Mat_M.val[2][i]=1; 

		
		Mat_m.val[0][i]=(s16)(pos_col[row][col]+0.5)-wsize+result_col;
		Mat_m.val[1][i]=(s16)(pos_row[row][col]+0.5)-wsize+result_row;
		Mat_m.val[2][i]=1; 

		Mat_L.val[2*i][0]=Mat_M.val[0][i];
		Mat_L.val[2*i][1]=Mat_M.val[1][i];
		Mat_L.val[2*i][2]=Mat_M.val[2][i];

		Mat_L.val[2*i][3]=0;
		Mat_L.val[2*i][4]=0;
		Mat_L.val[2*i][5]=0;

		Mat_L.val[2*i][6]=-Mat_m.val[0][i]*Mat_M.val[0][i];
		Mat_L.val[2*i][7]=-Mat_m.val[0][i]*Mat_M.val[1][i];
		Mat_L.val[2*i][8]=-Mat_m.val[0][i]*Mat_M.val[2][i];

		Mat_L.val[2*i+1][0]=0;
		Mat_L.val[2*i+1][1]=0;
		Mat_L.val[2*i+1][2]=0;

		Mat_L.val[2*i+1][3]=Mat_M.val[0][i];
		Mat_L.val[2*i+1][4]=Mat_M.val[1][i];
		Mat_L.val[2*i+1][5]=Mat_M.val[2][i];

		Mat_L.val[2*i+1][6]=-Mat_m.val[1][i]*Mat_M.val[0][i];
		Mat_L.val[2*i+1][7]=-Mat_m.val[1][i]*Mat_M.val[1][i];
		Mat_L.val[2*i+1][8]=-Mat_m.val[1][i]*Mat_M.val[2][i];		
	}
	return SUCCESS;
}

STATE Extract_Extrinsic(){
	s16 i,j,cnt;
	float lambda,tmp;
	

	
	if (MAT_Transpose(&Mat_L,&Mat_LT)==FAIL) return FAIL;	
	
	if (MAT_Multiply(&Mat_LT,&Mat_L,&Mat_LTL)==FAIL) return FAIL;	//!!!!!!!!!!!!!!!
		
	if (MAT_minEig(&Mat_LTL,&tmp,&Vect_Eig)==FAIL) return FAIL;
	
	if (OUTPUT)
	{
		sprintf(str,"Eig Done!\n");
		LCD_WriteString16_ili9481(200, 200, GREEN,BLACK,str);
		if (DEBUG)
		{
			Com2_Puts("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
			Com2_Puts(str);
		}
	}
	
	Mat_H.rows=3; Mat_H.cols=3; cnt=0;
	for (i=0;i<3;i++)
		for (j=0;j<3;j++)
		{
			Mat_H.val[i][j]=Vect_Eig.val[cnt][0];
			cnt++;
		}
	
	Vect_H1.rows=3; Vect_H1.cols=1;
	Vect_H2.rows=3; Vect_H2.cols=1;
	Vect_H3.rows=3; Vect_H3.cols=1;
	for (i=0;i<3;i++)
	{
		Vect_H1.val[i][0]=Mat_H.val[i][0];
		Vect_H2.val[i][0]=Mat_H.val[i][1];
		Vect_H3.val[i][0]=Mat_H.val[i][2];
	}
	
	
	if (MAT_Inverse(&Mat_A,&Mat_invA)==FAIL) return FAIL;
	
	
	if (MAT_Multiply(&Mat_invA,&Vect_H2,&Mat_Temp)==FAIL) return FAIL;	
	lambda=1/MAT_Norm(&Mat_Temp);  

	if (DEBUG)
	{
		MAT_output(&Mat_L,"L");
		MAT_output(&Vect_Eig,"Vect_Eig");
		MAT_output(&Mat_invA,"Mat_invA");
		MAT_output(&Vect_H2,"Vect_H2");
		MAT_output(&Mat_Temp,"Mat_Temp");
		sprintf(str,"lambda=%.4f\n",lambda);
		Com2_Puts(str);
	}
	
	if (MAT_Multiply(&Mat_invA,&Vect_H3,&Vect_T)==FAIL) return FAIL;
	for (i=0;i<3;i++)
		Vect_T.val[i][0]*=lambda;
 	if (Vect_T.val[2][0]<100)
 		return FAIL;

	if (DEBUG)
	{
		MAT_output(&Vect_T,"Vect_T");
		Com2_Puts("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");
	}
	
	
	if (OUTPUT)
	{
		sprintf(str,"X:%d    ",(s16)(Vect_T.val[0][0]+0.5));
		LCD_WriteString16_ili9481(10, 300, GREEN,DARKBLUE,str);
		sprintf(str,"Y:%d    ",(s16)(Vect_T.val[1][0]+0.5));
		LCD_WriteString16_ili9481(100, 300, GREEN,DARKBLUE,str);
		sprintf(str,"Z:%d    ",(s16)(Vect_T.val[2][0]+0.5));
		LCD_WriteString16_ili9481(200, 300, GREEN,DARKBLUE,str);
	}
		
// 	if (MAT_Multiply(Mat_invA,Vect_H1,&Vect_R1)==FAIL) return FAIL;
// 	for (i=0;i<3;i++)
// 		Vect_R1.val[i][0]*=lambda;

// 	if (MAT_Multiply(Mat_invA,Vect_H2,&Vect_R2)==FAIL) return FAIL;
// 	for (i=0;i<3;i++)
// 		Vect_R2.val[i][0]*=lambda;

//CrossProd!!!!!!	

// 	if (MAT_HorizCat(Vect_R1,Vect_R2,&Mat_R)==FAIL) return FAIL;
// 	if (MAT_HorizCat(Mat_R,Vect_R3,&Mat_R)==FAIL) return FAIL;
	

	return SUCCESS;
}

#endif


