# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-02-28 08:38:07
# @Last Modified by:   obsidian
# @Last Modified time: 2016-02-28 09:40:17
from ana import arr
for i in range(20):
    for k in range(80):
        pass
point = []
def getdis(i,k):
    for di in range(31):
        if i + di >= 80:
            return 0
        if arr[i + di - 15][k][0] == 31:
            print i + di - 15,k
            arr[i + di - 15][k].append(abs(di - 15))
            return abs(di - 15)
def getneibor(i,k):
    global arr
    arr[i][k][1] = 1
    point.append([i,k,getdis(i,k)])

    if i < 20 or i > 50:
        return
    if k < 10 or k > 60:
        return
    for ix in range(9):
        for iy in range(9):
            #print arr[ix + i - 5][iy + k - 5][0]
            if arr[ix + i - 5][iy + k - 5][0] == 15 and arr[ix + i - 5][iy + k - 5][1] == 0:
                getneibor(ix + i - 5,iy + k - 5)
                
getneibor(40,40)
# print arr
print point
