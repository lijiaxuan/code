#include <stdio.h>
#include <windows.h>
void Hook();
#pragma pack(1)
typedef struct _JMPCODE
{
    BYTE jmp;
    DWORD addr;
}JMPCODE,*PJMPCODE;
typedef int (WINAPI *MSGBOX)(HWND,LPSTR,LPSTR,UINT);
MSGBOX messagebox;
void log(char * data);


_declspec(naked) add(
	HWND hWnd,
    LPCSTR lpText,
    LPCSTR lpCaption,
    UINT uTypek)
{
	//::MessageBoxA(NULL,"hook test","tip",MB_OK);
	log((char *)lpCaption);
	_asm
	{
		PUSH ebp
        mov ebp,esp
		mov eax,messagebox
		add eax,5
		jmp eax
	}
	
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
   switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		Hook();
       break;
	case DLL_PROCESS_DETACH:
       break;
    case DLL_THREAD_ATTACH:
        break;
   case DLL_THREAD_DETACH:
         break;
    } 
    return TRUE;
}
void Hook()
{
	//::MessageBox(NULL,"hello","tip",MB_OK);
    HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	 DWORD dwFlag;
	 VirtualProtect( ( void* )MessageBoxA, sizeof(JMPCODE), PAGE_EXECUTE_READWRITE, &dwFlag );
	messagebox=(MSGBOX)GetProcAddress(hInst, "MessageBoxA" );
    
	JMPCODE jcode;
	//addr = (DWORD)add - (DWORD) MessageBoxA + 5;
    jcode.jmp = 0xe9;//jmp
    jcode.addr = (DWORD)add - (DWORD)MessageBoxA - 5; 

	memcpy((void *)MessageBoxA,&jcode,sizeof(JMPCODE));
	::MessageBoxA(NULL,"hello","tip",MB_OK);
	
}
void log(char * data)
{
    FILE *fp = fopen("C:\\code\\c++\\hookdll\\data.txt","w");
    fwrite(data,sizeof(data),sizeof(data),fp);
    fclose(fp);
}