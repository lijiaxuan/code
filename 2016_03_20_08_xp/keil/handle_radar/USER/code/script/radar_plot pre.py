#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-23 09:48:47
# @Last Modified by:   obsidian
# @Last Modified time: 2016-02-16 11:32:29
# among
# 
import numpy as np
island = \
        [
            {'ord' : [787.88,7228.40 ], 'phi': 0, 'en' : 0},
            {'ord' : [687.88,7128.40 ], 'phi': 0, 'en' : 0} ,
            {'ord' : [587.88,7228.40 ], 'phi': 0, 'en' : 0},
            {'ord' : [81.21,7228.40  ], 'phi':  0, 'en'  : 0},
            {'ord' : [-19.21,7128.40 ], 'phi': 0, 'en'  : 0},
            {'ord' : [-119.34,7228.40], 'phi':0,'en' : 0},
            {'ord' : [234.34,6533.43 ], 'phi': 0,'en' : 0},
            {'ord' : [334.34,6633.43 ], 'phi': 0,'en' : 0},
            {'ord' : [434.34,6533.43 ], 'phi': 0,'en' : 0},
            {'ord' : [941.44,6533.43 ], 'phi':0,'en' : 0},
            {'ord' : [1041.44,6633.43], 'phi':0,'en' : 0},
            {'ord' : [1141.44,6533.43], 'phi':0,'en' : 0},
        ]

p1 = [1076,6973]
p2 = [1055,6973]
p3 = [1030,6973]
print np.arctan((island[0]['ord'][1] - p1[1]) / (island[0]['ord'][0] - p1[0]))
print np.arctan((island[3]['ord'][1] - p2[1]) / (island[3]['ord'][0] - p2[0]))
print np.arctan((island[7]['ord'][1] - p3[1]) / (island[7]['ord'][0] - p3[0]))
print np.arctan((island[0]['ord'][1] - p2[1]) / (island[0]['ord'][0] - p2[0]))
print np.arctan((island[3]['ord'][1] - p2[1]) / (island[3]['ord'][0] - p2[0]))
print np.arctan((island[7]['ord'][1] - p2[1]) / (island[7]['ord'][0] - p2[0]))
print 0.3 / 57.3