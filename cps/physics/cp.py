#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2015-12-14 09:18:27
# @Last Modified by:   obsidian
# @Last Modified time: 2015-12-14 09:22:22
import commands
f1 = open('bus1/bus.py','r')
content = f1.read(-1)
for i in range(1, 9):
    #subprocess.Popen('mkdir bus' + str(i+1), shell=True)
    commands.getoutput('cp bus1/* bus' + str(i + 1) + '/')
    f = open('bus'+str(i+1)+'/bus.py','w')
    s = content.replace('BASEPORT = 6100','BASEPORT = '+str(6000+(i+1)*100))
    f.write(s)
    f.close()
commands.getoutput('python /home/obsidian/program/ljx/security/cps/model/script/data/datainit.py')
