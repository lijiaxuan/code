
/* Includes ------------------------------------------------------------------*/
#include "all.h"

u8 PID_flag=1;
// int Route[25]={0, 162, 258, 402, 498, 639, 735, 879, 975, 1116, 1203, 1373, 1522, 1625, 1707, 1789, 1947, 2097, 2391, 2655, 2773, 3018, 3096, 3262, 3713,};	
// //				ST	c1  sl   c2   h1   c3   s2   c4   h2   c5     s3    c6   h3    c7     c8    r     c9    d1   dc1    d2   dc2    d3   dc3    d4
//	24段路径

// #define LINE_PID	{0.205,0.00,0.00},
// #define BIG_PID		{0.195,0.00,0.07},

#define LINE_PID	{0.18,0.00,0.08},
#define BIG_PID		{0.15,0.00,0.08},

struct PID Pid_List[]=
{
	LINE_PID					//1		start
	
	BIG_PID						//2  	c1	1819
	LINE_PID					//3  	s1

	BIG_PID						//4  	c2	1819
	LINE_PID					//5  	h1

	BIG_PID						//6  	c3	1819
	LINE_PID					//7	 	s2

	BIG_PID						//8  	c4	1819
	LINE_PID					//9  	h2

	BIG_PID						//10  	c5	1819
	LINE_PID					//11 	s3
	//房 0.15
	{0.35,0.00,0.8},			//12  c6	400 //这一段可以直接用很大的p 保证能转过去
	LINE_PID					//13 	h3
	
	{0.20,0.00,0.03},			//14 	c7	564
	{0.20,0.00,0.03},			//15 	c8	564
	LINE_PID					//16  r

	{0.30,0.00,0.03},			//17  c9	950
		
	LINE_PID					//18  d1
	{0.27,0.00,0.05},			//19  dc1	665
	
	LINE_PID					//20  d2
	{0.25,0.00,0.05},			//21  dc2	590
	
	LINE_PID					//22  d3
	{0.25,0.00,0.05},			//23 	dc3	841
	
	{0.3,0.00,0.02},        	//24 	d4
    
    
    LINE_PID        			//占位 无意义
    
    
    
	LINE_PID					//1		start
	
	BIG_PID						//2  	c1	1819
	LINE_PID					//3  	s1

	BIG_PID						//4  	c2	1819
	LINE_PID					//5  	h1

	BIG_PID						//6  	c3	1819
	LINE_PID					//7	 	s2

	BIG_PID						//8  	c4	1819
	LINE_PID					//9  	h2

	BIG_PID						//10  	c5	1819
	LINE_PID					//11 	s3
	//房 0.15
	{0.45,0.00,0.05},			//12  c6	400 //这一段可以直接用很大的p 保证能转过去
	{0.25,0.00,0.03},					//13 	h3
	
	{0.25,0.00,0.03},			//14 	c7	564
	{0.25,0.00,0.03},			//15 	c8	564
	{0.25,0.00,0.03},					//16  r

	{0.30,0.00,0.03},			//17  c9	950
		
	LINE_PID					//18  d1
	{0.27,0.00,0.05},			//19  dc1	665
	
	LINE_PID					//20  d2
	{0.30,0.00,0.05},			//21  dc2	590
	
	LINE_PID					//22  d3
	{0.30,0.00,0.05},			//23 	dc3	841
	
	{0.3,0.00,0.02},        	//24 	d4
    
    
};



struct RouteInf RouteList[24]=
{
    {1,0,0.15,0.00,0.08,0},
    {2,0,0.15,0.00,0.08,1},//启动区

    {3,0,0.15,0.00,0.08,0},
    {4,0,0.15,0.00,0.08,1},//斜坡1

    {5,0,0.15,0.00,0.08,0},
    {6,0,0.15,0.00,0.08,1},//山冈1

    {7,0,0.15,0.00,0.08,0},
    {8,0,0.15,0.00,0.08,1},//斜坡2

    {9,0,0.15,0.00,0.08,0},
    {10,0,0.15,0.00,0.08,1},//山冈2

    {11,0,0.15,0.00,0.08,0},
    {12,0,0.35,0.00,0.10,1},//斜坡3->山冈3

    {13,0,0.15,0.00,0.08,0},//山冈3

    {14,0,0.25,0.00,0.08,1},

    {15,0,0.25,0.00,0.08,1},
    
    {16,0,0.15,0.00,0.08,0},//河流
    
    {17,0,0.25,0.00,0.08,1},//河流->高地
    
    {18,0,0.15,0.00,0.08,0},//高地
    
    {19,0,0.25,0.00,0.08,1},//山道1    
    {20,0,0.15,0.00,0.08,0},
    
    {21,0,0.25,0.00,0.08,1},//山道2
    {22,0,0.15,0.00,0.08,0},
    
    {23,0,0.25,0.00,0.08,1},//山道3
    {24,0,0.30,0.00,0.08,0},
};

/*
 * 函数名: PID_Clear
 * 描  述: 将PID存储的误差值清除
 * 输  入: 无
 * 输  出: 无
 * 调  用: 外部调用
 */
void PID_Clear(void)
{
	int i;
	
	for(i=0;i<23;i++)
	{
		Pid_List[i].mend=0;
		Pid_List[i].ep1=0;
		Pid_List[i].ep2=0;
	}
}

/*
 * 函数名: PID
 * 描  述: 位置式PID调整，利用了增量式的递推
 * 输  入: -Error 误差量
 *		   -pid pid参数
 * 输  出: 反馈结果
 * 调  用: 外部调用
 */
// float Pid_Cal(double Error,struct PID pid)
// {
// 	double A=0;
// 	double B=0;
// 	double C=0;
// 	double delta=0;
// 	
// 	A = (pid.p+pid.i+pid.d);
// 	B = -(pid.p+2*pid.d);
// 	C = pid.d;
// 	
// 	delta = A*Error + B*pid.ep1 + C*pid.ep2;
// 	pid.mend += delta;
// 	
// 	pid.ep2 = pid.ep1;
// 	pid.ep1 = Error;
// 	
// 	return pid.mend;
// }

float Pid_Cal(double Error,int pid_num)
{
    static	double A=0;
    static	double B=0;
    static	double C=0;
    static	double delta=0;
	
	A = (Pid_List[pid_num].p+Pid_List[pid_num].i+Pid_List[pid_num].d);
	B = -(Pid_List[pid_num].p+2*Pid_List[pid_num].d);
	C = Pid_List[pid_num].d;
	
	delta = A*Error + B*Pid_List[pid_num].ep1 + C*Pid_List[pid_num].ep2;
	Pid_List[pid_num].mend += delta;
	
	Pid_List[pid_num].ep2 = Pid_List[pid_num].ep1;
	Pid_List[pid_num].ep1 = Error;
	
	return Pid_List[pid_num].mend;
}

float LocPIDCalc(double Error,int pid_num)
{
    static double LastError=0;
    static double out=0;
    out = Pid_List[pid_num].p*Error+Pid_List[pid_num].d*(Error-LastError);
    LastError=Error;
    return out;
    
}










/**********************************END OF FILE*********************************/
