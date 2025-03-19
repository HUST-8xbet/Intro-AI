#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<chrono>
#include<random>
#include<mutex>
#include<thread>
#include<vector>
#include<list>
#include<unordered_map>
#include<queue>
#include<future>
#include<cassert>
#include<array>
#include<variant>
#include<map>
#include"Activation.hpp"
#include"GeneticAlgorithm.hpp"
using namespace std;

//Tao neuron va gia tri cua neuron
struct NeuronInput{
    int input_id;
    double weight;
};
struct Neuron{
    int neuron_id;
    Activation activation;
    double bias;
    vector<NeuronInput>inputs;
};

//tinh trong so cua neuron o layer sau
class FeedForwardNetwork{
    public:
    vector<double> activate(const vector<double> &inputs){
        assert(inputs.size() == input_ids.size());
        unordered_map<int, double>values;

        for(int i = 0;i < inputs.size();i++){
            int input_id = input_ids[i];
        values[input_id] = inputs[i];
        }

        for(int output_id: output_ids){
            values[output_id] = 0.0;           //Neuron output dc gan = 0 
        }

        for(const auto &neuron : neurons){
            double value =0.0;
            for(NeuronInput input: neuron.inputs){
                assert(values.contains(input.input_id));
                value += values[input.input_id] * input.weight;
            }

            value += neuron.bias;
            value = ActivationFn{}(neuron.activation, value);
;
            values[neuron.neuron_id] = value;
        }
        
        vector<double> outputs;
        for(int output_id: output_ids){
            assert(values.contains(output_id));
            outputs.push_back(values[output_id]);
        }
        return outputs;

    }

    private:
    vector<int> input_ids;
    vector<int> output_ids;
    vector<Neuron> neurons;

};

static FeedForwardNetwork create_from_geoneme(const Genome &geoneme){
    
}