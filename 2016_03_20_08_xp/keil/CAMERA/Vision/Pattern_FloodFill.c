#include "stm32f4xx.h"
#include "math.h"
#include "vision.h"
#include "can_database.h"
#include "MT9M111.h"
#include "ILI9481.h"
#include "ext_sram.h"
#include "stdio.h"
#ifdef Detect_Pattern


typedef u8 STATE;
typedef struct
{
	s16 rows,cols;
	float val[MAT_SIZE][MAT_SIZE];
}MAT;

float Angle = 0.0;
//RGB2BW
u8 vimg[ROWS][COLS];
u8 bwimg[ROWS][COLS];

//Find_QR  (BFS)
u8 	Flag_FF[ROWS][COLS];
s16 QRow[BFS_SIZE],QCol[BFS_SIZE];
s16 QHead,QTail; 
s16 dir_row[4]={0,1,0,-1};
s16 dir_col[4]={-1,0,1,0};
s16 Result_Num=0;
float Result_Row[MAX_BLOCKS],Result_Col[MAX_BLOCKS];

//Calc_Position
float pos_row[PATTERN_ROW][PATTERN_COL],pos_col[PATTERN_ROW][PATTERN_COL];

//Find_KeyPoint
u8 pos_list[9][2]={{0,0},{0,2},{0,5},{1,0},{1,2},{1,5},{2,0},{2,3},{2,5}};
u8 pos_list_len=9;
u16 Frames=0;
MAT Mat_M;
MAT Mat_m;
MAT Mat_L;

//Extract_Extrinsic
MAT Mat_invA={3,3,{{0.003345579501783,0.0,-0.530828757070971},{0.0,0.003348204621692,-0.390884494640152},{0.0,0.0,1.0000}}};
//MAT Mat_invA={3,3,{{0.001326033686638,0.0,-0.193053600706483},{0.0,0.001326829527471,-0.126807485621874},{0.0,0.0,1.000000000000000}}};
MAT Mat_LT,Mat_LTL,Mat_H,Mat_Temp,Mat_invR;
MAT Vect_Eig;
MAT	Vect_T,Vect_H1,Vect_H2,Vect_H3;
MAT Vect_R1,Vect_R2,Vect_R3;
MAT invM,V_last;




//LCD && UART
extern void LCD_DrawPixel(u16 x, u16 y,u16 color);
extern void Com1_Puts(char * str);
char str[255];

//----------------- 

void send_data(s16 status,s16 X,s16 Z,s16 angle )
{
	MISO_CAMERA_DATA.i16_data[0] = status;
	MISO_CAMERA_DATA.i16_data[1] = X;
	MISO_CAMERA_DATA.i16_data[2] = Z;
	MISO_CAMERA_DATA.i16_data[3] = angle;	
	Write_Database(S_CAMERA1_ID);
}

