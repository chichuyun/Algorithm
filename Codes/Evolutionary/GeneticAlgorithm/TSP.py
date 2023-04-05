# -*- coding: utf-8 -*-
"""
Created on Tue Apr  4 10:34:50 2023

@author: Rupert
"""

import os
import time
import numpy as np
from numpy.random import permutation, random, choice, seed
import matplotlib.pyplot as plt

class GA():
    def __init__(self, opt, iter_num, population, crossover_rate, mutation_rate, Data):
        " Some parameters "

        self._opt = opt
        self._iter_num       = iter_num
        self._population     = population
        self._mutation_rate  = mutation_rate
        self._crossover_rate = crossover_rate
        self._distances = np.zeros(self._iter_num)

        self._Data = Data
        self._position_num = len(Data)
        self._posi_init = np.arange(0, self._position_num, 1)
        self._bins = np.arange(0, self._position_num+1, 1)

        self._lists = []
        for _ in range(self._population):
            self._lists.append(self._get_random_list()) # initial particle lists
        self._lists = np.array(self._lists, dtype=np.int)
        self._indexs = np.arange(0,self._population,1)

        self._loss = np.zeros(self._population)
        self._best = self._posi_init

    def _get_random_list(self):
        """ Get one random parton list """
        return permutation(self._posi_init)

    def _get_distance(self, n): # impoving by modifying distance
        """ Calculate loss """
        # L2 distance
        distance = 0
        for i in range(position_num-1):
            distance += ((self._Data[self._lists[n][i]][0] -
                          self._Data[self._lists[n][i+1]][0])**2 + \
                         (self._Data[self._lists[n][i]][1] -
                          self._Data[self._lists[n][i+1]][1])**2)
        distance += ((self._Data[self._lists[n][0]][0] -
                      self._Data[self._lists[n][self._position_num-1]][0])**2 + \
                     (self._Data[self._lists[n][0]][1] -
                      self._Data[self._lists[n][self._position_num-1]][1])**2)
        return distance

    def _fitness(self):
        " Calculate Fitness "
        loss0, loss1 = 0, np.inf
        for i in range(self._population):
            loss0 = self._get_distance(i)
            self._loss[i] = loss0
            if loss0 < loss1:
                min_list = self._lists[i]
                loss1 = loss0

        return min_list.copy(), loss1

    @staticmethod
    def _loss_to_fitness(loss):
        return 1.0/loss

    def _sort_fitness(self):
        """ Sort the list by sort losses """
        self._lists = self._lists[np.argsort(self._loss)]

    def _Crossover_ox(self):
        """ Generate the next lists """

        bins = permutation(np.arange(0, self._population, 1))
        for c1, c2 in zip(bins[0::2], bins[1::2]):

            if(random() > self._crossover_rate): continue
            # Selection
            male   = self._lists[c1].copy()
            female = self._lists[c2].copy()

            # select position
            n, m = choice(self._bins, 2, replace=False)
            if(n > m): m, n = n, m
            m_select = male[n:m].copy()
            f_select = female[n:m].copy()
            mdiff = np.setdiff1d(male, f_select, assume_unique=True)
            fdiff = np.setdiff1d(female, m_select, assume_unique=True)
            child_m = np.hstack([mdiff, f_select])
            child_f = np.hstack([fdiff, m_select])

            self._lists[c1] = np.array(child_m)
            self._lists[c2] = np.array(child_f)

    def _Crossover_pmx(self):
        """ Generate the next lists """

        bins = permutation(np.arange(0, self._population, 1))
        for c1, c2 in zip(bins[0::2], bins[1::2]):

            if(random() > self._crossover_rate): continue
            # Selection
            male   = self._lists[c1].copy()
            female = self._lists[c2].copy()

            corres_m = self._posi_init.copy()
            corres_f = self._posi_init.copy()
            for i, j in enumerate(male):   corres_m[j] = i
            for i, j in enumerate(female): corres_f[j] = i
            # select position
            n, m = choice(self._bins, 2, replace=False)
            if(n > m): m, n = n, m

            child_m = male.copy()
            for i in range(n,m):
                ele = female[i] # cross element
                index = corres_m[ele] # check if repeat
                while index < n or index >= m: # if repeat
                    ele = female[index]
                    index = corres_m[ele]
                child_m[i] = ele

            child_f = female.copy()
            for i in range(n,m):
                ele = male[i] # cross element
                index = corres_f[ele] # check if repeat
                while index < n or index >= m: # if repeat
                    ele = male[index]
                    index = corres_f[ele]
                child_f[i] = ele

            self._lists[c1] = np.array(child_m)
            self._lists[c2] = np.array(child_f)

    def _Selection_ox(self):
        """ resample the worst gene from the best ones """
        self._fitness()
        self._sort_fitness()
        rain_num = int(self._crossover_rate*self._population)
        bins = np.arange(0, rain_num, 1)
        for c in range(rain_num, self._population-1, 2):
            c1, c2 = choice(bins, 2, replace=False)
            male   = self._lists[c1].copy()
            female = self._lists[c2].copy()

            # select position
            n, m = choice(self._bins, 2, replace=False)
            if(n > m): m, n = n, m
            m_select = male[n:m].copy()
            f_select = female[n:m].copy()
            mdiff = np.setdiff1d(male, f_select, assume_unique=True)
            fdiff = np.setdiff1d(female, m_select, assume_unique=True)
            child_m = np.hstack([mdiff, f_select])
            child_f = np.hstack([fdiff, m_select])

            self._lists[c] = np.array(child_m)
            self._lists[c+1] = np.array(child_f)

    def _Selection_pmx(self):
        """ resample the worst gene from the best ones """
        self._fitness()
        self._sort_fitness()
        rain_num = int(self._crossover_rate*self._population)
        bins = np.arange(0, rain_num, 1)
        for c in range(rain_num, self._population-1, 2):
            c1, c2 = choice(bins, 2, replace=False)
            male   = self._lists[c1].copy()
            female = self._lists[c2].copy()

            corres_m = self._posi_init.copy()
            corres_f = self._posi_init.copy()
            for i, j in enumerate(male):   corres_m[j] = i
            for i, j in enumerate(female): corres_f[j] = i
            # select position
            n, m = choice(self._bins, 2, replace=False)
            if(n > m): m, n = n, m

            child_m = male.copy()
            for i in range(n,m):
                ele = female[i] # cross element
                index = corres_m[ele] # check if repeat
                while index < n or index >= m: # if repeat
                    ele = female[index]
                    index = corres_m[ele]
                child_m[i] = ele


            child_f = female.copy()
            for i in range(n,m):
                ele = male[i] # cross element
                index = corres_f[ele] # check if repeat
                while index < n or index >= m: # if repeat
                    ele = male[index]
                    index = corres_f[ele]
                child_f[i] = ele

            self._lists[c] = np.array(child_m)
            self._lists[c+1] = np.array(child_f)

    def _Mutation(self):
        for i in range(self._population):
            if self._mutation_rate > random():
                n, m = choice(self._posi_init, 2, replace=False)
                if(n > m): m, n = n, m
                t = self._lists[i][n]
                for j in range(n,m,1):
                    self._lists[i][j] = self._lists[i][j+1]
                self._lists[i][m] = t

    def Evolution(self):
        """ Main loop """
        distance = np.inf
        self._Output(0)
        print("Loop begin: ------------------------")
        for i in range(self._iter_num):
            min_list, min_distance = self._fitness()

            if self._opt == "ox":
                self._Crossover_ox()
                self._Mutation()
                self._Selection_ox()

            if self._opt == "pmx":
                self._Crossover_pmx()
                self._Mutation()
                self._Selection_pmx()

            print("Iter: {:3d}, Loss: {:5.2f}, Loss0: {:5.2f}, Std: {:5.2f}"\
                  .format(i, min_distance, np.mean(self._loss), np.std(self._loss)))
            self._distances[i] = min_distance
            if min_distance < distance:
                self._best = min_list.copy()
                distance = min_distance
            self._Output(i)
        print("Loop end: --------------------------")

    def _Output(self, n):
        plt.ioff()
        fig = plt.figure(figsize=(5,5))
        ax = fig.gca()

        for i in range(position_num-1):
            ax.plot([self._Data[self._best[i]][0], self._Data[self._best[i+1]][0]],
                    [self._Data[self._best[i]][1], self._Data[self._best[i+1]][1]], "-bo")
        ax.plot([self._Data[self._best[self._position_num-1]][0],
                 self._Data[self._best[0]][0]],
                [self._Data[self._best[self._position_num-1]][1],
                 self._Data[self._best[0]][1]], "-bo")

        ax.set_xlim(-0.01,1.01)
        ax.set_ylim(-0.01,1.01)
        plt.axis("equal")
        fig.savefig("poss/"+str(n)+".png")
        plt.close(fig)
        plt.ion()

    def Plot(self):
        """ plot time evolution """
        fig = plt.figure()
        ax = fig.gca()

        time = np.arange(0, self._iter_num, 1, dtype=np.int)
        ax.plot(time, self._distances)
        ax.set_xlabel("iteration")
        ax.set_ylabel("minimum distance")

if __name__=="__main__":

    print("PID:", os.getpid())
    seed(os.getpid())

# --------------------------------------------------- #
# Genetic Algorithm
    position_num = 100
    positions = random([position_num, 2])

    ga = GA(opt            = "pmx",    # "pmx" or "ox"
            iter_num       = 1000,
            population     = 200,
            crossover_rate = 0.2,  # < 0.5 this program need low crossover rate
            mutation_rate  = 0.1,  # < 0.1
            Data           = positions)

    time_b = time.time()
    ga.Evolution()
    print("time:   ", (time.time()-time_b)/60)

    ga.Plot()