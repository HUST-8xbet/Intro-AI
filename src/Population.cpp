#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include <iostream>
#include <fstream>

#include "Population.hpp"
#include "Neural.hpp"

Genome Population::new_genome() {
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

// REVIEW survival_threshold là tỉ lệ số cá thể được chọn để lai ghép
// Ở đây không giữ lại cá thể nào cho thế hệ sau mà tất cả đều được sinh ra từ lai ghép
// Liệu có cần giữ lại một phần cá thể?
std::vector<Individual> Population::reproduce() {
    sort_individual_by_fitness();
    int reproduction_cutoff = ceil(individuals.size() * cf::survival_threshold);
    std::vector<Individual> new_generation;

    for (int i = 0; i < cf::population_size; i++) {
        auto p1 = RNG::select_randomly(individuals.begin(), individuals.begin() + reproduction_cutoff);
        auto p2 = RNG::select_randomly(individuals.begin(), individuals.begin() + reproduction_cutoff);

        Genome offspring = (p1->fitness > p2->fitness) ? 
            crossover(*p1, *p2, m_genome_indexer) : crossover(*p2, *p1, m_genome_indexer);
        
        mutate(offspring, m_genome_indexer);
        new_generation.emplace_back(Individual{std::move(offspring), 0.0});
    }
    return new_generation;
}

// NOTE hàm này sắp xếp các cá thể theo thứ tự giảm dần theo fitness
// REVIEW test thử xem có đúng là giảm dần ko?
void Population::sort_individual_by_fitness() {
    std::sort(individuals.begin(), individuals.end(),
    [] (Individual a, Individual b) {
        return a.fitness > b.fitness;
    });
}

void Population::run(int num_generation, int broad_rows, int broad_cols, int max_steps_since_last_food) {
    for (int i = 0; i < num_generation; i++) {
        compute_fitness(broad_cols, broad_rows, max_steps_since_last_food);
        std::cout << "Generation " << current_generation << ":\n";
        update_best();
        individuals = reproduce();
        current_generation++;
    }
}

void Population::compute_fitness(int broad_rows, int broad_cols, int max_steps_since_last_food) {
    for (Individual &individual : individuals) {
        update_fitness(individual, broad_cols, broad_rows, max_steps_since_last_food);
    }
}

void Population::update_best() {
    double sum = 0.0;
    best_fitness = individuals[0].fitness;
    for (Individual &individual : individuals) {
        if (individual.fitness > best_fitness) {
            best_fitness = individual.fitness;
        }
        sum += individual.fitness;
    }
    std::cout << "Best score: " << best_fitness << "\n";
    std::cout << "Average score: " << sum/(double)cf::population_size << "\n";
}

double calculate_fitness(SnakeEngine &snakeEngine) {
    double fitness = 0.0;
    if (snakeEngine.state == GameState::GameOver) {
        fitness -= 5.0;
    }
    fitness += snakeEngine.score;
    return fitness;
}

void update_fitness(Individual &individual, int broad_rows, int broad_cols, int max_steps_since_last_food) {
    SnakeEngine snakeEngine(broad_cols, broad_rows);
    snakeEngine.newGame();
    FeedForwardNetwork nn = create_from_geoneme(individual.genome);

    while(snakeEngine.step_since_last_food < max_steps_since_last_food &&
             snakeEngine.state == GameState::Running) {
        Direction action = get_action(nn, snakeEngine);
        snakeEngine.update(action);
    }
    individual.fitness = calculate_fitness(snakeEngine);
}

// Hàm debug
void print_input(std::vector<double> input) {
    std::cout << "Input: {";
    for (double i : input) {
        std::cout << i << " ";
    }
    std::cout << "}\n";
}

// Hàm debug
void print_output(std::vector<double> output) {
    std::cout << "Output: {";
    for (double i : output) {
        std::cout << i << " ";
    }
    std::cout << "}\n";
}

// NOTE số lượng output ở đây là 4 tương ứng với 4 hướng quy định
// quy định bằng num_output trong file config
Direction get_action(FeedForwardNetwork &nn, SnakeEngine &snakeEngine) {
    std::vector<double> inputs = extract_inputs(snakeEngine);
//    print_input(inputs);
    std::vector<double> outputs = nn.activate(inputs);
//    print_output(outputs);
    int max_id = 0;
    for (int i = 0; i < outputs.size(); i++) {
        if (outputs[max_id] < outputs[i]) {
            max_id = i;
        }
    }

    switch (max_id)
    {
    case 0:
//        std::cout << "Go Up!\n";
        return Direction::Up;
        break;
    case 1:
//        std::cout << "Go Down!\n";
        return Direction::Down;
        break;
    case 2:
//        std::cout << "Go Left!\n";
        return Direction::Left;
        break;
    case 3:
//        std::cout << "Go Down!\n";
        return Direction::Right;
        break;
    default:
//        std::cout << "Default (Up)\n";
        return Direction::Up;
        break;
    }
}

// REVIEW còn phần khoảng cách đến thân rắn do chưa hiểu rõ nên chưa thêm
// NOTE chú ý số lượng input phải bằng số num_input ghi trong file config.hpp
// Xem thêm chú ý ở file SnakeEngine.hpp để hiểu logic game con rắn
std::vector<double> extract_inputs(const SnakeEngine &state) {
    std::vector<double> inputs;
    // Lý do chuẩn hóa: Khoảng cách càng gần, giá trị 1.0 / dist càng lớn (rắn càng gần, mạng nơ-ron sẽ thấy nguy hiểm hơn).
    Coordinates head = state.snakeBody.front();

    auto calc_body_dis = [&] (int dx, int dy) {
        Coordinates next_pos = {head.col + dx, head.row + dy};    // / Vị trí hiện tại (cx, cy) được tính từ đầu rắn và hướng di chuyển (dx, dy) cy = y + dy;
        double dist = 1.0;
        while(!state.hitBody(next_pos) && !state.hitWall(next_pos)) {
            dist++;
            next_pos.col += dx;
            next_pos.row += dy;
        }
        return 1.0 / dist;
    };
    
    inputs.push_back(calc_body_dis(0, -1));  // UP
    inputs.push_back(calc_body_dis(0, 1));   // DOWN
    inputs.push_back(calc_body_dis(-1, 0));  // LEFT
    inputs.push_back(calc_body_dis(1, 0));   // RIGHT

    // Chuẩn hoá input về 0, 1 
    // Ví dụ nếu đầu rắn ở gần mép trái thì input nhỏ 
    // Thong tin ve khoang cach den tuong ;
    inputs.push_back( (double) head.row /(double) (state.rows-1) );   // UP 
    inputs.push_back( (double) (state.rows-1 - head.row) / (double) (state.rows-1) );   // DOWN 
    inputs.push_back( (double) head.col / (double) (state.cols-1) );   // LEFT
    inputs.push_back( (double) (state.cols-1 - head.col) / (double) (state.cols-1) );   // RIGHT 

    // Thong tin ve vi tri thuc an 
    inputs.push_back((state.food.col == head.col && state.food.row < head.row) ? 1.0 : 0.0);    // Food on up
    inputs.push_back((state.food.col == head.col && state.food.row > head.row) ? 1.0 : 0.0);    // Food on down
    inputs.push_back((state.food.row == head.row && state.food.col < head.col) ? 1.0 : 0.0);    // Food on the left
    inputs.push_back((state.food.row == head.row && state.food.col > head.col) ? 1.0 : 0.0);    // Food on the right


    // Thong tin ve huong hien tai 
    inputs.push_back((state.headDirection == Direction::Up) ? 1.0 : 0.0);
    inputs.push_back((state.headDirection == Direction::Down) ? 1.0 : 0.0);
    inputs.push_back((state.headDirection == Direction::Left) ? 1.0 : 0.0);
    inputs.push_back((state.headDirection == Direction::Right) ? 1.0 : 0.0);

    return inputs;
}

