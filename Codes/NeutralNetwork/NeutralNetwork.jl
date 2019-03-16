# This is a testing Neural Network
module NeuralNetwork
    using Random: rand
    using Distributions: Normal
    using Parameters: @with_kw

    export build_NN, train!, query # include NNFit could cause error
    # Main.NeuralNetwork.NNFit is not NNFit ??? why??

    @with_kw mutable struct NNFit{T1<:Int64, T2<:Float64}
        inodes::T1  # input nodes
        hnodes::T1  # hidden nodes
        onodes::T1  # output nodes

        lr::T2  # learning rate
        wih::Matrix{T2}  # (hnodes,inodes) weight matrix between input and hidden
        who::Matrix{T2}  # (onodes,hnodes) weight matrix between hidden and output

        activite_function::Function = x->1.0/(1.0+exp(-x))
    end

    function build_NN(;input_nodes::T1, hidden_nodes::T1, output_nodes::T1,
                       learning_rate::T2,
                       activite_function::Union{Function, Nothing} = nothing
                       ) where {T1<:Int64, T2<:Float64}
        # normal distributions
        NN = NNFit(inodes = input_nodes,
                   hnodes = hidden_nodes,
                   onodes = output_nodes,
                   lr = learning_rate,
                   wih = rand(Normal(0,1.0/sqrt(hidden_nodes)),
                                     hidden_nodes, input_nodes),
                   who = rand(Normal(0,1.0/sqrt(output_nodes)),
                                     output_nodes, hidden_nodes))
        if activite_function != nothing
            NN.activite_function = activite_function
        end
        return NN
    end

    function train!(NN::NNFit{Int64, Float64},
                    input_list::T, target_list::T) where {T<:Vector{Float64}}
        hidden_outputs = NN.activite_function.(NN.wih*input_list)
        final_outputs  = NN.activite_function.(NN.who*hidden_outputs)

        output_errors = target_list .- final_outputs  # output errors
        hidden_errors = NN.who'*output_errors  # hidden errors

        # update weight matrice
        NN.who += NN.lr.*((output_errors.*final_outputs.*(1.0 .- final_outputs))*
                             hidden_outputs')
        NN.wih += NN.lr.*((hidden_errors.*hidden_outputs.*(1.0 .- hidden_outputs))*
                             input_list')
        nothing
    end

    function query(NN::NNFit, input_list::Vector{Float64})
        hidden_outputs = NN.activite_function.(NN.wih*input_list)
        final_outputs  = NN.activite_function.(NN.who*hidden_outputs)

        argmax(final_outputs) - 1
    end
end;  # module NeuralNetwork
