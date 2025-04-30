#pragma once

#include <algorithm>
#include <optional>
#include <iostream>
#include <unordered_set>

#include <cereal/types/vector.hpp>
#include <cereal/access.hpp>

#include "RNG.hpp"
#include "Activation.hpp"
#include "config.hpp"


// NOTE Chú ý quan trọng về việc đánh ID các neuron
// Các neuron input có ID là số âm từ -1, -2,... đến -num_input (xem file config)
// Các neuron output có ID từ 0, 1,... đến num_ouput-1
// Các neuron hidden có ID bắt đầu num_output
// và được đánh số tiếp mỗi khi xảy ra đột biến thêm neuron (xem hàm mutate_add_neuron)

// NOTE Trong vector<NeuronGene> neurons của genome KHÔNG lưu các neuron input
// bởi vì các neuron input không cần tính giá trị kích hoạt mà lấy bằng input luôn
// cho nên nó cũng không có bias và cũng không cần lưu

//Lai ghep GA Neuron

struct NeuronGene {
    int neuron_id;
    double bias;
    Activation activation;

    template<class Archive>
    void serialize(Archive & archive)
    {
    archive(CEREAL_NVP(neuron_id),
            CEREAL_NVP(bias),
            CEREAL_NVP(activation));
    }

    bool operator==(const NeuronGene &other) const{
        return neuron_id == other.neuron_id;
    }
};

//Synurp trong mang
struct Link_ID{ 
    int input_id;
    int output_id;

    template<class Archive>
    void serialize(Archive & archive)
    {
    archive(CEREAL_NVP(input_id),
            CEREAL_NVP(output_id));
    }

    bool operator==(const Link_ID &other) const{
        return (input_id == other.input_id) && (output_id == other.output_id);
    }
};

struct Link_Gene{
    Link_ID linkid;
    double weight;
    bool is_enable;

    template<class Archive>
    void serialize(Archive & archive)
    {
    archive(CEREAL_NVP(linkid),
            CEREAL_NVP(weight),
            CEREAL_NVP(is_enable));
    }

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

    template<class Archive>
    void serialize(Archive & archive)
    {
    archive(CEREAL_NVP(genome_id),
            CEREAL_NVP(num_inputs),
            CEREAL_NVP(num_outputs),
            CEREAL_NVP(neurons),
            CEREAL_NVP(links));
    }

    bool would_create_cycle(const int input_id, const int output_id) const {
        if(input_id == output_id) return true; // Neu input va output giong nhau thi ko co vong lap

        std:: unordered_set<int> visited;
        visited.insert(output_id); // Danh dau input da duoc tham
        while(1){
            int num_added = 0;
            for(auto &link : links){
                if(visited.count(link.linkid.input_id) && !visited.count(link.linkid.output_id)){
                    if(link.linkid.output_id == input_id) return true; // Neu tim thay vong lap
                    visited.insert(link.linkid.output_id); // Danh dau output da duoc tham
                    num_added++;
                }
            }
            if (num_added == 0)  return false; // Neu ko co link nao duoc them thi ko co vong lap
            
        };
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

    template<class Archive>
    void serialize(Archive & archive)
    {
    archive(CEREAL_NVP(genome),
            CEREAL_NVP(fitness));
    }
};

// class tao Chi so cho genome ko giong id bo me
class GenomeIndexer {
private:
    int current_genome_id;
    int current_neuron_id; // bat dau tu num_ouput

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive & archive)
    {
        archive(CEREAL_NVP(current_genome_id),
                CEREAL_NVP(current_neuron_id));
    }
public:
    GenomeIndexer() : current_genome_id(0), current_neuron_id(cf::num_output) {}
    GenomeIndexer(int current_genome_id, int current_neuron_id) :
        current_genome_id(current_genome_id),
        current_neuron_id(current_neuron_id) {}

    int next_genome() { 
        return current_genome_id++;  // Cấp phát ID mới, sau đó tăng current_id
    }
    int next_neuron() { 
        return current_neuron_id++;  // Cấp phát ID mới, sau đó tăng current_id
    }
};

Genome crossover(const Individual &dominant, const Individual &recessive, GenomeIndexer &m_genome_indexer);

void mutate(Genome &genome, GenomeIndexer& genome_indexer);