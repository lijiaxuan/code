/*
* @Author: obsidian
* @Date:   2016-02-28 09:43:27
* @Last Modified by:   obsidian
* @Last Modified time: 2016-02-28 14:28:29
*/

#include "arr.h"
#include "math.h"
signed char record[80][60][2] = {0};
signed short point[100][3] = {0};
unsigned char is = 0;
void get_dis(i,k)
{
    signed char di = 0;
    for(di = -15; di < 15;di ++)
    {
        if(i + di >= 80){return;}
        if(arr[i + di][k] == 31)
        {
            record[i][k][1] = di;
        }
    }
}

void getneighbor(unsigned char i,unsigned char k)
{
    signed char ix = 0;
    signed char iy = 0;
    record[i][k][0] = 1;
    if(i < 20 || i > 50 || k < 10 || k > 60)
    {return;}
    
    get_dis(i,k);
    point[is][0] = i;
    point[is][1] = k;
    point[is][2] = record[i][k][1];
    is ++;
    for(ix = -4; ix <= 4; ix ++)
    {
        for(iy = -4 ; iy <= 4; iy ++)
        {
            if(arr[ix + i][iy + k] == 15 && record[ix + i][iy + k][0] == 0)
            {
                getneighbor(ix + i, iy + k);
            }
        }
    }
}
float ave()
{
}
int main() {
    getneighbor(40,40);
    return 0;
}
