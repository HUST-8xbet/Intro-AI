#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GameRenderer.hpp"
#include "Serializer.hpp"

void start_training() {
    sf::RenderWindow window(sf::VideoMode({cf::WindowWidth, cf::WindowHeight}),
                            "Training window",
                            sf::Style::Titlebar | sf::Style::Close);
    // Gioi han FPS la 10, train rat cham nhung ma qua cao thi kho nhin
    window.setFramerateLimit(10);

    SnakeEngine snakeEngine(cf::NumRows, cf::NumCols);
    snakeEngine.newGame();
    GameRenderer renderer(window, snakeEngine);

    // Khoi tao quan the va dung mang neuron tu ca the dau tien
    Population population;
    // NOTE train có đồ họa bị giới hạn bởi FPS nên rất chậm
    // Do đó train sẵn 50 thế hệ rồi dùng giao diện để xem tiến độ
    population.run(50);
    int current_generation = 50;
    int current_individual = 0;
    FeedForwardNetwork nn = create_from_geoneme(population.individuals[current_individual].genome);

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
        }

        // NOTE chua xong
        // Ca the con song
        if (snakeEngine.state == GameState::Running && snakeEngine.step < cf::max_steps) {
            Direction action = get_action(nn, snakeEngine);
            snakeEngine.update(action);
        }
        // Ca the chet
        else {
            population.individuals[current_individual].fitness = calculate_fitness(snakeEngine);
            current_individual++;
            // The he tiep theo
            if (current_individual == cf::population_size) {
                std::cout << "Generation " << current_generation << ":\n";
                population.update_best();
                population.individuals = population.reproduce();
                current_generation++;
                if (current_generation == 10) {
                    window.close();
                }
                current_individual = 0;
            }
            // Xay dung mang neuron tu ca the tiep theo
            nn = create_from_geoneme(population.individuals[current_individual].genome);
            snakeEngine.newGame();
        }

        // ve ra man hinh
        window.clear(sf::Color::Black);
        renderer.draw();
        window.display();
    }
}

void manual_play(SnakeEngine &snakeEngine, std::optional<sf::Event> &event) {
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

int main()
{
    start_training();
}


