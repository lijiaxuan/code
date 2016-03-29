#ifndef LCD_STRUCT

#define MAX_CHAR_NUM    20
#define MAX_ITEM_NUM    16
#define MAX_CHILD_NUM   16
#define LINE_DIS        2
#define PAGE_SIZE       10

#define FOLDER          1
#define FILE            0

// #include "module/GPS/GPS_global.h"
// #include "module/brake/brake_global.h"
// #include "module/camera/camera_global.h"
// #include "module/steer/steer_global.h"
// #include "module/pid/pid_global.h"
// #include "module/proximity/proximity_global.h"



struct Item
{
    unsigned char       ctx[MAX_CHAR_NUM];
    unsigned char       id;
    unsigned char       ctx_len;
    unsigned char       flag;
    unsigned char       is_int;
    unsigned short      color;
    unsigned short*     p_short;
    float*               p_float;
    void                (*func_enter)(void* parg);
    void                (*func_exec)(void* parg);
    void                (*func_input_callback)(void* parg);
    void                (*func_back)(void* parg);
};

struct Node
{
    unsigned char   itemnum;
    unsigned char   page_num;
    unsigned char   page_index;
    unsigned char   node_id;
    struct Item*    p_item;
    unsigned short   positon[2];
    struct Node*    parent;
    struct Node*    child[MAX_ITEM_NUM];
    void            (*init)(void *parg);
    void            (*save)();
};

struct Status
{
    struct Node*    p_current;
    struct Item*    p_item;
    unsigned char   is_input;
    unsigned char   is_run;
};

struct Lcd
{
    struct Node*    root_node;
    struct Status*  p_status;
    unsigned char   key;
    char            update_flag;
    void            (*init)();
    void            (*process)(char key);
    void            (*show_run)();
    void            (*input)();

};

static void menu_nop(void *parg)
{

}

#define LCD_STRUCT
#endif
