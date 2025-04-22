#pragma once
#include <SFML/Graphics.hpp>

namespace cf {

    //---------------------------------------------------------
    // Config cho giao dien
    //---------------------------------------------------------
    inline int const WindowWidth = 800;
    inline int const WindowHeight = 600;
    inline float const BoardWidth = 400.f;
    inline float const BoardHeight = 400.f;

    // Khong duoc nho hon 5
    // Neu ca hang va cot deu le se ko co chu trinh hamiltonian
    inline unsigned int const NumRows = 6;
    inline unsigned int const NumCols = 6;

    inline float const RowHeight = BoardHeight/NumRows;
    inline float const ColWidth = BoardWidth/NumCols;

    inline const sf::Color snakeBodyColor = sf::Color::Cyan;
    inline const sf::Color snakeHeadColor = sf::Color::Blue;
    inline const sf::Color foodColor = sf::Color::Green;
    inline const sf::Color gridColor = sf::Color::Red;

    //---------------------------------------------------------
    // Config cho NEAT
    //---------------------------------------------------------
    inline unsigned int const population_size = 500;
    inline unsigned int const num_input = 4;
    inline unsigned int const num_output = 4;

    inline unsigned int const max_steps = 1000;
}

