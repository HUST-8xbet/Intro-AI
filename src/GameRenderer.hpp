#pragma once

#include <SFML/Graphics.hpp>
#include "SnakeEngine.hpp"

class GameRenderer
{
private:
    sf::RenderWindow &window;
    const SnakeEngine &gameData;
    sf::Font font;

public:
    GameRenderer(sf::RenderWindow &win, SnakeEngine &gameData);
    void draw();
    void drawGrid();
    void drawSnake();
    void drawFood();
};