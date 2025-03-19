#ifndef GENETIC_ALGO_HPP   
#define GENETIC_ALGO_HPP 


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


//Lai ghep GA Neuron

struct NeuronGene{
    int neuron_id;
    double bias;
    Activation activation;
};
//Synurp trong mang
struct Link_ID{ 
    int input_id;
    int output_id;
    bool operator==(const Link_ID &other) const{
        return (input_id == other.input_id) && (output_id == other.output_id);
    }
};

struct Link_Gene{
    Link_ID linkid;
    double weight;
    bool is_enable;

    bool operator==(const Link_Gene &other) const{
        return (linkid == other.linkid);
    }
};

struct Genome{
    int genome_id;
    int num_inputs;
    int num_outputs;
    std:: vector<NeuronGene> neurons;
    std:: vector<Link_Gene> links;

};

struct Individual{
    Genome genome;
    double fitness;
};

// class tao Chi so cho genome ko giong id bo me
class GenomeIndexer {
    private:
        int current_id;
    public:
        GenomeIndexer() : current_id(0) {}
    
        int next() { 
            return current_id++;  // Cấp phát ID mới, sau đó tăng current_id
        }
    };
extern GenomeIndexer m_genome_indexer;
    
 //Dinh nghia ham random 
class RNG {
    public:
        RNG() : gen(std::random_device{}()) {}
    
        template <typename T>
        T choose(double prob, const T &val1, const T &val2) {
            std::bernoulli_distribution d(prob); // Phan phoi nhi thuc
            return d(gen) ? val1 : val2;
        }
    
    private:
        std::mt19937 gen;
    };
extern RNG rng;



#endif