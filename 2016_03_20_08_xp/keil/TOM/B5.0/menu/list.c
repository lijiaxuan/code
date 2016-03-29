
/* Includes ------------------------------------------------------------------*/
#include "all.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*�˵�*/
struct M_Type MainMenu[M_NUM];
struct M_Type M_VIEW[M_NUM];
struct M_Type M_PID[M_NUM];
struct M_Type M_TEST[M_NUM];
struct M_Type *CurrentMenu = MainMenu;//��ʼ����ǰ�˵�Ϊ���˵�

//���˵��µ��Ӳ˵�
struct M_Type MainMenu[M_NUM]=
{   /*Name          �˵�����    	����			�ڵ�*/

	{"1.GO",        M_Empty,        NULL,		     {MainMenu, NULL} },
    {"2.CH_GO",     M_UFunc,        choose_go,		 {MainMenu, NULL} },
	{"3.VIEW",      M_SMenu,		NULL,	        {MainMenu, M_VIEW} },
    {"4.TEST",      M_SMenu,        NULL,			 {MainMenu, M_TEST} },
	{"5.PID",	  	M_SMenu,        NULL,     	    {MainMenu, M_PID} },
	{"6.FILE",      M_UFunc,        NULL,		     {MainMenu, NULL} },
	{"7.GO_R ",     M_UFunc,        go_right,			     {MainMenu, NULL} },
	{"8.GO_L ",     M_UFunc,        go_left,			     {MainMenu, NULL} },
};

struct M_Type M_VIEW[M_NUM]=
{   /*Name			�˵�����		����			�ڵ�(���¼�ָ��)*/
	{"1.GPS",       M_UFunc,        sys_GPS,   	         {MainMenu, NULL} },
	{"2.ERR",       M_Empty,        NULL,			     {MainMenu, NULL} },
	{"3.COL",       M_Empty,        NULL,			     {MainMenu, NULL} },
	{"4.CAM ",      M_UFunc,        Camer,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
    {" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};


struct  M_Type M_PID[M_NUM]=
{   /*Name			�˵�����		����			�ڵ�(���¼�ָ��)*/
    {"1.LINE_PID",  M_UFunc,        set_LINE_PID,	 {MainMenu, NULL} },
    {"2.BIG_PID",   M_UFunc,        set_BIG_PID,	 {MainMenu, NULL} },
	{"3.ADP_PID",   M_UFunc,        set_My_PID,		     {MainMenu, NULL} },
	{"4.OTHER",     M_UFunc,        ChosePid,	        	 {MainMenu, NULL} },
{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};

struct  M_Type M_TEST[M_NUM]=
{   /*Name          �˵�����		����		 	�ڵ�(���¼�ָ��)*/
	{"1.Gyro",      M_UFunc,        sys_Gyro,			 {MainMenu, NULL} },
	{"2.Encoder",   M_UFunc,        sys_Encoder,		 {MainMenu, NULL} },
	{"3.Colour",    M_UFunc,        show_Colour,		 {MainMenu, NULL} },
	{"4.SD",        M_UFunc,        SdTest,			     {MainMenu, NULL} },
	{"5.Engine_1",           M_UFunc,        set_Engine_pwm1,			     {MainMenu, NULL} },
	{"6.Engine_2",           M_UFunc,        set_Engine_pwm2,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/********************************    END OF FILE    ***************************/
