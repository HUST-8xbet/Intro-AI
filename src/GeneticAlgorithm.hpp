#pragma once

#include <random>
#include <vector>
#include <algorithm>
#include <optional>
#include <iostream>

#include "RNG.hpp"
#include "Activation.hpp"
#include "config.hpp"

//Lai ghep GA Neuron

struct NeuronGene {
    int neuron_id;
    double bias;
    Activation activation;
    bool operator==(const NeuronGene &other) const{
        return neuron_id == other.neuron_id;
    }
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

    bool has_neuron(const NeuronGene &neuron) {
        return (neuron.neuron_id == linkid.input_id || neuron.neuron_id == linkid.output_id);
    }
};

struct Genome{
    int genome_id;
    int num_inputs;
    int num_outputs;
    std:: vector<NeuronGene> neurons;
    std:: vector<Link_Gene> links;

    // TODO can viet ham kiem tra vong lap
    bool would_create_cycle(const int input_id, const int output_id) const {
        return false;
    }

    std::vector<int> make_input_ids() const {
        std::vector<int> input_ids;
        for (int i = 0; i < num_inputs; i++) {
            input_ids.push_back(-i-1);
        }
        return input_ids;
    }

    std::vector<int> make_output_ids() const {
        std::vector<int> output_ids;
        for (int i = 0; i < num_outputs; i++) {
            output_ids.push_back(i);
        }
        return output_ids;
    }

    // REVIEW 2 ham add nay co nen bao gom viec check gen trung lap hay ko?
    //        hay la viec check gen trung lap la nhiem vu cua cac ham su dung no?
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

    // NOTE lien quan den chon ngau nhien
    int choose_random_input_or_hidden() const {
        std::vector<int> input_or_hidden_ids = make_input_ids();
        for (const NeuronGene& neuron : neurons) {
            if (neuron.neuron_id >= num_outputs)
                input_or_hidden_ids.emplace_back(neuron.neuron_id);
        }
        return *RNG::select_randomly(input_or_hidden_ids.begin(), input_or_hidden_ids.end());
    }

    // NOTE lien quan den chon ngau nhien
    int choose_random_output_or_hidden() const {
        return RNG::select_randomly(neurons.begin(), neurons.end())->neuron_id;
    }

    // NOTE lien quan den chon ngau nhien
    int choose_random_hidden() const {
        std::vector<int> hidden_ids;
        for (const NeuronGene& neuron : neurons) {
            if (neuron.neuron_id >= num_outputs)
                hidden_ids.emplace_back(neuron.neuron_id);
        }
        return *RNG::select_randomly(hidden_ids.begin(), hidden_ids.end());
    }
};

struct Individual{
    Genome genome;
    double fitness;
};

// class tao Chi so cho genome ko giong id bo me
class GenomeIndexer {
    private:
        int current_genome_id;
        int current_neuron_id; // bat dau tu num_ouput
    public:
        GenomeIndexer() : current_genome_id(0), current_neuron_id(cf::num_output) {}
    
        int next_genome() { 
            return current_genome_id++;  // Cấp phát ID mới, sau đó tăng current_id
        }
        int next_neuron() { 
            return current_neuron_id++;  // Cấp phát ID mới, sau đó tăng current_id
        }
};

    
//Dinh nghia ham random 
//class RNG {
//    public:
//        RNG() : gen(std::random_device{}()) {}
//    
//        template <typename T>
//        T choose(double prob, const T &val1, const T &val2) {
//            std::bernoulli_distribution d(prob); // Phan phoi nhi thuc
//            return d(gen) ? val1 : val2;
//        }
//
//    private:
//        std::mt19937 gen;
//    };
//extern RNG rng;
