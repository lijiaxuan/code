/* 
* @Author: obsidian
* @Date:   2015-12-15 19:55:59
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-30 08:40:10
*/
#ifndef STEER_STRUCT_H

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

struct Steer
{
	unsigned short	        *CCRx;
    unsigned char           status;
    unsigned short          midvalue;
    unsigned short          limit[2];
    float                   ratio;
    void                    (*init)();
    void                    (*set_angle)(float angle);
    void                    (*save)();
};

#define STEER_STRUCT_H
#endif
