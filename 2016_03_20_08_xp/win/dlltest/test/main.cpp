#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    ::MessageBoxA(NULL,"hello","tip",MB_OK);
    //LoadLibrary("C:\\code\\c++\\hookdll\\Debug\\hookdll.dll");
    cout << "Hello world!" << endl;
    return 0;
}
