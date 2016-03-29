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

typedef struct _REMOTE_PARAMETER
{
    CHAR m_msgContent[MAX_PATH];
    CHAR m_msgTitle[MAX_PATH];
    DWORD m_dwMessageBoxAddr;
}RemotePara, * PRemotePara;

//=====================================================================================//
//Name: bool RemoteThreadProc(LPVOID lpParameter)                                      //
//                                                                                     //
//Descripion: Զ���̴߳�������                                                                 //
//=====================================================================================//
DWORD WINAPI RemoteThreadProc(PRemotePara pRemotePara)
{
    //��� MessageBox �ĵ�ַ�������ⲿ�������룬��Ϊ��������������Ҫ�ض���
    typedef int (WINAPI *MESSAGEBOXA)(HWND, LPCSTR, LPCSTR, UINT);

    MESSAGEBOXA MessageBoxA;
    MessageBoxA = (MESSAGEBOXA)pRemotePara->m_dwMessageBoxAddr;

    //���� MessageBoxA ����ӡ��Ϣ
    MessageBoxA(NULL, pRemotePara->m_msgContent, pRemotePara->m_msgTitle, MB_OK);

    return 0;
}




void OutputErrorMessage(char *s)
{
    printf("%s",s);
}

void OutputSuccessMessage(char *s)
{
    printf("%s",s);
}


void GetMessageBoxParameter(PRemotePara pRemotePara)
{
   HMODULE hUser32 = LoadLibrary("User32.dll");
   pRemotePara->m_dwMessageBoxAddr = (DWORD)GetProcAddress(hUser32, "MessageBoxA");
   strcat(pRemotePara->m_msgContent, "Hello, Zachary.XiaoZhen !\0");
   strcat(pRemotePara->m_msgTitle, "Hello\0");
   //ע��Ҫ�ͷŵ� User32
   FreeLibrary(hUser32);
}


//=====================================================================================//
//Name: bool AdjustProcessTokenPrivilege()                                             //
//                                                                                     //
//Descripion: ������ǰ����Ȩ��                                                                 //
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
//Descripion: �ж�һ�������Ƿ�Ϊ Explorer ����                                                 //
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
    //������ǰ���̵�Ȩ��
    AdjustProcessTokenPrivilege();

    //��һ������Ϊ�����������еĽ��� ID
    //�ڶ����������ǵ�һ���������ֽ���
    //��������������д�� dwProcess ������ֽ���
    EnumProcesses(dwProcess, sizeof(dwProcess), &dwNeeded);

    //�ҵ� explorer.exe ���̵� ID
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

       //�رս��̾��
       CloseHandle(hProcess);
   }

   if(WriteProcessMemory(hProcess, pRemoteThread, (LPCVOID)RemoteThreadProc, THREAD_SIZE, 0) == FALSE)
   {
      OutputErrorMessage("main - WriteProcessMemory Failed , Error Code Is %d , Error Message Is %s !");

      //�ͷ� VirtualAllocEx ������ڴ�
      VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
      CloseHandle(hProcess);
   }

  RemotePara remotePara;
  ZeroMemory(&remotePara, sizeof(RemotePara));
  GetMessageBoxParameter(&remotePara);

  //�� hProcess ������Ľ����з��������ڴ��������̵߳Ĳ�������
  PRemotePara pRemotePara = (PRemotePara)VirtualAllocEx(hProcess, NULL, sizeof(RemotePara), MEM_COMMIT, PAGE_READWRITE);
  if(NULL == pRemotePara)
  {
      OutputErrorMessage("main - VirtualAllocEx Failed , Error Code Is %d , Error Message Is %s !");

      //�ͷ� VirtualAllocEx ������ڴ�
      VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
      CloseHandle(hProcess);
  }


  if(WriteProcessMemory(hProcess, pRemotePara, &remotePara, sizeof(RemotePara), 0) == FALSE)
  {
       OutputErrorMessage("main - WriteProcessMemory Failed , Error Code Is %d , Error Message Is %s !");
       //�ͷ� VirtualAllocEx ������ڴ�
       VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
       VirtualFreeEx(hProcess, pRemotePara, 0, MEM_RELEASE);

       CloseHandle(hProcess);
  }

  HANDLE hThread;
  DWORD dwThreadId;

  hThread = NULL;
  dwThreadId = 0;

  //���Ѿ�д�뵽 hProcess �����е��߳��Լ��̵߳Ĳ�����Ϊ CreateRemoteThread �Ĳ������Ӷ�����Զ���߳�
  hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteThread, (LPVOID)pRemotePara, 0, &dwThreadId);
  if(NULL == hThread)
  {
      OutputErrorMessage("main - CreateRemoteThread Failed , Error Code Is %d , Error Message Is %s !");
  }
  else
  {
      OutputSuccessMessage("Code Inject Success !");
  }

  //�ȴ�Զ���߳̽���
  WaitForSingleObject(hThread, INFINITE);
  CloseHandle(hThread);

  //����ȵ�Զ���߳̽���������ͷ�������������������ڴ棬�����������̻�ֱ�ӱ���
  //�ͷ� VirtualAllocEx ������ڴ�
  VirtualFreeEx(hProcess, pRemoteThread, 0, MEM_RELEASE);
  VirtualFreeEx(hProcess, pRemotePara, 0, MEM_RELEASE);

  CloseHandle(hProcess);
  return 0;
}
