#include <SFML/Graphics.hpp>
#include "Population.hpp"

class NetworkRenderer {
public:
    void init(Genome &genome) {
        nodes.clear();
        links.clear();

        std::vector<int> inputs  = genome.make_input_ids();
        std::vector<int> outputs = genome.make_output_ids();

        std::vector<std::vector<int>> layers = feed_forward_layer(inputs, outputs, genome.links);
        // Can sap xep lai output ra lop cuoi cung
        int num_output = outputs.size();
        int num_layer  = layers.size();
        // Skip hang dau va hang cuoi
        for (int i = 1; i < num_layer - 1; i++) {
            int num_neuron = layers[i].size();
            for (int j = 0; j < num_neuron; j++) {
                // Tim xoa id output o cac hang truoc
                if (*(layers[i].begin() + j) >= 0 && *(layers[i].begin() + j) <= num_output) {
                    layers[i].erase(layers[i].begin() + j);
                }
            }
        }
        layers[num_layer-1] = outputs;

        for (int i = 0; i<layers.size(); i++) {
            std::vector<int> &layer = layers[i];
            float layer_height = node_distance * (layer.size()-1);
            for (int j = 0; j < layer.size(); j++) {
                nodes.emplace_back(central_point_x + i*layer_distance,
                                   central_point_y - layer_height/2 + j*node_distance,
                                   layer[j]);
                }
        }

        //for (const Node &node : nodes) {
        //    std::cout << node.id << " ";
        //}
        //std::cout << "\n";

        for (const Link_Gene &link : genome.links) {
            if (!link.is_enable) continue;
            auto start_node = std::find_if(nodes.begin(), nodes.end(),
                [link](Node &node) {return link.linkid.input_id  == node.id;});
            auto end_node   = std::find_if(nodes.begin(), nodes.end(),
                [link](Node &node) {return link.linkid.output_id == node.id;});
            //std::cout << "{" << start_node->id << " " << end_node->id << "}" << " ";
            links.emplace_back(start_node->position, end_node->position, 
                (link.weight >= 0) ? sf::Color{230, 57, 70} : sf::Color{168, 218, 220});
        }

    }
    void draw(sf::RenderWindow &window) {
        int num_link = links.size();
        sf::VertexArray connections (sf::PrimitiveType::Lines, num_link * 2);
        int i = 0;
        for (Link link : links) {
            connections[i].position   = link.start_position;
            connections[i+1].position = link.end_position;

            connections[i].color   = link.color;
            connections[i+1].color = link.color;
            i += 2;
        }
        window.draw(connections);

        sf::CircleShape circle(node_radius);
        circle.setOrigin({node_radius, node_radius});
        circle.setOutlineThickness(outline_thickness);
        circle.setFillColor(node_color);
        circle.setOutlineColor(outline_color);
        for (const Node &node : nodes) {
            circle.setPosition(node.position);
            window.draw(circle);
        }
    }
private:
    struct Node {
        sf::Vector2f position;
        int id;
        Node(float x, float y, int id) : 
            position(x,y),
            id(id) {}
    };

    struct Link {
        sf::Vector2f start_position;
        sf::Vector2f end_position;
        sf::Color color;
        Link (sf::Vector2f start, sf::Vector2f end, sf::Color color) : 
            start_position(start),
            end_position(end),
            color(color) {}
    };

    std::vector<Node> nodes;
    std::vector<Link> links;

    float central_point_x   = 100.0f;
    float central_point_y   = 400.0f;

    float node_distance     = 50.0f;
    float layer_distance    = 100.0f;

    float node_radius       = 15.0f;
    float outline_thickness = 2.0f;

    int   node_sides        = 20;

    sf::Color node_color    = {69 , 123, 157};
    sf::Color outline_color = {241, 250, 238};

    //float line_thickness    = 5.0f;
};
