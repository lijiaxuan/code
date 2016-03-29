#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2015-12-19 19:41:15
# @Last Modified by:   obsidian
# @Last Modified time: 2015-12-19 22:35:14
import numpy as np
import traceback
import matplotlib.pyplot as plt


class encoder():

    def __init__(self):
        self.distance = 0
        self.last_distance = 0
        self.delta_distance = 0

position = []
x = []
y = []


class car():

    def __init__(self):
        self.position = {'x': 0, 'y': 0}
        self.radian = 0
        self.carwidth = 255.0
        self.encoder = [encoder(), encoder()]

    def get_x_y(self):
        pass

    def update(self):
        global position
        global x
        global y
        delta_distance1 = self.encoder[
            0].distance - self.encoder[0].last_distance
        delta_distance2 = self.encoder[
            1].distance - self.encoder[1].last_distance
        try:
            R = self.carwidth * \
                (delta_distance1 + delta_distance2) / \
                (2 * (delta_distance1 - delta_distance2))
            delta_theta = (delta_distance1 + delta_distance2) / (2 * R)
            l = R * delta_theta
            # delta_x = R * np.sin(delta_theta) * np.sin(self.radian + delta_theta) - R * (
            #     1 - np.cos(delta_theta)) * np.cos(self.radian + delta_theta)
            # delta_y = R * np.sin(delta_theta) * np.cos(self.radian + delta_theta) + R * (
            # 1 - np.cos(delta_theta)) * np.sin((self.radian +
            # delta_theta))

            delta_x = l * np.cos(self.radian + delta_theta / 2)
            delta_y = l * np.sin(self.radian + delta_theta / 2)

            self.radian += delta_theta

            self.position['x'] += delta_x
            self.position['y'] += delta_y
            self.encoder[0].last_distance = self.encoder[0].distance
            self.encoder[1].last_distance = self.encoder[1].distance
            position.append(
                (self.position['x'], self.position['y'], self.radian))
            x.append(self.position['x'])
            y.append(self.position['y'])
            print R
        except:
            traceback.print_exc()

            print self.encoder[0].distance, self.encoder[1].distance
Car = car()
step =  np.pi / 1000
R = 1000
for i in range(1000):
    Car.encoder[1].distance = i * (R + Car.carwidth / 2) * step
    Car.encoder[0].distance = i * (R - Car.carwidth / 2) * step
    Car.update()
print position
plt.plot(x, y)
plt.show()
