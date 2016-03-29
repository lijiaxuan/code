#include "list.h"


/*菜单*/
struct M_Type MainMenu[M_NUM];
struct M_Type M_GPS[M_NUM];
struct M_Type M_PID[M_NUM];
struct M_Type M_TEST[M_NUM];
struct M_Type *CurrentMenu = MainMenu;//初始化当前菜单为主菜单

//主菜单下的子菜单
struct M_Type MainMenu[M_NUM]=
{   /*Name          菜单属性    	函数			节点*/
	{"1.GPS",      	M_SMenu,		  NULL,			     {MainMenu, M_GPS} },
	{"2.PID",	  	M_SMenu,        NULL,     	         {MainMenu, M_PID} },
	{"3.Go_right",  M_UFunc,        full_go_right,				 {MainMenu, NULL} },
	{"4.BIG_PID",   M_UFunc,        set_BIG_PID,	 {MainMenu, NULL} },
	{"5.LINE_PID",  M_UFunc,        set_LINE_PID,	 {MainMenu, NULL} },
	{"6.TEST",      M_SMenu,        NULL,			     {MainMenu, M_TEST} },
	//{"7.CH_GO",     M_UFunc,        choose_go,		 {MainMenu, NULL} },
	//{"8.Go_left",   M_UFunc,        fu_go_left,		 {MainMenu, NULL} },
};


struct M_Type M_GPS[M_NUM]=
{   /*Name			菜单属性		函数			节点(上下级指针)*/
	{"1.GPS",       M_UFunc,        set_GPS,   	   {MainMenu, NULL} },
//	{"2.MY_P",      M_UFunc,        set_My_PID,		 {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",           M_Empty,        NULL,			     {MainMenu, NULL} },
};

struct  M_Type M_PID[M_NUM]=
{   /*Name			菜单属性		函数			节点(上下级指针)*/
  {"1.R9",         M_UFunc,          set_R9_PID,	         {MainMenu, NULL} },
	{"2.R11",        M_UFunc,        set_R11_PID,			     {MainMenu, NULL} },
	{"3.R13",        M_UFunc,        set_R13_PID,		       {MainMenu, NULL} },
	{"4.R14",        M_UFunc,        set_R14_PID,			     {MainMenu, NULL} },
	{"5.R16",        M_UFunc,        set_R16_PID,			     {MainMenu, NULL} },
	{"6.R18",        M_UFunc,        set_R18_PID,			     {MainMenu, NULL} },
	{"7.R20",        M_UFunc,        set_R20_PID,			     {MainMenu, NULL} },
	{"8.R22",        M_UFunc,        set_R22_PID,			     {MainMenu, NULL} },
};

struct  M_Type M_TEST[M_NUM]=
{   /*Name          菜单属性		函数		 	节点(上下级指针)*/
	{"1.Gyro",    M_UFunc,        set_Gyro,			   {MainMenu, NULL} },
	{"2.Encoder", M_UFunc,        set_Encoder,		 {MainMenu, NULL} },
    {"3.Engine_1",  M_UFunc,        set_Engine_pwm1,		   {MainMenu, NULL} },
	{"4.Engine_2",  M_UFunc,        set_Engine_pwm2,		   {MainMenu, NULL} },
	//{"5.Colour",  M_UFunc,        show_Colour,		 {MainMenu, NULL} },
//	{"6.dot_col", M_UFunc,        dot_collect,	 {MainMenu, NULL} },
	{" ",         M_Empty,        NULL,			     {MainMenu, NULL} },
	{" ",         M_Empty,        NULL,			     {MainMenu, NULL} },
};


/********************************    END OF FILE    ***************************/
