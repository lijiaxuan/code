//#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
//#include <stdint.h>

typedef char _TCHAR;

typedef int (WINAPI
    *MessageBox_type) (
    HWND hWnd,
    LPCSTR lpText,
    LPCSTR lpCaption,
    UINT uType) ;

MessageBox_type RealMessageBox = MessageBoxA;

//我们自己的MessageBox，每调用MessageBox都要跳到myMessageBox来处理
_declspec(naked)  void WINAPI
    myMessageBox(
    HWND hWnd,
    LPCSTR lpText,
    LPCSTR lpCaption,
    UINT uType)
{
        __asm
    {
        PUSH ebp
        mov ebp,esp

        /*
        vs2010 debug 编译后的代码由于要cmp esi esp来比较堆栈。
        所以这里在调用非__asm函数前push一下esi
        */
        push esi
    }
    //下面打印MessageBox参数
    printf("hwnd:%8X lpText:%s lpCaption:%s,uType:%8X",hWnd,lpText,lpCaption,uType); 

    __asm
    {
        /*
        vs2010 debug 编译后的代码由于要cmp esi esp来比较堆栈。
        所以这里在调用非__asm函数前push一下esi
        */
        pop esi

        mov ebx,RealMessageBox
        add ebx,5
        jmp ebx
    }
}


#pragma pack(1)
typedef struct _JMPCODE
{
    BYTE jmp;
    DWORD addr;
}JMPCODE,*PJMPCODE;

VOID HookMessageBoxA()
{
    JMPCODE jcode;
    jcode.jmp = 0xe9;//jmp
    jcode.addr = (DWORD)myMessageBox - (DWORD)RealMessageBox - 5; 

    RealMessageBox = MessageBoxA;
    ::WriteProcessMemory(GetCurrentProcess(),MessageBoxA,&jcode,sizeof(JMPCODE),NULL);
}

int main(int argc, _TCHAR* argv[])
{
    HookMessageBoxA();  //hook操作
    ::MessageBoxA(NULL,"hook test","tip",MB_OK); //执行api MessageBox
    return 0;
}