#include <vector>
#include <unordered_map>
#include <cassert>
#include <queue>
#include <iostream>

#include "Activation.hpp"
#include "Neural.hpp"

// Ham de debug
void print_genome(const Genome &genome) {
    std::cout << "GenomeID: " << genome.genome_id << "\n";
    std::cout << "NeuronIDs: ";
    for (NeuronGene neuron : genome.neurons) {
        std::cout << neuron.neuron_id << " ";
    }
    std::cout << "\n";
    std::cout << "LinkIDs: ";
    for (Link_Gene link : genome.links) {
        std::cout << "{" << link.linkid.input_id << " " << link.linkid.output_id << "}" << " ";
    }
    std::cout << "\n";
}

void print_layers(const std::vector<std::vector<int>> &layers) {
    std::cout << "Layers:\n";
    for (std::vector<int> layer: layers) {
        for (int id : layer) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }
}

// C++17 khong co ham contain
bool mapContainsKey(std::unordered_map<int, double>& map, int key)
{
    if (map.find(key) == map.end()) return false;
    return true;
}

// REVIEW cần kiểm tra lại
//tinh trong so cua neuron o layer sau
//Luu cac gia tri cua cac neuron o layer truoc vao map
//input_ids: id cua cac neuron o layer truoc
//output_ids: id cua cac neuron o layer sau
//Khoi tao output_ids = 0
//Tinh toan gia tri cho cac neuron o layer sau
//Duyet qua cac neuron o layer sau va tinh toan gia tri cho tung neuron
std::vector<double> FeedForwardNetwork:: activate(const std::vector<double> &inputs){
    assert(inputs.size() == input_ids.size());
    std::unordered_map<int, double>values;
    for (int i = 0; i < inputs.size(); i++) {
        int input_id = input_ids[i];
        values[input_id] = inputs[i];
    }
    for (int output_id : output_ids) {
        values[output_id] = 0.0;           //Neuron output dc gan = 0 
    }
    for (const auto &neuron : neurons) {
        double value = 0.0;
        for(NeuronInput input : neuron.inputs){
            assert(mapContainsKey(values, input.input_id));
            value += values[input.input_id] * input.weight;
        }
        value += neuron.bias;
        value = ActivationFn{}(neuron.activation, value);
        values[neuron.neuron_id] = value;
    }
    
    std::vector<double> outputs;
    for(int output_id : output_ids){
        assert(mapContainsKey(values, output_id));
        outputs.push_back(values[output_id]);
    }
    return outputs;
}

// FIXME đang có lỗi gom nhầm cả neuron hidden mà không có input nào vào lớp đầu tiên //Da chinh sua
// hình như chưa kiểm tra xem link có được bật hay không
std::vector<std::vector<int>> feed_forward_layer(std::vector<int> inputs, 
    std::vector<int> outputs, std:: vector<Link_Gene> links)
{
    // Xay dung do thi vao ra cho cac node
    std::unordered_map<int, std::vector<int>> forward_graph;
    std::unordered_map<int, int> indegree; // số lượng đầu vào mỗi node

    //Khoi tao cac node co indegree = 0
    for (const auto &link : links) {
        if(!link.is_enable) continue; // Chi xet cac link duoc enable, tranh loi de tao ra graph khong hop le
        
        int u = link.linkid.input_id;
        int v = link.linkid.output_id;
        forward_graph[u].push_back(v); // u -> v
        indegree[v]++; // Tang indegree cho v
        if (indegree.find(u) == indegree.end()) {
            indegree[u] = 0; // Khoi tao indegree cho u
        }
    }

    //Queue cac nut co indegree = 0
    std::queue<int> q;
    std::unordered_map<int, int> node_layer;    // luu layer cho cac node
    for (int input_id : inputs) {
        q.push(input_id); // Cho cac node input vao queue
        node_layer[input_id] = 0; // Gan layer cho cac node input
    }

    // Topological Sort để gán layer
    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int neighbor : forward_graph[node]) {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0) {
                q.push(neighbor);
                node_layer[neighbor] = node_layer[node] + 1; // Gan layer cho cac node lien ket
            }
        }
    }

    // Gom cac node theo layer
    int max_layer = 0;
    for (const auto& [node_id, layer] : node_layer) {
        if (layer > max_layer) {
            max_layer = layer;
        }
    }
    std::vector<std::vector<int>> layers(max_layer + 1);
    for (const auto& [node_id, layer] : node_layer) {
        layers[layer].push_back(node_id);
    }    
    return layers;

}

// REVIEW cần kiểm tra lại
// FIXME hình như chưa kiểm tra xem link có được bật hay không //Da chinh sua
FeedForwardNetwork create_from_geoneme(Genome &genome) {
//    print_genome(genome);

    std::vector<int> inputs = genome.make_input_ids();
    std::vector<int> outputs = genome.make_output_ids();
    std::vector<std::vector<int>> layers = feed_forward_layer(inputs, outputs, genome.links);

//    print_layers(layers);

    std::vector<Neuron> neurons;
    // Bo qua input layers
    for (int i = 1; i < layers.size(); i++) {
        std::vector<int> layer = layers[i];
        for (int neuron_id : layer) {
            std::vector<NeuronInput> neuron_inputs;
            for (auto link : genome.links) { //Da them kiem tra link duoc enable hay khong
                if (link.is_enable && neuron_id == link.linkid.output_id) { //Neu link duoc enable va neuron_id = link.linkid.output_id thi thuc hien
                    
                    neuron_inputs.emplace_back(NeuronInput{link.linkid.input_id, link.weight});
                }
            }

            auto neuron_gene = std::find(genome.neurons.begin(), genome.neurons.end(),
             // NOTE khoi tao neuron o day
                     NeuronGene{neuron_id, 0.0, relu{}});
            assert(neuron_gene != genome.neurons.end());
            neurons.emplace_back(
                Neuron{neuron_gene->neuron_id, neuron_gene->bias, neuron_gene->activation, std::move(neuron_inputs)});
        }
    }

    return FeedForwardNetwork(std::move(inputs), std::move(outputs), std::move(neurons));
}