//----------------- 
s16 max4(s16 a,s16 b,s16 c,s16 d)
{
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
STATE bfs(s16 row,s16 col,s16 code){
	s16 row0,col0,i;
	float avrow=0,avcol=0;

	QHead=0; QTail=1; 
	QRow[1]=row; QCol[1]=col; 
	Flag_FF[row][col]=code;

	while (QHead!=QTail)
	{
		QHead++;
		row0=QRow[QHead]; avrow+=row0;
		col0=QCol[QHead]; avcol+=col0;
		for (i=0;i<4;i++)
		{
			row=row0+dir_row[i];
			col=col0+dir_col[i];
			if (row<0||col<0||row>=ROWS||col>=COLS)
				continue;

			if (Flag_FF[row][col]==0&&bwimg[row][col]==0)
			{
				Flag_FF[row][col]=code;
				QTail++;
				if (QTail>BFS_SIZE)
					return FAIL;
		
				QRow[QTail]=row;
				QCol[QTail]=col;
			}
		}
	}
	Result_Row[code]=avrow/QTail;
	Result_Col[code]=avcol/QTail;
	if (QTail>BFS_THRESH)
		return SUCCESS;
	else return FAIL;
}



STATE calc_position(s16 i,s16 j,float *row,float *col){
	float row_u,col_u,row_d,col_d,row_l,col_l,row_r,col_r;
	float k1,k2,b1,b2;

	row_u=pos_row[0][j];	col_u=pos_col[0][j];
	row_d=pos_row[2][j];	col_d=pos_col[2][j];
	row_l=pos_row[i][0];	col_l=pos_col[i][0];
	row_r=pos_row[i][5];	col_r=pos_col[i][5];

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
	Com1_Puts(str);
	for (row=0;row<M->rows;row++)
	{
		for (col=0;col<M->cols;col++)
		{
				sprintf(str,"%.10f",M->val[row][col]);
				Com1_Puts(str);
				if (col!=M->cols-1)
					Com1_Puts(",");
		}
		if (row!=M->rows-1)
				Com1_Puts(";\r\n\t");		
	}
	Com1_Puts("];\r\n\r\n");
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
STATE MAT_CrossProd(MAT *V1,MAT *V2,MAT* R){
	float x1,x2,y1,y2,z1,z2;
	
	if (V1->rows!=3||V2->rows!=3||V1->cols!=1||V2->cols!=1)
	{
		R->rows=0;
		R->cols=0;
		return FAIL;
	}
	R->rows=3;
	R->cols=1;
	
	x1=V1->val[0][0];	x2=V2->val[0][0];
	y1=V1->val[1][0];	y2=V2->val[1][0];
	z1=V1->val[2][0];	z2=V2->val[2][0];
	
	R->val[0][0]=y1*z2 - y2*z1;
	R->val[1][0]=x2*z1 - x1*z2;
	R->val[2][0]=x1*y2 - x2*y1;
	
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
	s16 row,col,trow,tcol;
	s16 row_sta,row_end,col_sta,col_end;
	s8 Max,Min,thresh,Maxv=0,Minv=120;
		
	if (OUTPUT)
	{	
		LCD_SetBox(IMG_STA_X,IMG_STA_Y,IMG_STA_X+160-1,IMG_STA_Y+120-1);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
	}
	
	
	for(row=0;row<240;row++) 
    {
		for (col=0;col<320;col++)
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
									
			Maxv=max(Maxv,v);
			Minv=min(Minv,v);
			
			if (OUTPUT)
			{
				color=make_rgb565(r/2,g,b/2);
				*(volatile u16 *) (LCD_DATA) = color;
			}
		}
    }
		
		for(row=0;row<ROWS;row++) 
			for (col=0;col<COLS;col++)
			{
				row_sta=max(row-BW_WSIZE,0);
				row_end=min(row+BW_WSIZE,ROWS-1);
				col_sta=max(col-BW_WSIZE,0);
				col_end=min(col+BW_WSIZE,COLS-1);
				Max=-1; Min=120;
				for(trow=row_sta;trow<=row_end;trow+=BW_SKIP)
					for(tcol=col_sta;tcol<=col_end;tcol+=BW_SKIP)
					{	
						Max=max(Max,vimg[trow][tcol]);
						Min=min(Min,vimg[trow][tcol]);
					}
				if (Max-Min>15)					
					thresh=(Max+Min)*5/10;
				else 
					thresh=(Maxv+Minv)/2;
				if (vimg[row][col]>thresh)
					bwimg[row][col]=1;
				else
					bwimg[row][col]=0;
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
 			now=bwimg[row][col];
			
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
				if (fabs(2.0*(float)(pos[0]+pos[1]+pos[3]+pos[4])/4.0-pos[2])<QR_THRESH)
				{
					for (i=w2b[1];i<b2w[1];i++)
					{
						if (Flag_FF[row][i]==0)
						{
 							Result_Num++; 
							if (bfs(row,i,Result_Num)==FAIL) 
								Result_Num--;
 							if (Result_Num>MAX_BLOCKS)
							{
								sprintf(str,"QR Fail..  ");	
								LCD_WriteString16_ili9481(8, 280, GREEN,DARKBLUE,str);
 								return FAIL;
							}
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
				now=bwimg[row][col];
				if (Flag_FF[row][col]>0)
					color=make_rgb565(31,0,0);
				else
					color=make_rgb565(now*31,now*63,now*31);
				Flag_FF[row][col]=0; //!!!!
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
	STATE FindKeyPoint(void);
	
	if (OUTPUT)
	{
		LCD_SetBox(IMG_STA_X,IMG_STA_Y+ROWS,IMG_STA_X+COLS-1,IMG_STA_Y+ROWS+ROWS-1);
		*(volatile u16 *) (LCD_CMD)=(0x002c);
		
		for (trow=0;trow<ROWS;trow++)
			for (tcol=0;tcol<COLS;tcol++)
			{
				color=make_rgb565(bwimg[trow][tcol]*31,bwimg[trow][tcol]*63,bwimg[trow][tcol]*31);
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
				
					pos_row[0][0]=Result_Row[tl];		pos_col[0][0]=Result_Col[tl];
					pos_row[0][5]=Result_Row[tr];		pos_col[0][5]=Result_Col[tr];
					pos_row[2][0]=Result_Row[bl];		pos_col[2][0]=Result_Col[bl];
					pos_row[2][5]=Result_Row[br];		pos_col[2][5]=Result_Col[br];


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

					for (i=1;i<PATTERN_COL-1;i++)
					{
						pos_row[0][i]=(pos_row[0][5]-pos_row[0][0])*i/5.0+pos_row[0][0];
						pos_col[0][i]=(pos_col[0][5]-pos_col[0][0])*i/5.0+pos_col[0][0];
						pos_row[2][i]=(pos_row[2][5]-pos_row[2][0])*i/5.0+pos_row[2][0];
						pos_col[2][i]=(pos_col[2][5]-pos_col[2][0])*i/5.0+pos_col[2][0];
					}
					
					for (i=1;i<PATTERN_ROW-1;i++)
					{
						pos_row[i][0]=(pos_row[2][0]-pos_row[0][0])*i/2.0+pos_row[0][0];
						pos_col[i][0]=(pos_col[2][0]-pos_col[0][0])*i/2.0+pos_col[0][0];
						pos_row[i][5]=(pos_row[2][5]-pos_row[0][5])*i/2.0+pos_row[0][5];
						pos_col[i][5]=(pos_col[2][5]-pos_col[0][5])*i/2.0+pos_col[0][5];
					}
					
					
					for (i=1;i<PATTERN_ROW-1;i++)
						for (j=1;j<PATTERN_COL-1;j++)
						{
							calc_position(i,j,&row,&col);
							pos_row[i][j]=row;
							pos_col[i][j]=col;
						}
					
					//------------------------
					
					i=(int)(pos_row[0][2]); 
					j=(int)(pos_col[0][2]);
					if (bwimg[i][j]==1)
						continue;
					
					i=(int)(pos_row[1][0]);
					j=(int)(pos_col[1][0]);
					if (bwimg[i][j]==1)
						continue;

					i=(int)(pos_row[1][2]);
					j=(int)(pos_col[1][2]);
					if (bwimg[i][j]==1)
						continue;

					i=(int)(pos_row[1][5]);
					j=(int)(pos_col[1][5]);
					if (bwimg[i][j]==1)
						continue;

					i=(int)(pos_row[2][3]);
					j=(int)(pos_col[2][3]);
					if (bwimg[i][j]==1)
						continue;

					//------------------------
					
					i=(int)(pos_row[0][3]); 
					j=(int)(pos_col[0][3]);
					if (bwimg[i][j]==0)
						continue;
					
					i=(int)(pos_row[1][3]);
					j=(int)(pos_col[1][3]);
					if (bwimg[i][j]==0)
						continue;

					i=(int)(pos_row[2][2]);
					j=(int)(pos_col[2][2]);
					if (bwimg[i][j]==0)
						continue;

					if (FindKeyPoint()==SUCCESS)
					{
						sprintf(str,"Pattern found!         ");	
						LCD_WriteString16_ili9481(100, 280, GREEN,DARKBLUE,str);

						return SUCCESS;
					}
				}
				
	sprintf(str,"Pattern not found..   ");	
	LCD_WriteString16_ili9481(100, 280, GREEN,DARKBLUE,str);
	return FAIL;
}



STATE FindKeyPoint()
{
	s16 result_row,result_col,row,col,i;
    //s16 j;
	s16	row0,col0;
	s16 wsize=(HARRIS_ARRAYSIZE-1)/2;
	
	Mat_m.rows=3;		Mat_m.cols=pos_list_len;
	Mat_M.rows=3;		Mat_M.cols=pos_list_len;
	Mat_L.rows=2*pos_list_len;	Mat_L.cols=9; 

	Result_Num=1;
	
	for(i=0;i<pos_list_len;i++)
	{
		row=pos_list[i][0]; col=pos_list[i][1];
		row0=(s16)(pos_row[row][col]+0.5)-wsize;
		col0=(s16)(pos_col[row][col]+0.5)-wsize;
		
// 		if (OUTPUT&&col0>0&&row0>0)
// 		{
// 			for (j=0;j<2*wsize+1;j++)
// 			{
// 				LCD_DrawPixel(IMG_STA_X+col0,IMG_STA_Y+ROWS+row0+j,RED);
// 				LCD_DrawPixel(IMG_STA_X+col0+2*wsize,IMG_STA_Y+ROWS+row0+j,RED);
// 			}
// 			for (j=0;j<2*wsize+1;j++)
// 			{
// 				LCD_DrawPixel(IMG_STA_X+col0+j,IMG_STA_Y+ROWS+row0,RED);
// 				LCD_DrawPixel(IMG_STA_X+col0+j,IMG_STA_Y+ROWS+row0+2*wsize,RED);
// 			}
// 		}
		
		if (row0<0||col0<0||row0+2*wsize-1>=ROWS||col0+2*wsize-1>=COLS)
			return FAIL;
		
		result_row=-1;	 result_col=-1;
		if (bfs(row0+wsize,col0+wsize,Result_Num)==FAIL) return FAIL;		
		result_row=2*Result_Row[Result_Num];	 result_col=2*Result_Col[Result_Num];
		Result_Num++;
// 		result_row=row0+wsize;	 result_col=col0+wsize;		
		
 		if (result_row<0||result_row>=SIZE_H||result_col<0||result_col>=SIZE_W)
 			return FAIL;
		
 	
// 		if (OUTPUT)
// 			for (row=-1;row<=1;row++)
// 				for (col=-1;col<=1;col++)
// 					LCD_DrawPixel(IMG_STA_X+result_col+col,IMG_STA_Y+ROWS+result_row+row,RED);
// 		
		
 		row=pos_list[i][0]; col=pos_list[i][1];
		
		Mat_M.val[0][i]=(col-2.5)*(PATTERN_X);	
		Mat_M.val[1][i]=(row-1.0)*(PATTERN_Y);
		Mat_M.val[2][i]=1; 

		
		Mat_m.val[0][i]=result_col;
		Mat_m.val[1][i]=result_row;
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
	s16 i,j,cnt,row,col;
	float lambda,tmp;



	
	if (OUTPUT)
	{
		LCD_WriteString16_ili9481(8, 300, GREEN,DARKBLUE,"                      ");		
		LCD_WriteString16_ili9481(180, 180, GREEN,BLACK,"            ");
		LCD_WriteString16_ili9481(180, 210, GREEN,BLACK,"            ");
		LCD_WriteString16_ili9481(180, 240, GREEN,BLACK,"            ");
	}
	

	if (MAT_Transpose(&Mat_L,&Mat_LT)==FAIL) return FAIL;	
	
	if (MAT_Multiply(&Mat_LT,&Mat_L,&Mat_LTL)==FAIL) return FAIL;	//!!!!!!!!!!!!!!!
		
	if (MAT_minEig(&Mat_LTL,&tmp,&Vect_Eig)==FAIL) return FAIL;
	
	if (OUTPUT)
	{
		
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
		Com1_Puts(str);
	}
	
	if (MAT_Multiply(&Mat_invA,&Vect_H3,&Vect_T)==FAIL) return FAIL;
	for (i=0;i<3;i++)
			Vect_T.val[i][0]*=lambda;

	if (Vect_T.val[2][0]<100||Vect_T.val[2][0]>1300)
			return FAIL;
	if (MAT_Multiply(&Mat_invA,&Vect_H1,&Vect_R1)==FAIL) 
			return FAIL;
	Angle = asin(Vect_R1.val[2][0]*lambda)*180/3.14159;
	

	if (DEBUG)
			MAT_output(&Vect_T,"Vect_T");

	if (OUTPUT)
	{
			sprintf(str,"Eig Done!\n");
		  LCD_WriteString16_ili9481(8, 300, GREEN,BLACK,str);
		
			for (i=0;i<3;i++)
					Vect_R1.val[i][0]*=lambda;

			if (MAT_Multiply(&Mat_invA,&Vect_H2,&Vect_R2)==FAIL) return FAIL;
			for (i=0;i<3;i++)
					Vect_R2.val[i][0]*=lambda;
			
			sprintf(str,"%.2f    ",Vect_R1.val[0][0]);
			LCD_WriteString16_ili9481(180, 160, GREEN,BLACK,str);
			sprintf(str,"%.2f    ",Vect_R1.val[1][0]);
			LCD_WriteString16_ili9481(180, 180, GREEN,BLACK,str);
			sprintf(str,"%.2f    ",Vect_R1.val[2][0]);
			LCD_WriteString16_ili9481(180, 200, GREEN,BLACK,str);
		
			sprintf(str,"%.2f    ",Vect_R2.val[0][0]);
			LCD_WriteString16_ili9481(240, 160, GREEN,BLACK,str);
			sprintf(str,"%.2f    ",Vect_R2.val[1][0]);
			LCD_WriteString16_ili9481(240, 180, GREEN,BLACK,str);
			sprintf(str,"%.2f    ",Vect_R2.val[2][0]);
			LCD_WriteString16_ili9481(240, 200, GREEN,BLACK,str);
		
			sprintf(str,"%d    ",(s16)(Vect_T.val[0][0]+0.5));
			LCD_WriteString16_ili9481(300, 160, GREEN,BLACK,str);
			sprintf(str,"%d    ",(s16)(Vect_T.val[1][0]+0.5));
			LCD_WriteString16_ili9481(300, 180, GREEN,BLACK,str);
			sprintf(str,"%d    ",(s16)(Vect_T.val[2][0]+0.5));
			LCD_WriteString16_ili9481(300, 200, GREEN,BLACK,str);
			
			sprintf(str,"Angle: %d", (int)Angle);
			LCD_WriteString16_ili9481(180, 220, GREEN,BLACK,str);

	}

	if (OUTPUT)
			for (cnt=0;cnt<pos_list_len;cnt++)
					for (row=-2;row<=2;row++)
							for (col=-2;col<=2;col++)
									LCD_DrawPixel(IMG_STA_X+Mat_m.val[0][cnt]+col,IMG_STA_Y+ROWS+Mat_m.val[1][cnt]+row,RED);		



	return SUCCESS;
}
void findPattern(void)
{
	switch(CAM_CMD)
	{
		case FIND_ONCE:
			if (RGB2BW()==FAIL)
			{
				send_data(0x00, 0, 0, 0);
				break;
			}
			if (Find_QR()==FAIL) 
			{
				send_data(0x00, 0, 0, 0);
				break;
			}
			if (Check_Pattern()==FAIL)
			{
				send_data(0x00, 0, 0, 0);
				break;
			}
			if (Extract_Extrinsic()==FAIL)
			{
				send_data(0x00, 0, 0, 0);
				break;
			}
			CAM_CMD = FIND_NO_SEND;
			send_data(0x01,(s16)(Vect_T.val[0][0]+0.5),(s16)(Vect_T.val[2][0]+0.5),(s16)Angle);
			break;
		case FIND_CONTINIOUS:
			if (RGB2BW()==FAIL)
			{
				send_data(0x00, 0, 0, 0);
				break;
			}
			if (Find_QR()==FAIL) 
			{
				send_data(0x00, 0, 0, 0);
				break;
			}
			if (Check_Pattern()==FAIL)
			{
				send_data(0x00, 0, 0, 0);
				break;
			}
			if (Extract_Extrinsic()==FAIL)
			{
				send_data(0x00, 0, 0, 0);
				break;
			}
			send_data(0x01,(s16)(Vect_T.val[0][0]+0.5),(s16)(Vect_T.val[2][0]+0.5),(s16)Angle);
			break;
		case FIND_NO_SEND:
			if (RGB2BW()==FAIL)
				break;
			if (Find_QR()==FAIL) 
				break;
			if (Check_Pattern()==FAIL)
				break;
			if (Extract_Extrinsic()==FAIL)
				break;
			break;
		case FIND_STOP:
			break;
	}
}
#endif
