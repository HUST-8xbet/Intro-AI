#include <imgui.h>
#include <imgui-SFML.h>

#include "gui.hpp"
#include "Serializer.hpp"

sf::Color vec3f_to_color(float rgb[3]) {
    return sf::Color{
        (uint8_t) (rgb[0]*255),
        (uint8_t) (rgb[1]*255),
        (uint8_t) (rgb[2]*255)};
}

void GUI::training_thread(int num_gen, std::string load_from, std::string save_as,
        int broad_cols, int broad_rows, int max_step_since_last_food) {
    Population population;
    if (!load_population_json(population, load_from)) {
        std::cout << "Cannot open file, start training new population" << std::endl;
    } else std::cout << "Loaded population from: " << load_from << std::endl;

    std::cout << num_gen << "\n";
    std::cout << broad_cols << "\n";
    std::cout << broad_rows << "\n";
    std::cout << max_step_since_last_food << "\n";
    population.run(num_gen, broad_cols, broad_rows, max_step_since_last_food);
    save_population_json(population, save_as);

    std::cout << "Training completed" << std::endl;
    state = GuiState::Idle;
}

void GUI::show_control_window() {
    ImGui::SetNextWindowSize(ImVec2{800, 400}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2{200, 200}, ImVec2{FLT_MAX, FLT_MAX});

    if (!ImGui::Begin("Control Window")) {
        ImGui::End();
        return;
    }

    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Training Tab"))
        {
            ImGui::InputText("Load population from", control_window_data.training_filename, MAX_TEXT_SIZE);
            ImGui::InputText("Save population as", control_window_data.save_filename, MAX_TEXT_SIZE);

            ImGuiSliderFlags flags = ImGuiSliderFlags_ClampOnInput;
            ImGui::SliderInt("Number generation", &control_window_data.num_generation, 0, 2000, "%d", flags);
            ImGui::SliderInt("Train Board Columns", &control_window_data.train_broad_cols, 5, 20, "%d", flags);
            ImGui::SliderInt("Train Board Rows", &control_window_data.train_broad_rows, 5, 20, "%d", flags);
            ImGui::SliderInt("Max Steps Since Last Food", &control_window_data.max_step_since_last_food, 0, 500, "%d", flags);
            if (ImGui::Button("Start training")) {
                if (strlen(control_window_data.save_filename) == 0) {
                    std::cout << "Save Filename Is Null";
                } else {state = GuiState::Training;
                train_thread_result = std::async(std::launch::async, training_thread, this,
                                            control_window_data.num_generation,
                                            control_window_data.training_filename,
                                            control_window_data.save_filename,
                                            control_window_data.train_broad_cols,
                                            control_window_data.train_broad_rows,
                                            control_window_data.max_step_since_last_food);}
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Playing Tab"))
        {
            ImGuiSliderFlags flags = ImGuiSliderFlags_ClampOnInput;
            ImGui::SliderInt("Board Columns", &control_window_data.board_cols, 5, 20, "%d", flags);
            ImGui::SliderInt("Board Rows", &control_window_data.board_rows, 5, 20, "%d", flags);
            ImGui::RadioButton("Manual Play", &control_window_data.ai_play, 0); ImGui::SameLine();
            ImGui::RadioButton("AI Play", &control_window_data.ai_play, 1);

            if (control_window_data.ai_play) {
                ImGui::Text("Current Loaded Population: ");
                ImGui::Text(control_window_data.current_loaded_population);
                ImGui::Text(control_window_data.loading_status);
                ImGui::InputText("Load population from", control_window_data.playing_filename, MAX_TEXT_SIZE);
                if (ImGui::Button("Load Population")) {
                    if (!load_population_json(population, control_window_data.playing_filename)) {
                        strcpy(control_window_data.loading_status, "Cannot load population!");
                    } else {
                        strcpy(control_window_data.current_loaded_population, control_window_data.playing_filename);
                        strcpy(control_window_data.loading_status, "Population loaded!");
                    }
                }
            }
            if (ImGui::Button("Start Playing")) {
                if (control_window_data.ai_play) start_ai_play();
                else start_manual_play();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}

void GUI::show_board_control() {
    ImGui::SetNextWindowSize(ImVec2{400, 400}, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2{200, 200}, ImVec2{FLT_MAX, FLT_MAX});

    if (!ImGui::Begin("Board Control Window")) {
        ImGui::End();
        return;
    }

    //ImGui::SeparatorText("Render Control");
    //ImGuiSliderFlags flags = ImGuiSliderFlags_ClampOnInput;
    //ImGui::SliderFloat("Board Width", &grd.board_width, 0.0f, 800.0f, "%.3f", flags);
    //ImGui::SliderFloat("Board Height", &grd.board_height, 0.0f, 800.0f, "%.3f", flags);
//
    //ImGui::ColorEdit3("Snake Body Color", grd.snake_body_color);
    //ImGui::ColorEdit3("Snake Head Color", grd.snake_head_color);
    //ImGui::ColorEdit3("Food Color", grd.food_color);
    //ImGui::ColorEdit3("Grid Color", grd.grid_color);
    //ImGui::ColorEdit3("Background Color", grd.background_color);

    if (control_window_data.ai_play) {
        ImGui::SeparatorText("AI Control");

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Current Individual:");
        ImGui::SameLine();
    
        float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
        ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, false);
        if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
            if (current_individual >= 0) {
                snake_game.newGame();
                current_individual--;
                nn = create_from_geoneme(population.individuals[current_individual].genome);
                nr.init(population.individuals[current_individual].genome);
            }
        }
        ImGui::SameLine(0.0f, spacing);
        if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
            if (current_individual < cf::population_size) {
                snake_game.newGame();
                current_individual++;
                nn = create_from_geoneme(population.individuals[current_individual].genome);
                nr.init(population.individuals[current_individual].genome);
            }
        }
        ImGui::PopItemFlag();
        ImGui::SameLine();
        ImGui::Text("%d", current_individual);
    }

    ImGui::SeparatorText("Game Control");
    if (ImGui::Button("New Game")) {
        snake_game.newGame();
    }
    if (ImGui::Button("Stop Playing")) {
        stop_play();
    }
    ImGui::End();
}

void GUI::init() {
    window.setFramerateLimit(20);
    bool imgui_started = ImGui::SFML::Init(window, false);
    ImGuiIO& IO = ImGui::GetIO();

    IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    IO.Fonts->AddFontFromFileTTF("ARIAL.TTF", 20.f);

    bool imgui_font_loaded = ImGui::SFML::UpdateFontTexture(); // important call: updates font texture
}

void GUI::run() {
    sf::Clock deltaClock;
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (state == GuiState::Playing && !control_window_data.ai_play)
            {
                if (snake_game.state == GameState::Running) 
                {
                    if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
                    {
                        if (keyReleased->scancode == sf::Keyboard::Scancode::Up) 
                            snake_game.update(Direction::Up);
                        else if (keyReleased->scancode == sf::Keyboard::Scancode::Down) 
                            snake_game.update(Direction::Down);
                        else if (keyReleased->scancode == sf::Keyboard::Scancode::Left) 
                            snake_game.update(Direction::Left);
                        else if (keyReleased->scancode == sf::Keyboard::Scancode::Right)
                            snake_game.update(Direction::Right);
                    }
                }
                else if (snake_game.state == GameState::GameOver || snake_game.state == GameState::Win)
                {
                    if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
                        if (keyReleased->scancode == sf::Keyboard::Scancode::R)
                            snake_game.newGame();
                }
            }
        }

        // ImGui Stuff
        ImGui::SFML::Update(window, deltaClock.restart());

        if (state == GuiState::Idle) {
            show_control_window();
        }

        if (state == GuiState::Training) {
            ImGui::Begin("Notify Window");
            ImGui::Text("Check Terminal For Training Progress");
            ImGui::End();
        }

        if (state == GuiState::Playing) {
            show_board_control();
        }

        // Update
        if (state == GuiState::Playing && control_window_data.ai_play && snake_game.state == GameState::Running) {
            Direction action = get_action(nn, snake_game);
            snake_game.update(action);
        }

        // ve ra man hinh
        window.clear(background_color);
        if (state == GuiState::Playing) {
            br.draw(window, snake_game);
            if (control_window_data.ai_play) {
                nr.draw(window);
            }
        }
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}

void GUI::start_ai_play() {
    SnakeEngine new_snake_game(control_window_data.board_cols, control_window_data.board_rows);
    snake_game = new_snake_game;
    snake_game.newGame();

    current_individual = 0;
    nn = create_from_geoneme(population.individuals[current_individual].genome);
    nr.init(population.individuals[current_individual].genome);

    state = GuiState::Playing;
}

void GUI::start_manual_play() {
    SnakeEngine new_snake_game(control_window_data.board_cols, control_window_data.board_rows);
    snake_game = new_snake_game;
    snake_game.newGame();
    state = GuiState::Playing;
}

void GUI::stop_play() {
    state = GuiState::Idle;
}

