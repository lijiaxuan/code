 #include "stdio.h"

#define FilterL 3

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

