#pragma once

#include <vector>
#include <unordered_map>

#include "Activation.hpp"
#include "GeneticAlgorithm.hpp"

//Tao neuron va gia tri cua neuron
struct NeuronInput{
    int input_id;
    double weight;
};

// REVIEW khoi tao neuron nhu the nao?
struct Neuron{
    int neuron_id;
    double bias;
    Activation activation;
    std::vector<NeuronInput>inputs;
};

//tinh trong so cua neuron o layer sau
class FeedForwardNetwork{

public:
    
    FeedForwardNetwork(std::vector<int> input_ids, std::vector<int> output_ids, std::vector<Neuron> neurons) : 
    input_ids(input_ids), output_ids(output_ids), neurons(neurons) {}

    std::vector<double> activate(const std::vector<double> &inputs);

private:
    std::vector<int> input_ids;
    std::vector<int> output_ids;
    std::vector<Neuron> neurons;

};

bool mapContainsKey(std::unordered_map<int, double>& map, int key);

std::vector<std::vector<int>> feed_forward_layer(std::vector<int> inputs, 
    std::vector<int> outputs, std:: vector<Link_Gene> links);

FeedForwardNetwork create_from_geoneme(Genome &genome);

