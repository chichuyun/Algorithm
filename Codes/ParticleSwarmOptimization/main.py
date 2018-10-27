# -*- coding: utf-8 -*-
"""
Created on Fri Oct 26 15:56:31 2018

@author: Rupert
"""

import numpy as np
from numpy.random import random as ran
import matplotlib.pyplot as plt

class PSO(object):
    def __init__(self, dim, num, count):
        self.p1 = 2         # parameter 1
        self.p2 = 2         # parameter 2
        self.w = 0.8        # weight
        self.num = num      # size of population
        self.count = count  # the number of iterations
        self.dim = dim      # dimension
        self.vel = None     # velocity
        self.vmax = 1       # the max velocity
        self.pos = None     # position
        self.p_best = None  # the best position of single particles
        self.g_best = None  # the best position of all particle
        self.values = None  # the values of particles
        self.trace = list() # the evolutionary track of the best particle
        self.track = list() # the evolutionary track of all particles

    def initialize(self, lower, upper):
        """ initialization """
        self.lower = lower
        self.upper = upper
        self.pos = ran((self.num, self.dim))*(upper - lower) + lower
        print(self.pos)
        self.vel = np.zeros_like(self.pos)
        self.p_best = self.pos.copy()
        self.values = np.array(list(map(self._fit, self.pos)))
        self.g_best = self.p_best[self.values.argmax()]

    @property
    def fitness(self):
        return self._fit

    @fitness.setter
    def fitness(self, fit):
        """ set fitness function """
        self._fit = fit

    def evolve(self):
        """ iteration """
        for j in range(self.count):
            self.track.append(list())
            for i in range(self.num):
                self.vel[i] = self.w*self.vel[i] + \
                              self.p1*(self.p_best[i] - self.pos[i])*ran() + \
                              self.p2*(self.g_best - self.pos[i])*ran()
                self.vel[i] = np.where(self.vel[i]>self.vmax, \
                                       self.vmax, self.vel[i])
                self.pos[i] += self.vel[i]
                self.pos[i] = np.where(self.pos[i]>self.upper,\
                                       self.upper, self.pos[i])
                self.pos[i] = np.where(self.pos[i]<self.lower, \
                                       self.lower, self.pos[i])
                self.track[-1].append(self.pos[i].copy())
            self.update_the_best_position()

    def update_the_best_position(self):
        for i, best in enumerate(self.p_best):
            if self._fit(best) < self._fit(self.pos[i]):
                self.p_best[i] = self.pos[i]
                self.values[i] = self._fit(self.pos[i])
        self.g_best = self.p_best[self.values.argmax()]
        self.trace.append(self._fit(self.g_best))

    def result(self):
        print("The best position: ", self.g_best)
        print("The best value:     %7.3f"%self._fit(self.g_best))

    def plot(self):
        fig = plt.figure()
        ax = fig.gca()
        ax.plot(list(range(self.count)), self.trace)
        ax.set_xlabel('Counts')
        ax.set_ylabel('Value')
        ax.set_xlim(0, self.count)

if __name__=="__main__":

    def fun(var):
        x, y = var
        return -(x*x + y*y)

    process = PSO(2, 10, 20)  # dim, num, count
    process.fitness = fun     # fitness function
    process.initialize(-5, 5) # lower, upper

    process.evolve()
    process.result() # 7.860, 24.853
    process.plot()   # the evolution of values

# ------------------------------------------------------------ # gif of evolution
# to creat gif need install ffmpeg and moviepy
    import moviepy.editor as mpy
    from moviepy.video.io.bindings import mplfig_to_npimage
    from mpl_toolkits.mplot3d import Axes3D

    duration = 4    # s
    fps = 5         # frame number: fps/s

    plt.ioff()
    def make_frame_mpl(i):
        index = int(i*fps)
        x = np.linspace(process.lower,process.upper,1000,endpoint=True)
        X, Y = np.meshgrid(x, x)
        Z = -fun([X, Y])

        x, y, z = [], [], []
        for xy in process.track[index]:
            x.append(xy[0])
            y.append(xy[1])
            z.append(-fun(xy))

        fig = plt.figure()
        ax = fig.gca(projection='3d')
        ax.view_init(elev=48, azim=-61)
        ax.plot_surface(X, Y, Z, cmap=plt.cm.coolwarm, alpha=0.8)
        ax.scatter(x, y, z, c='k', s=50, facecolor='k', alpha=1)
        ax.set_xlabel('x')
        ax.set_ylabel('y')
        ax.set_zlabel('value')
        ax.set_title(str(index)+'  value:  '+str(process.trace[index]))
        ax.set_xlim(process.lower,process.upper)
        ax.set_ylim(process.lower,process.upper)
        return mplfig_to_npimage(fig)

    animation = mpy.VideoClip(make_frame_mpl, duration=duration)
    animation.write_gif('PSO.gif', fps=fps)
    plt.ion()
