#include <vector>
#include <unordered_map>
#include <cassert>

#include "Activation.hpp"
#include "GeneticAlgorithm.hpp"

//Tao neuron va gia tri cua neuron
struct NeuronInput{
    int input_id;
    double weight;
};

// REVIEW khoi tao neuron nhu the nao?
struct Neuron{
    int neuron_id;
    double bias;
    Activation activation;
    std::vector<NeuronInput>inputs;
};

// C++17 khong co ham contain
bool mapContainsKey(std::unordered_map<int, double>& map, int key)
{
    if (map.find(key) == map.end()) return false;
    return true;
}


//tinh trong so cua neuron o layer sau
class FeedForwardNetwork{
    public:
    std::vector<double> activate(const std::vector<double> &inputs){
        assert(inputs.size() == input_ids.size());
        std::unordered_map<int, double>values;

        for(int i = 0;i < inputs.size();i++){
            int input_id = input_ids[i];
            values[input_id] = inputs[i];
        }

        for(int output_id: output_ids){
            values[output_id] = 0.0;           //Neuron output dc gan = 0 
        }

        for(const auto &neuron : neurons){
            double value =0.0;
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

    FeedForwardNetwork(std::vector<int> input_ids, std::vector<int> output_ids, std::vector<Neuron> neurons) : 
        input_ids(input_ids), output_ids(output_ids), neurons(neurons) {}

    private:
    std::vector<int> input_ids;
    std::vector<int> output_ids;
    std::vector<Neuron> neurons;

};

// TODO Sap xep cac neuron thanh cac layers
std::vector<std::vector<int>> feed_forward_layer(std::vector<int> inputs, 
    std::vector<int> outputs, std:: vector<Link_Gene> links)
{

}


static FeedForwardNetwork create_from_geoneme(Genome &genome) {
    std::vector<int> inputs = genome.make_input_ids();
    std::vector<int> outputs = genome.make_output_ids();
    std::vector<std::vector<int>> layers = feed_forward_layer(inputs, outputs, genome.links);

    std::vector<Neuron> neurons;
    for (auto &layer : layers) {
        for (int neuron_id : layer) {
            std::vector<NeuronInput> neuron_inputs;
            for (auto link : genome.links) {
                if (neuron_id == link.linkid.output_id) {
                    // NOTE khoi tao neuron o day
                    neuron_inputs.emplace_back(NeuronInput{link.linkid.input_id, link.weight, });
                }
            }
            auto neuron_gene = std::find(genome.neurons.begin(), genome.neurons.end(),
                     NeuronGene{neuron_id});
            assert(neuron_gene != genome.neurons.end());
            neurons.emplace_back(
                Neuron{neuron_gene->neuron_id, neuron_gene->bias, neuron_gene->activation, std::move(neuron_inputs)});
        }
    }

    return FeedForwardNetwork(std::move(inputs), std::move(outputs), std::move(neurons));
}

