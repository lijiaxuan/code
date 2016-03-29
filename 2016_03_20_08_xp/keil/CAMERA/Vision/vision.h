#ifndef _VISION_H__
#define _VISION_H__
#include "stm32f4xx.h"

#define CAM_ID 4 //1=Front,2=Left,3=Right,4=Hook,5=Pole

// #define Transfer_img
// #define Detect_Edge
// #define Detect_Square
// #define Detect_WhiteTape
// #define Detect_Pattern
// #define Detect_Circle
// #define Detect_Edge_And_WhiteTape
// #define Detect_Edge_And_WhiteTape_V2
   #define Tape
	////////////////////////////////////
	#define get_b_value(rgb) ((unsigned char)(rgb&0x1f))
	#define get_g_value(rgb) ((unsigned char)((rgb>>5)&0x3f))
	#define get_r_value(rgb) ((unsigned char)((rgb>>11)&0x1f))
	#define make_rgb565(r,g,b) ((r<<11)|(g<<5)|(b))
	#define min(a,b) (a<b?a:b)
	#define max(a,b) (a>b?a:b)
	#define SIZE_H 240
	#define SIZE_W 320
	#define Himg Bank1_SRAM3_ADDR+SIZE_H*SIZE_W*2
	//////////////////////////////////////
	
	#ifdef Transfer_img
	extern void ExtSRAMCopyToLCDQVGA(void);
	#endif

	#ifdef Detect_Edge
	extern void HSV_Decompose(void);
	extern void Sobel(void);
	extern void GetLine(void);
	#endif

	#ifdef Detect_Square
	extern void HSV_Decompose(void);
	extern void Filter(void);
	extern void FloodFill(void);
	#endif

	#ifdef Detect_WhiteTape
	extern void HSV_Decompose(void);
	extern void Hough(void);
	#endif

	#ifdef Detect_Pattern_Harris				//OBSOLETE
	extern u8 RGB2BW(void);
	extern u8 Find_QR(void);
	extern u8 Check_Pattern(void);
	extern u8 FindKeyPoint(void);
	extern u8 Extract_Extrinsic(void);
	#endif
    
    #ifdef WhiteTape
     void HSV_Decompose(void);
     void Sobel(void);
     void GetLine(void);
     void imshow(void);
    #endif
    
    #ifdef Tape
     void Decompose(void);
     void Sobel(void);
     void GetLine(void);
     void imshow(void);
    #endif
    

	#ifdef Detect_Pattern
		extern u8 RGB2BW(void);
		extern u8 Find_QR(void);
		extern u8 Check_Pattern(void);
		extern u8 FindKeyPoint(void);
		extern u8 Extract_Extrinsic(void);
		extern void findPattern(void);
		#define SUCCESS 0
		#define FAIL 	1

		// #define BLACK_THRESH 	38
		#define ROWS 					120
		#define COLS 					160

		#define BW_WSIZE			3
		#define BW_SKIP				2

		#define PATTERN_X 		29.0
		#define PATTERN_Y 		59.0
		#define PATTERN_ROW 	3
		#define PATTERN_COL 	6
		#define QR_THRESH 		10

		#define MAX_BLOCKS 		25
		#define BFS_SIZE		900
		#define BFS_THRESH		6

		#define HARRIS_ARRAYSIZE 		9 //Must be odd  
		#define HARRIS_FACTOR 			0.06
		#define HARRIS_FACTOR_RESULT 	1
		#define HARRIS_WSIZE 			2

		#define MAT_SIZE		30
		#define MAT_EPS			1e-12
		#define EIG_MAX_ITER	10
		#define EIG_EPS			1e-3

		#define DEBUG 			0
		#define OUTPUT 			1
		#define GLOBAL_AXIS 	1

		#define FUNC_ID		4 	//1=Edge,2=Seesaw,3=WhiteTape,4=Pattern,5=Circle
	#endif


	#ifdef Detect_Circle
	extern void HSV_Decompose(void);
	extern void FloodFill(void);
	extern void AxisTransform(void);
	extern void GetCircle(void);
	#endif

	#ifdef Detect_Edge_And_WhiteTape
	extern void HSV_Decompose(void);
	extern void Sobel(void);
	extern void GetLine(void);
	#endif

	#ifdef Detect_Edge_And_WhiteTape_V2
	extern void HSV_Decompose(void);
	extern void Filter(void);
	extern void GetLine(void);
	#endif

	extern unsigned int * dma_base[2];
	extern char state; 
#endif
