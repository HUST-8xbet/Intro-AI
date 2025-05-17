#pragma once
#include <future>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Population.hpp"

#define MAX_TEXT_SIZE 64

class GUI {

public:
    GUI() : window(sf::VideoMode({1440, 810}),
                            "Training window",
                            sf::Style::Titlebar | sf::Style::Close),
    snake_game(6, 6),
    state(GuiState::Idle),
    current_individual(0),
    nn(create_from_geoneme(population.individuals[current_individual].genome)),
    font("ARIAL.TTF"),
    control_window_data(ControlWindowData {
        100,                           // num_generation
        6,                             // train_broad_cols
        6,                             // train_broad_rows
        "data.json",                   // training_filename
        "data.json",                   // save_filename
        6,                             // board_cols
        6,                             // board_rows
        0,                             // ai_play
        "data.json",                   // playing_filename
        "None",                        // current_loaded_population
        "New population will be used"  // loading_status
    }),
    grd(GameRenderData{
        600.0f,                        // board_width
        600.0f,                        // board_height
        {0.0f, 0.0f, 0.7f},            // snake_body_color
        {0.0f, 0.0f, 1.0f},            // snake_head_color
        {0.0f, 1.0f, 0.0f},            // food_color
        {1.0f, 0.0f, 0.0f},            // grid_color
        {0.0f, 0.0f, 0.0f}             // background_color
    }) {};

    void init();
    void run();
private:
    struct ControlWindowData {
        // Training Tab
        int num_generation;
        int train_broad_cols;
        int train_broad_rows;
        char training_filename[MAX_TEXT_SIZE];
        char save_filename[MAX_TEXT_SIZE];

        // Playing Tab
        int board_cols;
        int board_rows;
        int ai_play;
        char playing_filename[MAX_TEXT_SIZE];
        char current_loaded_population[MAX_TEXT_SIZE];
        char loading_status[MAX_TEXT_SIZE];

    } control_window_data;

    struct GameRenderData {
        float board_width;
        float board_height;
        float snake_body_color[3];
        float snake_head_color[3];
        float food_color[3];
        float grid_color[3];
        float background_color[3];
    } grd;

    enum class GuiState {
        Idle, Training, Playing
    } state;

    sf::RenderWindow window;
    sf::Font font;
    SnakeEngine snake_game;
    Population population;
    int current_individual;
    FeedForwardNetwork nn;
    std::future<void> train_thread_result;
    
    void training_thread(int num_gen, std::string file, std::string save_as, int broad_cols, int broad_rows);
    void start_ai_play();
    void start_manual_play();
    void stop_play();

    void show_control_window();
    void show_board_control();

    void draw_grid();
    void draw_food();
    void draw_snake();
    void draw_snake_game();
};