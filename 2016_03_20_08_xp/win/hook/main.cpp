#include <stdio.h>
#include <windows.h>
#pragma pack(1)
typedef struct _JMPCODE
{
    BYTE jmp;
    DWORD addr;
}JMPCODE,*PJMPCODE;
typedef int (WINAPI *MSGBOX)(HWND,LPSTR,LPSTR,UINT);
MSGBOX messagebox;

_declspec(naked) add(
	HWND hWnd,
    LPCSTR lpText,
    LPCSTR lpCaption,
    UINT uTypek)
{
	
	_asm
	{
		PUSH ebp
        mov ebp,esp
		mov eax,messagebox
		add eax,5
		jmp eax
	}
	
}

int add2(int a, int b)
{
	::MessageBoxA(NULL,"hook test","tip",MB_OK);
	return b + a + 2;
}

int main()
{
	char a = 0;
	char b = 0;
	char c = 0;
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	
	messagebox=(MSGBOX)GetProcAddress(hInst, "MessageBoxA" );
    
	JMPCODE jcode;
	//addr = (DWORD)add - (DWORD) MessageBoxA + 5;
    jcode.jmp = 0xe9;//jmp
    jcode.addr = (DWORD)add - (DWORD)MessageBoxA - 5; 

	::WriteProcessMemory(GetCurrentProcess(),MessageBoxA,&jcode,sizeof(JMPCODE),NULL);

	//c = add(a,b);
	//add2(a,b);
	::MessageBoxA(NULL,"hook test","tip",MB_OK);
	return 0;
}