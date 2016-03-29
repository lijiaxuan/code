
#include "goPointPath.h"
#include "gps.h"
#include "PointRoute.h"
#include "lcd.h"
#include "brake.h"

void SetPath_begin_and_end(int sta_num,int end_num);
void full_go_right(void);
extern int	Route_Num;
extern int Point_NowNum;
extern int Point_StartNum;
extern int Point_EndNum;