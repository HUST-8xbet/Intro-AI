#include <SFML/Graphics.hpp>

#include "SnakeEngine.hpp"

class BoardRenderer {
public:
    BoardRenderer() : font("ARIAL.TTF") {}
    void draw(sf::RenderWindow &window, const SnakeEngine &snake_game) {
        float row_height = board_height / snake_game.rows;
        float col_width  = board_width  / snake_game.cols;

        // Ve than ran
        sf::RectangleShape bodyRec({col_width, row_height});
        bodyRec.setFillColor(snake_body_color);
        for (Coordinates bodyPart : snake_game.snakeBody) {
            bodyRec.setPosition({board_x + bodyPart.col * col_width, board_y + bodyPart.row * row_height});
            window.draw(bodyRec);
        }

        // Ve lai dau ran mau khac
        sf::RectangleShape headRec({col_width, row_height});
        headRec.setPosition({board_x + snake_game.snakeBody.front().col * col_width,
                             board_y + snake_game.snakeBody.front().row * row_height});
        headRec.setFillColor(snake_head_color);
        window.draw(headRec);

        sf::RectangleShape foodRec({col_width, row_height});
        foodRec.setPosition({board_x + snake_game.food.col * col_width, board_y + snake_game.food.row * row_height});
        foodRec.setFillColor(food_color);
        window.draw(foodRec);

                sf::VertexArray grid(sf::PrimitiveType::Lines, (snake_game.rows + 1 + snake_game.cols + 1) * 2);

        // Them cac duong ke ngang
        for (int i = 0; i <= snake_game.rows * 2; i += 2) {
            grid[i].position = sf::Vector2f(board_x, board_y + row_height*(i/2));
            grid[i + 1].position = sf::Vector2f(board_x + board_width, board_y + row_height*(i/2));

            grid[i].color = grid_color;
            grid[i + 1].color = grid_color;
        }

        // Them cac duong ke doc
        // (rows+1)*2 la so dinh cua cac duong ke ngang
        for (int i = 0; i <= snake_game.cols * 2; i += 2) {
            grid[i + (snake_game.rows + 1)*2].position = sf::Vector2f(board_x + col_width*i/2, board_y);
            grid[i + (snake_game.rows + 1)*2 + 1].position = sf::Vector2f(board_x + col_width*i/2, board_y + board_height);

            grid[i + (snake_game.rows + 1)*2].color = grid_color;
            grid[i + (snake_game.rows + 1)*2 + 1].color = grid_color;
        }

        window.draw(grid);

        if (snake_game.state == GameState::GameOver) {
            sf::Text text(font);
            text.setPosition({board_x, board_y + 10.0f + board_height});
            text.setString("GameOver");
            window.draw(text);
        } else if (snake_game.state == GameState::Win) {
            sf::Text text(font);
            text.setPosition({board_x, board_y + 10.0f + board_height});
            text.setString("You win");
            window.draw(text);
        }
    }
private:
    const float board_width  = 600.0f;
    const float board_height = 600.0f;
    const float board_x      = 800.0f;
    const float board_y      =  10.0f;

    const sf::Color snake_body_color = sf::Color{168, 218, 220};
    const sf::Color snake_head_color = sf::Color{69 , 123, 157};
    const sf::Color food_color       = sf::Color{230, 57 , 70 };
    const sf::Color grid_color       = sf::Color{241, 250, 238};

    const sf::Font  font;
};