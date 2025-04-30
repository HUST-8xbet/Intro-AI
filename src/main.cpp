#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GameRenderer.hpp"
#include "Serializer.hpp"

void start_game() {
    sf::RenderWindow window(sf::VideoMode({cf::WindowWidth, cf::WindowHeight}),
                            "My window",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    SnakeEngine snakeEngine(cf::NumRows, cf::NumCols);
    snakeEngine.newGame();
    GameRenderer renderer(window, snakeEngine);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (snakeEngine.state == GameState::Running) 
            {
                if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
                {
                    if (keyReleased->scancode == sf::Keyboard::Scancode::Up) 
                    {
                        snakeEngine.update(Direction::Up);
                        std::vector<double> inputs = extract_inputs(snakeEngine);
                        print_input(inputs);
                    } 
                    else if (keyReleased->scancode == sf::Keyboard::Scancode::Down) 
                    {
                        snakeEngine.update(Direction::Down);
                        std::vector<double> inputs = extract_inputs(snakeEngine);
                        print_input(inputs);
                    } 
                    else if (keyReleased->scancode == sf::Keyboard::Scancode::Left) 
                    {
                        snakeEngine.update(Direction::Left);
                        std::vector<double> inputs = extract_inputs(snakeEngine);
                        print_input(inputs);
                    } 
                    else if (keyReleased->scancode == sf::Keyboard::Scancode::Right) 
                    {
                        snakeEngine.update(Direction::Right);
                        std::vector<double> inputs = extract_inputs(snakeEngine);
                        print_input(inputs);
                    }
                }
            }
            else if (snakeEngine.state == GameState::GameOver || snakeEngine.state == GameState::Win)
            {
                if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) 
                {
                    if (keyReleased->scancode == sf::Keyboard::Scancode::R) 
                    {
                        snakeEngine.newGame();
                    }
                }
            }
        }

    
        // ve ra man hinh
        window.clear(sf::Color::Black);
        renderer.draw();
        window.display();
    }
}

void train() {
    Population new_population;
    new_population.run(100);
}

static Genome new_genome() {
    Genome genome{0, cf::num_input, cf::num_output};
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

int main()
{
    // Test thử tuần tự hóa
    Population population;
    save_population_json(population, "data.json");
}


