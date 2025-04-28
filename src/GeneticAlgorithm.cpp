#include <vector>
#include <cassert>
#include <algorithm>
#include <optional>

#include "Activation.hpp"
#include "GeneticAlgorithm.hpp"
#include "RNG.hpp"
#include <chrono>

NeuronGene crossover_neuron(const NeuronGene &a, const NeuronGene &b, double prob = 0.5) {
    assert(a.neuron_id == b.neuron_id);
    return {
        a.neuron_id,
        RNG::choose(prob, a.bias, b.bias),
        RNG::choose(0.5, a.activation, b.activation),
    };
}

// NOTE lien quan den chon ngau nhien
Link_Gene crossover_linkgene(const Link_Gene &a, const Link_Gene &b, double prob = 0.5){
    assert(a.linkid == b.linkid);
    return {
        a.linkid,
        RNG::choose(prob, a.weight, b.weight),
        RNG::choose(prob, a.enabled, b.enabled),
    };
}

Genome crossover(const Individual &dominant, const Individual &recessive, GenomeIndexer &m_genome_indexer){
    Genome offspring{m_genome_indexer.next_genome(), dominant.genome.num_inputs, dominant.genome.num_outputs};

    //Ke thua neuron genes 
    for(const auto &dominant_neuron: dominant.genome.neurons){
        int neuron_id = dominant_neuron.neuron_id;
        optional<NeuronGene> recessive_neuron = recessive.genome.find_neurons(neuron_id);
        if(!recessive_neuron){
            offspring.add_neuron(dominant_neuron);
        }
        else{
            offspring.add_neuron(crossover_neuron(dominant_neuron, *recessive_neuron));
        }
    }

    // Ke thua neuron link
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

    optional<Link_Gene> existing_link = genome.find_link(Link_ID{input_id, output_id});
    if (existing_link) {
        existing_link->enabled = true;
        return;
    }
       
    vector<Connection> connections;
    if (genome.cycle_check(connections, {input_id, output_id})) {
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
    link_to_split->enabled = false;

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
    auto neuron_to_remove = find_if(genome.neurons.begin(), genome.neurons.end(),
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


// Dot bien khong lam thay doi cau truc
void mutate_weights_and_biases(Genome genome, double weight_mutation_rate, double link_mutation_rate, double delta_range) {
    for(auto& link : genome.links) {
        if(RNG::get_double() < link_mutation_rate) {
            double delta = RNG::get_double_in_range(-delta_range, delta_range);
            link.weight += delta;
        }
    }
}

// Dot bien lam thay doi cau truc
void mutate_structure(Genome &genome, GenomeIndexer &genome_indexer) {
    double prob_add_link = 0.25;
    double prob_remove_link = 0.25;
    double prob_add_neuron = 0.25;
    double prob_remove_neuron = 0.25;

    double r = RNG::get_double();

    if( r < prob_add_link ) {
        mutate_add_link(genome);
    }
    else if( r < prob_add_link + prob_remove_link ) {
        mutate_remove_link(genome);
    }
    else if( r < prob_add_link + prob_remove_link + prob_add_neuron) {
        mutate_add_neuron(genome, genome_indexer);
    }
    else {
        mutate_remove_neuron(genome);
    }

}


// TODO Sau do dinh nghia ham dot bien chung
void mutate(Genome &genome, GenomeIndexer& genome_indexer, 
    double link_mutation_rate, double weight_mutation_rate, double delta_range,
    double add_link_rate, double add_neuron_rate) {
    mutate_weights_and_biases(genome, weight_mutation_rate, link_mutation_rate, delta_range);
    mutate_structure(genome, genome_indexer);
}
