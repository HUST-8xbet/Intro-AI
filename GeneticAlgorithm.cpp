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
#include<map>
#include<cstdlib>
#include"Activation.hpp"
#include"GeneticAlgorithm.hpp"
using namespace std;

//Lai ghep neuron con dua tren thuoc tinh cha me
NeuronGene crossover_neuron(const NeuronGene &a, const NeuronGene &b){
    assert(a.neuron_id == b.neuron_id);
    int neuron_id = a.neuron_id;
    double bias = rng.choose(0.5, a.bias, b.bias);
    Activation activation  = rng.choose(0.5, a.activation, b.activation); 
    
    return {neuron_id, bias, activation};
}

Link_Gene crossover_linkgene(const Link_Gene &a, const Link_Gene &b){
    assert(a.linkid == b.linkid);
    Link_ID linkid = a.linkid;
    double weight = rng.choose(0.5, a.weight, b.weight);
    bool is_enable = rng.choose(0.5, a.is_enable, b.is_enable);
    
    return{linkid, weight, is_enable};
}

Genome crossover(const Individual &dominant, const Individual &recessive){
    Genome offspring{m_genome_indexer.next(), dominant.genome.num_inputs, dominant.genome.num_outputs};
    //Ke thua neuron genes 
    for(const auto &dominant_neuron: dominant.genome.neurons){
        int neuron_id = dominant_neuron.neuron_id;
        

    }
}