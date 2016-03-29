#ifndef __MENUTYPEDEF_H
#define __MENUTYPEDEF_H

#include "M_PID.h"
#include "fullGo.h"
#include "M_GPS.h"
#include "M_Gyro.h"
#include "M_Encoder.h"
#include "M_Engine.h"

#define M_NUM 8
#define NULL  (void *)0

//链表节点
struct M_NodeType
{
	struct M_Type *prev;//前级菜单
	struct M_Type *next;//后级菜单
};
//菜单项属性
enum M_AttribType
{
	M_Empty = 0x00,		//空
	M_UFunc = 0x01,		//用户函数
	M_SMenu = 0x02,		//菜单
};
//菜单项
struct M_Type
{
	char *Name;						//名称
	enum M_AttribType  M_Attrib;	//属性
	void (*MenuFunc)(void);			//入口函数
   struct M_NodeType Mnode;		//节点
};

extern struct M_Type MainMenu[M_NUM];
extern struct M_Type *CurrentMenu;
#endif