/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MENUGO_H
#define __MENUGO_H

/* Includes ------------------------------------------------------------------*/


/* variable ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
void go(void);
void fu_go(void);
void choose_go(void);
void fu_go_left(void);//左边跑道
void go_left(void);
void go_right(void);

extern int Start_Offset;		//当前前轮对应的点阵下标  前轮的下标 + 车长换算的下标
extern int StartNum;
extern float stop_engine_angle;
//extern Pointfp64 Gps_Start;//当前
       

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#endif 

/**********************************END OF FILE*********************************/
