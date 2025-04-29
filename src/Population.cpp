#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
#include <iostream>

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

// NOTE hàm này sắp xếp các cá thể theo thứ tự tăng dần theo fitness
// REVIEW test thử xem có đúng là tăng dần ko?
void Population::sort_individual_by_fitness() {
    std::sort(individuals.begin(), individuals.end(),
    [] (Individual a, Individual b) {
        return a.fitness > b.fitness;
    });
}

void Population::run(int num_generation) {
    for (int i = 0; i < num_generation; i++) {
        compute_fitness();
        std::cout << "Generation " << i << ":\n";
        update_best();
        individuals = reproduce();
    }
}

void Population::compute_fitness() {
    for (Individual individual : individuals) {
        update_fitness(individual);
    }
}

void Population::update_best() {
    best_fitness = 0;
    for (Individual individual : individuals) {
        if (individual.fitness > best_fitness) {
            best_fitness = individual.fitness;
        }
    }
    std::cout << "New best score: " << best_fitness << "\n";
}

void update_fitness(Individual &individual) {
    int max_steps = cf::max_steps;

    SnakeEngine snakeEngine(cf::NumRows, cf::NumCols);
    snakeEngine.newGame();
    FeedForwardNetwork nn = create_from_geoneme(individual.genome);

    for (int i = 0; i < max_steps && snakeEngine.state == GameState::Running; i++) {
        Direction action = get_action(nn, snakeEngine);
        snakeEngine.update(action);
    }
    individual.fitness = snakeEngine.score;
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

//    auto calc_body_dis = [&] (int dx, int dy) {
//        int cx = x + dx, cy = y + dy;    // / Vị trí hiện tại (cx, cy) được tính từ đầu rắn và hướng di chuyển (dx, dy) cy = y + dy;
//        int dist = 1.0;
//        while(cx >= 0 && cx < state.board_width && cy >= 0 && cy < state.board_height) {
//            if(find(state.snake_body.begin(), state.snake_body.end(), make_pair(cx, cy)) != state.snake_body.end()) {
//                return 1.0 / dist;
//            }
//            dist++;
//            cx += dx;
//            cy += dy;
//        }
//        return 0.0;
//}
//    
//    inputs.push_back(calc_body_dis(-1, 0));  // LEFT
//    inputs.push_back(calc_body_dis(1, 0));   // RIGHT
//    inputs.push_back(calc_body_dis(0, -1));  // UP
//    inputs.push_back(calc_body_dis(0, 1));   // DOWN

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