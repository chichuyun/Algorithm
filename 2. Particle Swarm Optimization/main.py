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
        self.trace = list() # evolutionary track

    def initialize(self, lower, upper):
        """ initialization """
        self.lower = lower
        self.upper = upper
        self.pos = ran((self.num, self.dim))*(upper - lower) + lower
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
            self.update_the_best_position()
            print("%4d: %7.3f, %7.3f"%(j, self.trace[-1], self.g_best))

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

    def fun(x):
        return x[0]+10*np.sin(5*x[0])+7*np.cos(4*x[0])

    process = PSO(1, 10, 20)  # dim, num, count
    process.fitness = fun     # fitness function
    process.initialize(0, 9)  # lower, upper

    process.evolve()
    process.result() # 7.860, 24.853
    process.plot()
