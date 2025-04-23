#include <SFML/Graphics.hpp>
#include "GameRenderer.hpp"
#include "config.hpp"
#include <iostream>

GameRenderer::GameRenderer(sf::RenderWindow &win, SnakeEngine &gameData) : window(win), gameData(gameData)
{
    if (!font.openFromFile("ARIAL.TTF"))
    {
        // error...
    }
}

void GameRenderer::drawGrid() {

    sf::VertexArray grid(sf::PrimitiveType::Lines, (gameData.rows + 1 + gameData.cols + 1) * 2);

    // Them cac duong ke ngang
    for (int i = 0; i <= gameData.rows * 2; i += 2) {
        grid[i].position = sf::Vector2f(10.0f, 10.f + cf::RowHeight*(i/2));
        grid[i + 1].position = sf::Vector2f(10.0f + cf::BoardWidth, 10.f + cf::RowHeight*(i/2));

        grid[i].color = cf::gridColor;
        grid[i + 1].color = cf::gridColor;
    }

    // Them cac duong ke doc
    // (rows+1)*2 la so dinh cua cac duong ke ngang
    for (int i = 0; i <= gameData.cols * 2; i += 2) {
        grid[i + (gameData.rows + 1)*2].position = sf::Vector2f(10.0f + cf::ColWidth*i/2, 10.f);
        grid[i + (gameData.rows + 1)*2 + 1].position = sf::Vector2f(10.0f + cf::ColWidth*i/2, 10.f + cf::BoardHeight);

        grid[i + (gameData.rows + 1)*2].color = cf::gridColor;
        grid[i + (gameData.rows + 1)*2 + 1].color = cf::gridColor;
    }

    window.draw(grid);
}

void GameRenderer::drawSnake() {
    // Ve than ran
    for (Coordinates bodyPart : gameData.snakeBody) {
        sf::RectangleShape bodyRec({cf::ColWidth, cf::RowHeight});
        bodyRec.setPosition({10.f + bodyPart.col * cf::ColWidth, 10.f + bodyPart.row * cf::RowHeight});
        bodyRec.setFillColor(cf::snakeBodyColor);
        window.draw(bodyRec);
    }

    // Ve lai dau ran mau khac
    sf::RectangleShape headRec({cf::ColWidth, cf::RowHeight});
    headRec.setPosition({10.f + gameData.snakeBody.front().col * cf::ColWidth,
                         10.f + gameData.snakeBody.front().row * cf::RowHeight});
    headRec.setFillColor(cf::snakeHeadColor);
    window.draw(headRec);
}

void GameRenderer::drawFood() {
    sf::RectangleShape foodRec({cf::ColWidth, cf::RowHeight});
    foodRec.setPosition({10.f + gameData.food.col * cf::ColWidth, 10.f + gameData.food.row * cf::RowHeight});
    foodRec.setFillColor(cf::foodColor);
    window.draw(foodRec);
}

void GameRenderer::draw() {
    if (gameData.state == GameState::Running) {
        drawSnake();
        drawFood();
        drawGrid();
    }
    else if (gameData.state == GameState::GameOver) {
        sf::Text text(font);
        text.setString("GameOver Press R to restart");
        window.draw(text);
    }
    else if (gameData.state == GameState::Win) {
        sf::Text text(font);
        text.setString("You win Press R to restart");
        window.draw(text);
    }
}