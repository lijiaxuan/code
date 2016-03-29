#include <iostream>
#include <windows.h>
#include <errno.h>
#include <psapi.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <TCHAR.H>

using namespace std;

#define THREAD_SIZE 1024

#define DLLNAME "\\ZacharyDll.dll\0"

typedef struct _REMOTE_PARAMETER
{
    CHAR m_printMsgBox[MAX_PATH];
    CHAR m_printDbgStr[MAX_PATH];
    CHAR m_strDllPath[MAX_PATH];
    DWORD m_dwLoadLibraryAddr;
    DWORD m_dwFreeLibraryAddr;
    DWORD m_dwGetProcAddrAddr;
    DWORD m_dwMessageBoxAddr;
    char m_msgContent[50];
    char m_msgTitle[50];
}RemotePara, * PRemotePara;
//=====================================================================================//
//Name: bool RemoteThreadProc(LPVOID lpParameter)                                      //
//                                                                                     //
//Descripion: 远程线程处理例程                                                                 //
//=====================================================================================//
DWORD WINAPI RemoteThreadProc(PRemotePara pRemotePara)
{
    //这个 MessageBox 的地址必须由外部参数传入，因为在其他进程中需要重定向
    //typedef int (WINAPI *MESSAGEBOXA)(HWND, LPCSTR, LPCSTR, UINT);
    typedef HMODULE (WINAPI *LOADLIBRARY_ZACHARY)(LPCSTR);
    LOADLIBRARY_ZACHARY Loadlibrary_cp;
    Loadlibrary_cp = (LOADLIBRARY_ZACHARY)pRemotePara->m_dwLoadLibraryAddr;
    Loadlibrary_cp(pRemotePara->m_strDllPath);
    typedef int (WINAPI *MESSAGEBOXA)(HWND, LPCSTR, LPCSTR, UINT);

    //MESSAGEBOXA MessageBoxA;
    //MessageBoxA = (MESSAGEBOXA)pRemotePara->m_dwMessageBoxAddr;

    //调用 MessageBoxA 来打印消息
    //MessageBoxA(NULL, pRemotePara->m_msgContent, pRemotePara->m_msgTitle, MB_OK);
    return 0;
}


void GetMessageBoxParameter(PRemotePara pRemotePara)
{
   HMODULE hUser32 = LoadLibrary("User32.dll");
   pRemotePara->m_dwMessageBoxAddr = (DWORD)GetProcAddress(hUser32, "MessageBoxA");
   strcat(pRemotePara->m_msgContent, "Hello, Zachary.XiaoZhen !\0");
   strcat(pRemotePara->m_msgTitle, "Hello\0");
   //注意要释放掉 User32
   FreeLibrary(hUser32);
}


void OutputErrorMessage(char *s)
{
    printf("%s",s);
}

void OutputSuccessMessage(char *s)
{
    printf("%s",s);
}

//=====================================================================================//
//Name: bool AdjustProcessTokenPrivilege()                                             //
//                                                                                     //
//Descripion: 提升当前进程权限                                                                 //
//=====================================================================================//
bool AdjustProcessTokenPrivilege()
{
    LUID luidTmp;
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        OutputDebugString("AdjustProcessTokenPrivilege OpenProcessToken Failed ! \n");

        return false;
    }

    if(!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidTmp))
    {
        OutputDebugString("AdjustProcessTokenPrivilege LookupPrivilegeValue Failed ! \n");

        CloseHandle(hToken);

        return FALSE;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luidTmp;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
    {
        OutputDebugString("AdjustProcessTokenPrivilege AdjustTokenPrivileges Failed ! \n");

        CloseHandle(hToken);

        return FALSE;
    }
    return true;
}

//=====================================================================================//
//Name: bool ProcessIsExplorer(DWORD dwProcessId)                                      //
//                                                                                     //
//Descripion: 判定一个进程是否为 Explorer 进程                                                 //
//=====================================================================================//
bool ProcessIsExplorer(DWORD dwProcessId)
{
    HANDLE hProcess;

    hProcess = NULL;

    hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
    if(NULL == hProcess)
    {
        OutputErrorMessage("ProcessIsExplorer - OpenProcess Failed , Error Code Is %d , Error Message Is %s !");

        return FALSE;
    }

    DWORD dwNameLen;
    TCHAR pathArray[MAX_PATH];
    ZeroMemory(pathArray, MAX_PATH);

    dwNameLen = 0;
    dwNameLen = GetModuleFileNameEx(hProcess, NULL, pathArray, MAX_PATH);
    if(dwNameLen == 0)
    {
        CloseHandle(hProcess);

        return FALSE;
    }

    TCHAR exeNameArray[MAX_PATH];
    ZeroMemory(exeNameArray, MAX_PATH);
    _tsplitpath(pathArray, NULL, NULL, exeNameArray, NULL);

    string str1 = exeNameArray;
    if((str1.compare("calc") == 0) || (str1.compare("Calc") == 0))
    {
        CloseHandle(hProcess);

        return TRUE;
    }

    return FALSE;
}


