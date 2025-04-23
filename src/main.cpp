#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>

#include "config.hpp"
#include "GameRenderer.hpp"
#include "GeneticAlgorithm.hpp"

int main()
{

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
                    } 
                    else if (keyReleased->scancode == sf::Keyboard::Scancode::Down) 
                    {
                        snakeEngine.update(Direction::Down);
                    } 
                    else if (keyReleased->scancode == sf::Keyboard::Scancode::Left) 
                    {
                        snakeEngine.update(Direction::Left);
                    } 
                    else if (keyReleased->scancode == sf::Keyboard::Scancode::Right) 
                    {
                        snakeEngine.update(Direction::Right);
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