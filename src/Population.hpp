#include <vector>

#include "config.hpp"
#include "GeneticAlgorithm.hpp"
#include "SnakeEngine.hpp"
using namespace std;


class Population
{
public:
    Population() {
        for (int i = 0; i < cf::population_size; i++) {
            individuals.push_back({new_genome(), 0});
        }
    }

private:
    vector<Individual> individuals;
    GenomeIndexer m_genome_indexer;
    double best_fitness;

    Genome new_genome() {
        Genome genome{m_genome_indexer.next_genome(), cf::num_input, cf::num_output};
        for (int neuron_id = 0; neuron_id < cf::num_output; neuron_id++) {
            // NOTE co khoi tao neuron
            genome.neurons.push_back(NeuronGene{neuron_id, 0.0, relu{}});
        }

        // Genome moi khoi tao co day du lien ket tu input toi output
        for (int i = 0; i< cf::num_input; i++) {
            int input_id = -i - 1;
            for (int output_id = 0; output_id < cf::num_output; output_id++) {
                genome.links.push_back(Link_Gene{Link_ID{input_id, output_id}, 0.0, true});
            }
        }
        return genome;
    }
    
    vector<Individual> reproduce() {

    }

    void update_best() {

    }

    void update_fitness(Individual &Individual) {

    }
};

// TODO chuan bi dau vao cho mang neuron
vector<double> extract_inputs(const SnakeEngine &snake_engine) {
    
}
