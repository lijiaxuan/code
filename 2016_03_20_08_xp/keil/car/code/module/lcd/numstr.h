#ifndef NUMSTR_H
#include "lcd.h"

struct Num
{
    char        numstr[11];
    int         intvalue;
    float       floatvalue; 
    signed char dotpos;
    char        index;
    void        (*init)();
    void        (*input)(char key);
    void        (*show)(unsigned char pos);
    void        (*int2str)();
    void        (*float2str_3)();
    void        (*float2str_6)();
    void        (*str2int)();
    void        (*str2float)();
};

extern struct Num num;
#define NUMSTR_H
#endif
