#include "pid.h"
#include "engineConfig.h"
#include "PointRoute.h"
#include "gps.h"
#include "PointRoute.h"
#include "math.h"

//#include "fullGo.h"

/*
struct error
{
	float error_angle;
	float error_dis;
};
*/

extern float error_angle;
extern float error_dis;
extern Gps_State adj_start;
void go_pointPath_right();