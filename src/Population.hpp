#include <vector>

#include "config.hpp"
#include "GeneticAlgorithm.hpp"
#include "SnakeEngine.hpp"
#include<RNG.hpp>
#include<config.hpp>
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

    // Sinh the he tiep theo
    void evole_new_generation() {
        for(auto& ind : individuals) {
            update_fitness();
        }

        update_best();
        individuals = reproduce();
    }


    void update_fitness(Individual& ind) {
        SnakeEngine engine(cf::NumRows, cf::Num);
        engine.newGame();

        GenomeEvaluator evaluator(ind.genome);

        int score = 0, steps = 0; 
        while(!engine::GameOver() && steps < cf::max_steps) {
            vector<double> inputs = extract_inputs(engine);
            vector<double> outputs = evaluator.evaluate(inputs);
        }
    }


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
        vector<Individual> new_generation;
        // Chọn ra ngẫu nhiên các các thể từ dân số hiện tại và tạo ra cá thể mới 
        new_generation.push_back(individuals[0]);
        for( int i = 0; i < cf::population_size; i++ ) {

            int parent1_index = RNG::get_int_in_range(0, individuals.size() - 1);
            int parent2_index = RNG::get_int_in_range(0, individuals.size() - 1);

            Individual parent1 = individuals[parent1_index];
            Individual parent2 = individuals[parent2_index];

            if(parent2.fitness > parent1.fitness) 
                swap(parent1, parent2);
            
            // Crossover 
            Genome child_genome = crossover(parent1, parent2, m_genome_indexer);

            // Mutation
            mutate(child_genome, m_genome_indexer, cf::link_mutation_rate, 
                    cf::weight_mutation_rate, 
                    cf::delta_range, 
                    cf::add_link_rate,  
                    cf::add_neuron_rate);
            new_generation.push_back(make_pair(child_genome, 0));  //Fitness se duoc tinh sau
        }
        return new_generation;
    }

    void update_best() {
        if(individuals.empty()) return;
        best_fitness = individuals[0].fitness;
        for(auto& ind : individuals) {
            if(ind.fitness > best_fitness) {
                best_fitness = ind.fitness;
            }
        }
        
        // Sort the individuals based on fitness (giảm dần)
        sort(individuals.begin(), individuals.end(), [](const Individual& a, const Individual& b) {
            return a.fitness > b.fitness;
        }
       );
    }


// Trang thai con ran 
struct SnakeState {
    pair<int, int> snake_head; // Luu vi tri dau con ran
    vector< pair<int, int> > snake_body;  // Luu vi tri than con ran
    pair<int, int> food_pos;      // Vi tri thuc an
    int board_width;
    int board_height;
    enum class Direction {UP, DOWN, LEFT, RIGHT};
    Direction cur_direction;   // Huong hien tai cua con ran     
};


// Ham kiem tra xem co gap than ran khong
vector<double> get_inputs(const SnakeState& state) {
    vector<double> inputs;
    // Lý do chuẩn hóa: Khoảng cách càng gần, giá trị 1.0 / dist càng lớn (rắn càng gần, mạng nơ-ron sẽ thấy nguy hiểm hơn).
    auto[x, y] = state.snake_head;

    auto calc_body_dis = [&] (int dx, int dy) {
        int cx = x + dx, cy = y + dy;    // / Vị trí hiện tại (cx, cy) được tính từ đầu rắn và hướng di chuyển (dx, dy) cy = y + dy;
        int dist = 1.0;
        while(cx >= 0 && cx < state.board_width && cy >= 0 && cy < state.board_height) {
            if(find(state.snake_body.begin(), state.snake_body.end(), make_pair(cx, cy)) != state.snake_body.end()) {
                return 1.0 / dist;
            }
            dist++;
            cx += dx;
            cy += dy;
        }
        return 0.0;
}
    
    inputs.push_back(calc_body_dis(-1, 0));  // LEFT
    inputs.push_back(calc_body_dis(1, 0));   // RIGHT
    inputs.push_back(calc_body_dis(0, -1));  // UP
    inputs.push_back(calc_body_dis(0, 1));   // DOWN


    // Chuẩn hoá input về 0, 1 
    // Ví dụ nếu đầu rắn ở gần mép trái thì input nhỏ 
    // Thong tin ve khoang cach den tuong 
    inputs.push_back( (double)x / state.board_width );   // LEFT
    inputs.push_back( (double)(state.board_width - x) / state.board_width );   // RIGHT 
    inputs.push_back( (double)y / state.board_height );   // UP 
    inputs.push_back( (double)(state.board_height -y) / state.board_height );   // DOWN 

    // Thong tin ve vi tri thuc an 
    inputs.push_back(state.food_pos.first < x ? 1.0 : 0.0);     // Food on the left
    inputs.push_back(state.food_pos.first > x ? 1.0 : 0.0);     // Food on the right
    inputs.push_back(state.food_pos.second < y ? 1.0 : 0.0);    // Food on up
    inputs.push_back(state.food_pos.second > y ? 1.0 : 0.0);    // Food on down

    // Thong tin ve huong hien tai 
    inputs.push_back(state.cur_direction == SnakeState::Direction::LEFT ? 1.0 : 0.0);
    inputs.push_back(state.cur_direction == SnakeState::Direction::RIGHT ? 1.0 : 0.0);
    inputs.push_back(state.cur_direction == SnakeState::Direction::UP ? 1.0 : 0.0);
    inputs.push_back(state.cur_direction == SnakeState::Direction::DOWN ? 1.0 : 0.0);
    
    return inputs;
}



// TODO chuan bi dau vao cho mang neuron
vector<double> extract_inputs(const SnakeEngine &snake_engine) {

    // Gán thông tin từ SnakeEngine vào SnakeState
    SnakeState state;

    state.board_height = snake_engine.rows;
    state.board_width = snake_engine.cols;
    state.food_pos = make_pair(snake_engine.food.col, snake_engine.food.row);
    state.snake_head = make_pair(snake_engine.snakeBody.front().col, snake_engine.snakeBody.front().row);

    state.snake_body.clear();
    // size_t ko bao gio am
    for( size_t i = 1; i < snake_engine.snakeBody.size(); i++ ) {
        state.snake_body.push_back( make_pair(snake_engine.snakeBody[i].col, snake_engine.snakeBody[i].row) );
    }

    switch(snake_engine.headDirection) {
        case Direction::Left : 
            state.cur_direction = SnakeState::Direction::LEFT;
            break;
        case Direction::Right : 
            state.cur_direction = SnakeState::Direction::RIGHT;
            break;
        case Direction::Up : 
            state.cur_direction = SnakeState::Direction::UP;
            break;
        case Direction::Down : 
            state.cur_direction = SnakeState::Direction::DOWN;
            break;
    }
    return get_inputs(state);
}

