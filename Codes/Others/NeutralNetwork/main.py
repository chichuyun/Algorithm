# -*- coding: utf-8 -*-
"""
Created on Fri Feb 22 15:58:16 2019

@author: Rupert
"""

import numpy as np
import matplotlib.pyplot as plt
from scipy import special

class NeuralNetwork(object):
    """ neural network class definition """
    def __init__(self, inputnodes, hiddenodes, outputnodes, learningrate):
        """ initialize the neural network """
        # set number of nodes in each input, hidden, output layer
        self.inodes = inputnodes
        self.hnodes = hiddenodes
        self.onodes = outputnodes

        # set learning rate
        self.lr = learningrate

        # link weight matrices, wih and who
        # weights inside the arrays are w_ij, where link is from ...
        # node i to node j in the next layer

        # method(1) uniform distribution
        # self.wih = np.random.rand(self.hnodes, self.inodes) - 0.5
        # self.who = np.random.rand(self.onodes, self.hnodes) - 0.5
        # methon(2) normal distribution
        self.wih = np.random.normal(0, 1.0/np.sqrt(self.hnodes),
                                    (self.hnodes,self.inodes))
        self.who = np.random.normal(0, 1.0/np.sqrt(self.onodes),
                                    (self.onodes,self.hnodes))

        # activation function is the sigmoid function
        self.activation_function = lambda x: special.expit(x)

    def train(self,inputs_list, targets_list):
        """ train the neural network """

        # convert inputs list to 2d array
        inputs = np.atleast_2d(inputs_list).T
        targets = np.atleast_2d(targets_list).T

        # calculate the signals into hidden layer
        hidden_inputs = np.dot(self.wih, inputs)
        # calculate the signals emerging from hidden layer
        hidden_outputs = self.activation_function(hidden_inputs)

        # calculate the signals into final output layer
        final_inputs = np.dot(self.who, hidden_outputs)
        # calculate the signals emerging from final output layer
        final_outputs = self.activation_function(final_inputs)

        # error is the (target - actual)
        output_errors = targets - final_outputs
        # hidden layer error is the output_error, ...
        # split by weight, recombined at hidden nodes
        hidden_errors = np.dot(self.who.T, output_errors)

        # update the weights for the links between the hidden and output layers
        self.who += self.lr * np.dot((output_errors*final_outputs*(1-final_outputs)),
                                     hidden_outputs.T)
        # update the weights for the links between the input and hidden layers
        self.wih += self.lr * np.dot((hidden_errors*hidden_outputs*(1-hidden_outputs)),
                                     inputs.T)

    def query(self, inputs_list):
        """ query the neural network """

        # convert inputs list to 2d array
        inputs = np.atleast_2d(inputs_list).T

        # calculate the signals into hidden layer
        hidden_inputs = np.dot(self.wih, inputs)
        # calculate the signals emerging from hidden layer
        hidden_outputs = self.activation_function(hidden_inputs)

        # calculate the signals into final output layer
        final_inputs = np.dot(self.who, hidden_outputs)
        # calculate the signals emerging from final output layer
        final_outputs = self.activation_function(final_inputs)

        return final_outputs


if __name__=="__main__":

    # number of input, hidden, output nodes
    input_nodes = 784 # the size of image is (28,28) = 784
    hidden_nodes = 100
    output_nodes = 10 # range from 0 to 9

    # learning rate is 0.5
    learining_rate = 0.3

    nn = NeuralNetwork(input_nodes, hidden_nodes, output_nodes, learining_rate)

    # load data from data files
    with open("mnist_dataset/mnist_train.csv","r") as file:
        for data_list in file:
            targets_list = np.zeros(10) + 0.01
            inputs_list = np.asfarray(data_list.strip().split(',')[1:])
            inputs_list = inputs_list/255.0*0.99 + 0.01 # normalization

            targets_list[int(data_list.strip().split(',')[0])] = 1.0
            nn.train(inputs_list, targets_list)

    with open("mnist_dataset/mnist_test.csv","r") as file:
        for data_list in file:
            inputs_list = np.asfarray(data_list.strip().split(',')[1:])
            inputs_list = inputs_list/255.0*0.99 + 0.01

            final_outputs = nn.query(inputs_list)
            print("real:  ", int(data_list.strip().split(',')[0]))
            print("model: ", np.argmax(final_outputs))

#    inputs_list = inputs_list.reshape((28,28))
#    fig = plt.figure()
#    ax = fig.gca()
#    ax.imshow(inputs_list, cmap='Greys', interpolation='None')


