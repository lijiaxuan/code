#include "all.h"

int main(void)
{ 
    InitAll();

    /******************************************
    *	*GPS在中断中实现
    *	*main函数中实现 菜单显示、按键响应、调用相应子函数
    *	*
    *	*程序初始化后一直呆在一个一个的 while(1) 中 有两种while(1)
    *	*一种是菜单 要执行的操作是显示菜单和响应按键进入下级菜单和函数 
    *	*一种是函数 调用相应的函数 
    *	*菜单和函数之间要能随时切换
    *	*2016/1/13
    *	*旺	旺
    *******************************************/
    //进入菜单
    {
        LCD_Clear();
        ShowMenuList();
            
        while(1)
        {
            if (KeyReady==1)
            { 
                KeyReady=0;
                KeyNumOperate(KeyValue);	
                
            }
            refresh();
            delay(10);
        }
    }
}
