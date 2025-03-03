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
#include <cassert>
#include 
using namespace std;
//Tao neuron va gia tri cua neuron
struct NeuronInput{
    int input_id;
    double weight;
};
struct Neuron{
    ActivationFn Activation;
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
        for(int input_ids: output_ids){
            values[output_id] = 0;           //Neuron output dc gan = 0 
        }
    }



    private:
    vector<int> input_ids;
    vector<int> output_ids;
    vector<Neuron> neurons;

};