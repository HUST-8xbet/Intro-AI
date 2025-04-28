#include <vector>
#include <unordered_map>
#include <cassert>
#include "Activation.hpp"
#include "GeneticAlgorithm.hpp"
#include <set>
using namespace std;

// Đại diện cho mỗi xi-nap 
struct Neuron {
    int neuron_id;
    int bias;
    Activation activation;
};

// Đại diện cho các kết nối
struct Link {
    int input_id;
    int output_id;
    double weight;
    bool enabled;
    int innovation_num;

    bool operator==(const Link& other) const {
        return innovation_num == other.innovation_num;
    }
};

// Đại diện cho một mạng neuron 
struct Genome {
    int genome_id;     // Mỗi cá thể trong một mạng 
    int num_inputs;    // Kích thước đầu vào 
    int num_outputs;   // Kích thước đầu ra 
    vector<Neuron> neurons;
    vector<Link> links;

};



//tinh trong so cua neuron o layer sau  
class FeedForwardNetwork{
        
// layer_nodes = Các neuron ẩn cần tính thêm
// required = Toàn bộ neuron (bao gồm input + hidden + output) cần thiết để ra kết quả

// Kiểu dữ liệu: một kết nối là (input, output)
typedef pair<int, int> Connection;

// Hàm kiểm tra có chu trình hay ko?
bool cycle_check(const vector<Connection>& connections, Connection test) {
    int in = test.first;
    int out = test.second;
    
    if( in == out ) return true;
    set<int> visited;
    visited.insert(out);

    while(true) {
        int num_added = 0;
        for( const auto& conn : connections) {
            int a = conn.first;
            int b = conn.second;
            if( visited.count(a) && !visited.count(b) ) {
                if(b == in) {
                    return true;
                }

                visited.insert(b);
                num_added++;
            }
        }

        if(num_added == 0) {
            return false;
        }
    }
}


// Hàm required_for_output
public: 
static  set<int> required_for_output(const vector<int> inputs, const vector<int> outputs, const vector<Connection> connections) {
    set<int> inputs_set(inputs.begin(), inputs.end());
    set<int> outputs_set(outputs.begin(), outputs.end());

    // Kiểm tra inputs và outputs không trùng nhau
    for (int x : inputs) {
        if (outputs_set.count(x)) {
            exit(1);
        }
    }
    
    
    set<int> required(outputs.begin(), outputs.end());      // bắt đầu từ outputs, do output luon luon can thiet
    set<int> s(outputs.begin(), outputs.end());             // tập đang mở rộng, để truy ngược lại ai cấp dữ liệu cho s, kiểu trace
        
    while(true) {
        set<int> temp;    // Các node tạm thời tìm được trong một lần dò ngược, để biết bước này tìm được những gì mới, nếu ko có gì mới thì break

        for(const auto& conn: connections) {
            int a = conn.first;
            int b = conn.second;

            if(s.count(b) && !s.count(a)) {
                temp.insert(a);
            }
        }
        if(temp.empty()) {
            break;
        }

        set<int> layer_node;        // Các node tạm tìm được nhưng không phải input, chỉ những node cần tính toán mới đưa vào required
        for(const int x : temp) {
            if(!inputs_set.count(x)) {
                layer_node.insert(x);
            }
        }

        required.insert(layer_node.begin(), layer_node.end());
        s.insert(temp.begin(), temp.end());
    }
    return required;
}


static vector< set<int> > feed_forward_layer( const vector<int>& inputs, const vector<int>& outputs, const vector<pair<int, int>>& connections) {
        set<int> required = required_for_output(inputs, outputs, connections);
        set<int> s(inputs.begin(),inputs.end());        // Nhung node da co gia tri roi (input + đã tính xong)

        vector< set<int> >layers;

        while(true) {
            set<int> c;           // Các node có thể nhận input từ s
            for(const auto& conn : connections) {
                int a = conn.first; 
                int b = conn.second;

                if(s.count(a) && !s.count(b)) {
                    c.insert(b);
                }
            }
     
            set<int> t;            // Cac node co the tinh luon duoc (input da du)
            for( int n : c ) {
                if(required.count(n)) {
                    bool ready = true;
                    for(const auto& conn : connections) {
                        if(conn.second == n && !s.count(conn.first)) {
                            ready = false;
                            break;
                        }
                    }
                    if(ready) {
                        t.insert(n);
                    }
                }
            }

            if(t.empty()) {
                break;
            }

            layers.push_back(t);
            s.insert(t.begin(), t.end());
        }
        return layers;
    }


};

// TODO Sap xep cac neuron thanh cac layers
vector<vector<int>> sort_neuron_by_layers(vector<int> inputs, vector<int> outputs, vector<Link_Gene> links)
{

}


static FeedForwardNetwork create_from_geoneme(Genome &genome) {
    vector<int> inputs = genome.make_input_ids();
    vector<int> outputs = genome.make_output_ids();
    vector<std::vector<int>> layers = feed_forward_layer(inputs, outputs, genome.links);

    vector<Neuron> neurons;
    for (auto &layer : layers) {
        for (int neuron_id : layer) {
            vector<NeuronInput> neuron_inputs;
            for (auto link : genome.links) {
                if (neuron_id == link.linkid.output_id) {
                    // NOTE khoi tao neuron o day
                    neuron_inputs.emplace_back(NeuronInput{link.linkid.input_id, link.weight});
                }
            }
            auto neuron_gene = std::find(genome.neurons.begin(), genome.neurons.end(),
                     NeuronGene{neuron_id});
            assert(neuron_gene != genome.neurons.end());
            neurons.emplace_back(
                Neuron{neuron_gene->neuron_id, neuron_gene->bias, neuron_gene->activation, move(neuron_inputs)});
        }
    }

    return FeedForwardNetwork(move(inputs), move(outputs), move(neurons));
}

