# This is the main file
include("NeuralNetwork.jl")

using .NeuralNetwork: build_NN, train!, query
using Plots

NN = build_NN(input_nodes=784,
              hidden_nodes=100,
              output_nodes=10,
              learning_rate=0.5)

open("/mnist_dataset/mnist_train.csv", "r") do train_file
    for data in readlines(train_file)
        targets_list = zeros(Float64, 10) .+ 0.01
        inputs_list = parse.(Float64, split(data, ","))
        @inbounds targets_list[trunc(Int64, inputs_list[1])+1] = 1.0
        train!(NN, inputs_list[2:end]./255.0.*0.99 .+ 0.01, targets_list)
    end;
end;

open("/mnist_dataset/mnist_test.csv", "r") do test_file
    for data in readlines(test_file)
        inputs_list = parse.(Float64, split(data, ","))
        println(trunc(Int64, inputs_list[1]), " ",
                query(NN, inputs_list[2:end]./255.0.*0.99 .+ 0.01))
    end;
end;
