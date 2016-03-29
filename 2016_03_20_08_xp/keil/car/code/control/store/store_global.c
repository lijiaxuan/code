/*
* @Author: obsidian
* @Date:   2016-03-16 17:48:32
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-16 19:22:31
*/
#include "store_global.h"
#include "store.h"

struct Store store = 
{
    {0},          //fil
    str_init,   //init
    str_save,   //save
    str_close,  //close
};
