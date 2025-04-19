#ifndef GENOME_HPP   
#define GENOME_HPP 

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
#include"Activation.hpp"
#include<map>
#include<cstdlib>
#include"GeneticAlgorithm.hpp"
struct Genome{
    int genome_id;
    int num_inputs;
    int num_outputs;
    std:: vector<NeuronGene> neurons;
    std:: vector<Link_Gene> links;

    void add_link(const Link_Gene& newlink){
        auto it = std::find_if(links.begin(), links.end(),
        [&newlink](const Link_Gene& link){
            return link.linkid == newlink.linkid;
        });

        if(it == links.end()){
            links.push_back(newlink);
        }
        else{
            std::cout << "Link with ID (" << newlink.linkid.input_id << ", " << newlink.linkid.output_id
            << ") already exists!" << std::endl;
        }
    }
    void add_neuron(const NeuronGene& newNeuron){
        auto it = std::find_if(neurons.begin(), neurons.end(),
        [&newNeuron](const NeuronGene& neuron) {
            return neuron.neuron_id == newNeuron.neuron_id;
        });

        if(it == neurons.end()){
            neurons.push_back(newNeuron);
        }
        else{
            std::cout << "Neuron with ID " << newNeuron.neuron_id << " already exists!" << std::endl;
        }
    }

    std:: optional<NeuronGene> find_neurons(int neuron_id) const{
        auto it = std::find_if(neurons.begin(), neurons.end(),
        [neuron_id](const NeuronGene& neuron) {
            return neuron.neuron_id == neuron_id;
        });
        if(it != neurons.end()){
            return *it;
        }
        return std::nullopt;
    }

    std:: optional<Link_Gene> find_link(const Link_ID& link_id) const{
        auto it = std::find_if(links.begin(), links.end(),
        [link_id](const Link_Gene& link){
            return link.linkid == link_id;
        });
        if(it != links.end()){
            return *it;
        }
        return std::nullopt;
    }

    static int choose_random_input_or_hidden(const std::vector<NeuronGene> &neurons) {

        if(neurons.empty()){
            exit(1);
        }
        
        int random_index = rand() % neurons.size();
        return neurons[random_index].neuron_id;
    }
};

#endif