int main()
{
    DWORD dwProcess[30] = {0};
    DWORD dwNeeded = 0;
    DWORD dwExplorerId = 0;
    HANDLE hProcess = 0;
    char * dllpath = "C:\\code\\c++\\hookdll\\Debug\\hookdll.dll";
    //提升当前进程的权限
    AdjustProcessTokenPrivilege();

    //第一个参数为用来保存所有的进程 ID
    //第二个参数则是第一个参数的字节数
    //第三个参数则是写入 dwProcess 数组的字节数
    EnumProcesses(dwProcess, sizeof(dwProcess), &dwNeeded);

    //找到 explorer.exe 进程的 ID
    dwExplorerId = 0;
    for(int i = 0; i < dwNeeded / sizeof(DWORD); i++)
    {
        if(0 != dwProcess[i])
        {
            if(ProcessIsExplorer(dwProcess[i]))
            {
                dwExplorerId = dwProcess[i];
                break;
            }
        }
    }

    hProcess = NULL;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwExplorerId);
    if(NULL == hProcess)
    {
        OutputErrorMessage("main - OpenProcess Failed , Error Code Is %d , Error Message Is %s !");
    }
   PVOID pRemoteThread = VirtualAllocEx(hProcess, NULL, THREAD_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
   if(NULL == pRemoteThread)
   {
       OutputErrorMessage("main - VirtualAllocEx Failed , Error Code Is %d , Error Message Is %s !");

       //关闭进程句柄
       CloseHandle(hProcess);
   }

   if(WriteProcessMemory(hProcess, pRemoteThread, (LPCVOID)RemoteThreadProc, THREAD_SIZE, 0) == FALSE)
   {
      OutputErrorMessage("main - WriteProcessMemory Failed , Error Code Is %d , Error Message Is %s !");

      //释放 VirtualAllocEx 分配的内存
      VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
      CloseHandle(hProcess);
   }

  RemotePara remotePara;
  ZeroMemory(&remotePara, sizeof(RemotePara));
  GetMessageBoxParameter(&remotePara);
  strcpy(remotePara.m_strDllPath,dllpath);
  HMODULE hKernel32 = GetModuleHandle("Kernel32");
  remotePara.m_dwLoadLibraryAddr = (DWORD)GetProcAddress(hKernel32, "LoadLibraryA");
  remotePara.m_dwFreeLibraryAddr = (DWORD)GetProcAddress(hKernel32, "FreeLibrary");
  remotePara.m_dwGetProcAddrAddr = (DWORD)GetProcAddress(hKernel32, "GetProcAddress");

  //在 hProcess 所代表的进程中分配虚拟内存来容纳线程的参数部分
  PRemotePara pRemotePara = (PRemotePara)VirtualAllocEx(hProcess, NULL, sizeof(RemotePara), MEM_COMMIT, PAGE_READWRITE);
  if(NULL == pRemotePara)
  {
      OutputErrorMessage("main - VirtualAllocEx Failed , Error Code Is %d , Error Message Is %s !");

      //释放 VirtualAllocEx 分配的内存
      VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
      CloseHandle(hProcess);
  }


  if(WriteProcessMemory(hProcess, pRemotePara, &remotePara, sizeof(RemotePara), 0) == FALSE)
  {
       OutputErrorMessage("main - WriteProcessMemory Failed , Error Code Is %d , Error Message Is %s !");
       //释放 VirtualAllocEx 分配的内存
       VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
       VirtualFreeEx(hProcess, pRemotePara, 0, MEM_RELEASE);

       CloseHandle(hProcess);
  }

  HANDLE hThread;
  DWORD dwThreadId;

  hThread = NULL;
  dwThreadId = 0;

  //将已经写入到 hProcess 进程中的线程以及线程的参数作为 CreateRemoteThread 的参数，从而创建远程线程
  hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteThread, (LPVOID)pRemotePara, 0, &dwThreadId);
  if(NULL == hThread)
  {
      OutputErrorMessage("main - CreateRemoteThread Failed , Error Code Is %d , Error Message Is %s !");
  }
  else
  {
      OutputSuccessMessage("Code Inject Success !");
  }

  //等待远程线程结束
  WaitForSingleObject(hThread, INFINITE);
  CloseHandle(hThread);

  //必须等到远程线程结束后才能释放宿主进程中所分配的内存，否则宿主进程会直接崩溃
  //释放 VirtualAllocEx 分配的内存
  VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
  VirtualFreeEx(hProcess, pRemotePara, 0, MEM_RELEASE);

  CloseHandle(hProcess);
  return 0;
}

