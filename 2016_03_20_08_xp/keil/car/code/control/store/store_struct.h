#ifndef STORE_STRUCT_H
#include "driver/sd/file.h"

struct Store
{
    FIL         str_fl;
    void        (*init)();
    void        (*save)();
    void        (*close)();
};

#define STORE_STRUCT_H 
#endif
