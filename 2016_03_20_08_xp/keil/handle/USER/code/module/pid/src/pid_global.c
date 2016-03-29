/* 
* @Author: obsidian
* @Date:   2015-12-16 10:19:01
* @Last Modified by:   obsidian
* @Last Modified time: 2016-01-18 17:34:47
*/
#include "../inc/pid_global.h"
#define LINE_PID	{0.19,0.00,0.00}
#define LINE2_PID	{0.175,0.00,0.07}
struct Pid pid = 
{
	{
		LINE_PID,

		LINE2_PID,
		LINE_PID,

		LINE2_PID,
		LINE_PID,

		LINE2_PID,
		LINE_PID,

		LINE2_PID,
		LINE_PID,

		{0.16,0.00,0.05},
		LINE_PID,

		{0.43,0.00,0.13},
		LINE_PID,

		{0.16,0.00,0.05},
		{0.16,0.00,0.05},
		LINE_PID,

		{0.15,0.00,0.15},

		LINE_PID,
		{0.20,0.00,0.16},

		LINE_PID,
		{0.25,0.00,0.18},

		LINE_PID,
		{0.18,0.00,0.14},

		LINE_PID,
	},
	{0},	//mend[2]
	{0},	//error[2]
	0,
	get_pid,
	get_double_loop_pid
};
