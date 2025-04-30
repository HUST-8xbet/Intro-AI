#pragma once

#include <vector>

#include "config.hpp"
#include "GeneticAlgorithm.hpp"
#include "Neural.hpp"
#include "SnakeEngine.hpp"

// NOTE population (quần thể) là tập hợp các individual (cá thể)
// Hàm run() sẽ mô phỏng quần thể qua num_generation thế hệ
// bằng cách tính fitness (độ thích nghi)
// rồi sau đó chọn ra các individual có finess cao nhất để reproduce() thế hệ tiếp theo
// Độ thích nghi cao nhất được cập nhập vào best_fitness

class Population
{
public:
    Population() {
        for (int i = 0; i < cf::population_size; i++) {
            individuals.push_back(Individual{new_genome(), 0.0});
            best_fitness = 0.0;
        }
    }
    Population(std::vector<Individual> i, GenomeIndexer m, double b) : 
        individuals(i),
        m_genome_indexer(m),
        best_fitness(b) {}

    void run(int num_generation);

private:
    std::vector<Individual> individuals;
    GenomeIndexer m_genome_indexer;
    double best_fitness;

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive & archive)
    {
        archive(CEREAL_NVP(individuals),
                CEREAL_NVP(m_genome_indexer),
                CEREAL_NVP(best_fitness));
    }

    Genome new_genome();
    
    std::vector<Individual> reproduce();

    void compute_fitness();

    void update_best();

    void sort_individual_by_fitness();

};

void update_fitness(Individual &Individual);
void print_input(std::vector<double> input);
void save_to_json(Population &population);

std::vector<double> extract_inputs(const SnakeEngine &snake_engine);

Direction get_action(FeedForwardNetwork &nn, SnakeEngine &snakeEngine);

