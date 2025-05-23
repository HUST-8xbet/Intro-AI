#pragma once
#include <future>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Population.hpp"
#include "NetworkRenderer.hpp"
#include "BoardRenderer.hpp"

#define MAX_TEXT_SIZE 64

class GUI {

public:
    GUI() : 
    window(sf::VideoMode({1440, 810}), "Training window", sf::Style::Titlebar | sf::Style::Close),
    snake_game(6, 6),
    state(GuiState::Idle),
    current_individual(0),
    nn(create_from_geoneme(population.individuals[current_individual].genome)),
    control_window_data(ControlWindowData {
        100,                           // num_generation
        6,                             // train_broad_cols
        6,                             // train_broad_rows
        50,                            // max_step_since_last_food
        "data.json",                   // training_filename
        "data.json",                   // save_filename
        6,                             // board_cols
        6,                             // board_rows
        0,                             // ai_play
        "data.json",                   // playing_filename
        "None",                        // current_loaded_population
        "New population will be used"  // loading_status
    }) {};

    void init();
    void run();
private:
    struct ControlWindowData {
        // Training Tab
        int  num_generation;
        int  train_broad_cols;
        int  train_broad_rows;
        int  max_step_since_last_food;
        char training_filename[MAX_TEXT_SIZE];
        char save_filename[MAX_TEXT_SIZE];

        // Playing Tab
        int  board_cols;
        int  board_rows;
        int  ai_play;
        char playing_filename[MAX_TEXT_SIZE];
        char current_loaded_population[MAX_TEXT_SIZE];
        char loading_status[MAX_TEXT_SIZE];

    } control_window_data;

    enum class GuiState {
        Idle, Training, Playing
    } state;

    sf::RenderWindow window;
    sf::Color background_color = sf::Color{29, 53, 87};
    SnakeEngine snake_game;
    Population population;
    int current_individual;
    FeedForwardNetwork nn;
    NetworkRenderer nr;
    BoardRenderer br;
    std::future<void> train_thread_result;

    
    void training_thread(int num_gen, std::string file, std::string save_as,
        int broad_cols, int broad_rows, int max_step_since_last_food);
    void start_ai_play();
    void start_manual_play();
    void stop_play();

    void show_control_window();
    void show_board_control();

};