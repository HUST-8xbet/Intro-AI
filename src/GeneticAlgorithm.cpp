#include <vector>
#include <cassert>
#include <algorithm>
#include <optional>

#include "Activation.hpp"
#include "GeneticAlgorithm.hpp"
#include "RNG.hpp"

// NOTE lien quan den chon ngau nhien
//Lai ghep neuron con dua tren thuoc tinh cha me
NeuronGene crossover_neuron(const NeuronGene &a, const NeuronGene &b){
    assert(a.neuron_id == b.neuron_id);
    int neuron_id = a.neuron_id;
    double bias = RNG::choose(0.5, a.bias, b.bias);
    Activation activation  = RNG::choose(0.5, a.activation, b.activation); 
    
    return {neuron_id, bias, activation};
}

// NOTE lien quan den chon ngau nhien
Link_Gene crossover_linkgene(const Link_Gene &a, const Link_Gene &b){
    assert(a.linkid == b.linkid);
    Link_ID linkid = a.linkid;
    double weight = RNG::choose(0.5, a.weight, b.weight);
    bool is_enable = RNG::choose(0.5, a.is_enable, b.is_enable);
    
    return {linkid, weight, is_enable};
}

Genome crossover(const Individual &dominant, const Individual &recessive, GenomeIndexer &m_genome_indexer){
    Genome offspring{m_genome_indexer.next_genome(), dominant.genome.num_inputs, dominant.genome.num_outputs};
    //Ke thua neuron genes 
    for(const auto &dominant_neuron: dominant.genome.neurons){
        int neuron_id = dominant_neuron.neuron_id;
        std::optional<NeuronGene> recessive_neuron = recessive.genome.find_neurons(neuron_id);
        if(!recessive_neuron){
            offspring.add_neuron(dominant_neuron);
        }
        else{
            offspring.add_neuron(crossover_neuron(dominant_neuron, *recessive_neuron));
        }
    }
    for(const auto &dominant_link : dominant.genome.links){
        Link_ID link_id = dominant_link.linkid;
        std::optional<Link_Gene> recessive_link = recessive.genome.find_link(link_id);

        if(!recessive_link){
            offspring.add_link(dominant_link);
        }
        else{
            offspring.add_link(crossover_linkgene(dominant_link, *recessive_link));
        }
    }
    return offspring;
}

void mutate_add_link(Genome &genome) {
    int input_id = genome.choose_random_input_or_hidden();
    int output_id = genome.choose_random_output_or_hidden();

    std::optional<Link_Gene> existing_link = genome.find_link(Link_ID{input_id, output_id});
    if (existing_link) {
        existing_link->is_enable = true;
        return;
    }

    if (genome.would_create_cycle(input_id, output_id)) {
        return;
    }

    genome.add_link(Link_Gene{Link_ID{input_id, output_id}, 0.0, true});
}

// NOTE lien quan den chon ngau nhien
void mutate_remove_link(Genome &genome) {
    if (genome.links.empty()) {
        return;
    }

    auto link_to_remove = RNG::select_randomly(genome.links.begin(), genome.links.end());
    genome.links.erase(link_to_remove);
}

// NOTE lien quan den chon ngau nhien
void mutate_add_neuron(Genome &genome, GenomeIndexer &m_genome_indexer) {
    if (genome.links.empty()) {
        return;
    }

    auto link_to_split = RNG::select_randomly(genome.links.begin(), genome.links.end());
    link_to_split->is_enable = false;

    // NOTE co khoi tao neuron
    NeuronGene new_neuron{m_genome_indexer.next_neuron(), 0.0, relu{}};
    genome.neurons.push_back(new_neuron);

    Link_ID link_id = link_to_split->linkid;
    double weight = link_to_split->weight;

    genome.links.push_back(Link_Gene{{link_id.input_id, new_neuron.neuron_id}, 1.0, true});
    genome.links.push_back(Link_Gene{{new_neuron.neuron_id, link_id.output_id}, weight, true});
}

void mutate_remove_neuron(Genome &genome) {
    if (genome.neurons.empty()) {
        return;
    }

    int remove_id = genome.choose_random_hidden();
    auto neuron_to_remove = std::find_if(genome.neurons.begin(), genome.neurons.end(),
        [remove_id](const NeuronGene& neuron) {
            return neuron.neuron_id == remove_id;
        });

    genome.links.erase(
        remove_if(genome.links.begin(), genome.links.end(),
            [neuron_to_remove] (Link_Gene &link) {
                return link.has_neuron(*neuron_to_remove);
            }
        ), genome.links.end());
    genome.neurons.erase(neuron_to_remove);
}

// TODO Thieu phan dot bien khong lam thay doi cau truc

// TODO Sau do dinh nghia ham dot bien chung
void mutate(Genome &genome) {
    
}
