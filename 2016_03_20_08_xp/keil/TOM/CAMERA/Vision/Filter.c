// /*************************************************************************
//  * 函数名称：
//  *   MedianFilter()
//  * 参数:
//  *   int   iFilterH			- 滤波器的高度
//  *   int   iFilterW			- 滤波器的宽度
//  *   int   iFilterMX		- 滤波器的中心元素X坐标
//  *   int   iFilterMY		- 滤波器的中心元素Y坐标
//  * 说明:
//  *   该函数对DIB图像进行中值滤波。
//  ************************************************************************/
// #define iFilterW 2
// #define iFilterH 2
// #define iFilterMX 1
// #define iFilterMY 1
// #define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

// typedef unsigned char u8;
// typedef unsigned char u8;


// unsigned char GetMedianNum(int * bArray, int iFilterLen);

// void MedianFilter(unsigned char *pImg1,unsigned char *pImg,int nWidth,int nHeight)
// {		
//     unsigned char   *lpSrc;			                // 指向源图像的指针
// 	unsigned char   *lpDst;		                 	// 指向要复制区域的指针
// 	int         aValue[iFilterH*iFilterW];		    // 指向滤波器数组的指针
//     
// 	int			i,j,k,l;		                    // 循环变量	
// 	int			lLineBytes;		                    // 图像每行的字节数	
// 	lLineBytes = WIDTHBYTES(nWidth * 8);
//     
// 	for ( i=0;i<nWidth;i++,pImg++ )
// 		(*pImg)=0;
//     
// 	// 开始中值滤波
// 	// 行(除去边缘几行)
// 	for(i = iFilterMY; i < nHeight - iFilterH + iFilterMY + 1; i++)
// 	{
// 		// 列(除去边缘几列)
// 		for(j = iFilterMX; j < nWidth - iFilterW + iFilterMX + 1; j++)
// 		{
// 			// 指向新DIB第i行，第j个象素的指针
// 			lpDst = pImg + lLineBytes * (nHeight - 1 - i) + j;
// 			
// 			// 读取滤波器数组
// 			for (k = 0; k < iFilterH; k++)
// 			{
// 				for (l = 0; l < iFilterW; l++)
// 				{
// 					// 指向DIB第i - iFilterMY + k行，第j - iFilterMX + l个象素的指针
// 					lpSrc = pImg1 + lLineBytes * (nHeight - 1 - i + iFilterMY - k) + j - iFilterMX + l;
// 				
// 					// 保存象素值
// 					aValue[k * iFilterW + l] = *lpSrc;
// 				}
// 			}
// 			
// 			// 获取中值
// 			* lpDst = GetMedianNum(aValue, iFilterH * iFilterW);
// 		}
// 	}

// }

// unsigned char GetMedianNum(int * bArray, int iFilterLen)
// {
// 	int		i,j;			// 循环变量
// 	unsigned char bTemp;
// 	
// 	// 用冒泡法对数组进行排序
// 	for (j = 0; j < iFilterLen - 1; j ++)
// 	{
// 		for (i = 0; i < iFilterLen - j - 1; i ++)
// 		{
// 			if (bArray[i] > bArray[i + 1])
// 			{
// 				// 互换
// 				bTemp = bArray[i];
// 				bArray[i] = bArray[i + 1];
// 				bArray[i + 1] = bTemp;
// 			}
// 		}
// 	}
// 	
// 	// 计算中值
// 	if ((iFilterLen & 1) > 0)
// 	{
// 		// 数组有奇数个元素，返回中间一个元素
// 		bTemp = bArray[(iFilterLen + 1) / 2];
// 	}
// 	else
// 	{
// 		// 数组有偶数个元素，返回中间两个元素平均值
// 		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
// 	}
// 	
// 	return bTemp;
// }

/*************************************************************************
 * 函数名称：
 *   MedianFilter()
 * 参数:
 *   int   iFilterH			- 滤波器的高度
 *   int   iFilterW			- 滤波器的宽度
 *   int   iFilterMX		- 滤波器的中心元素X坐标
 *   int   iFilterMY		- 滤波器的中心元素Y坐标
 * 说明:
 *   该函数对DIB图像进行中值滤波。
 ************************************************************************/
 #include "stdio.h"

#define FilterL 5

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

//typedef unsigned char u8;
//typedef unsigned char u8;


unsigned char GetMedianNum(int * bArray, int iFilterLen);

void MedianFilter(unsigned char *pImgSrc,unsigned char *pImgDst,int nLength)
{
    unsigned char   *pSrc=pImgSrc;			                // 指向源图像的指针
	unsigned char   *pDst=pImgDst;		                 	// 指向要复制区域的指针

	int         aValue[FilterL];		    // 指向滤波器数组的指针

	int			i,j;		                    // 循环变量


    pDst=pImgDst;
	for ( i=0;i<nLength;i++,pDst++ )
		(*pDst)=0;

	// 开始中值滤波

	(*pImgDst)=(*pImgSrc);
	*(pImgDst+nLength-1)=*(pImgSrc+nLength-1);

	for(i = FilterL/2; i < nLength -(FilterL/2) ; i++)
	{
	    // 保存象素值
        pSrc = pImgSrc + i-1;
        for (j = 0; j < FilterL; j++)
        {
            aValue[j] = *pSrc;
            pSrc++;
        }
        // 获取中值
        pDst=pImgDst+i;
        * pDst = GetMedianNum(aValue, FilterL);

    }

}

unsigned char GetMedianNum(int * bArray, int iFilterLen)
{
	int		i,j;			// 循环变量
	unsigned char bTemp;

	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// 互换
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}

	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		// 数组有奇数个元素，返回中间一个元素
		bTemp = bArray[(iFilterLen - 1) / 2];
	}
	else
	{
		// 数组有偶数个元素，返回中间两个元素平均值
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}

	return bTemp;
}

