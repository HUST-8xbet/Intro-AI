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
    auto[x, y] state.snake_head;

    auto calc_body_dis = [&] (int dx, int dy) {
        int cx = x + dx, cy = y + dy;    // / Vị trí hiện tại (cx, cy) được tính từ đầu rắn và hướng di chuyển (dx, dy) cy = y + dy;
        int dist = 1.0;
        while(cx >= 0 && cx < state.board_width && cy >= 0 && cy < state.board_height) {
            if(find(state.snake_body.begin(), state.snake_body.end(), make_pair(cx,cy) != state.snake_body.end())) {
                return 1.0 / dist;
            }
            dist++;
            cx += dx;
            cy += dy;
        }
        return 0.0;
}


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
    
}
