#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
    ::MessageBoxA(NULL,"hook test","tip",MB_OK);
    return 0;
}
