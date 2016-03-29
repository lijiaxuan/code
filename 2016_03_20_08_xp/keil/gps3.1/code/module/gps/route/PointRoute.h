#ifndef POINTROUTE_H
#define POINTROUTE_H

struct path_point
{
	float							x;
	float							y;
};

struct path_point_withAngle
{
	struct path_point  		position;	//位置
	float  								selfdir;			//车体方向
};
extern int Route[25];
extern const struct path_point_withAngle PointRoute[];



#endif
