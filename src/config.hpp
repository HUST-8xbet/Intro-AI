#pragma once
#include <SFML/Graphics.hpp>

// NOTE file này bao gồm các hằng số sử dụng trong NEAT để tiện thay đổi
// Mẹo: giữ ctrl + click chuột trái vào tên hằng số để xem nó được sử dụng ở đâu

namespace cf {

//----------------------------------------------------------------------------------
// Config cho NEAT
//----------------------------------------------------------------------------------

    // Population
    inline unsigned int const population_size = 500;
    inline unsigned int const num_input       =  16;
    inline unsigned int const num_output      =   4;
    inline double const survival_threshold    = 0.2;

    // Non structral mutation
    inline double const normal_dist_mean      =   0.0; // ki vong cua phan phoi chuan
    inline double const normal_dist_stddev    =   1.0; // do lech chuan cua phan phoi chuan
    inline double const neuron_mutation_rate  =   0.7;
    inline double const link_mutation_rate    =   0.7;
    inline double const delta_max_value       =  30.0;
    inline double const delta_min_value       = -30.0;

    // Structral mutation
    inline double const prob_add_link         = 0.25;
    inline double const prob_remove_link      = 0.25;
    inline double const prob_add_neuron       = 0.10;
    inline double const prob_remove_neuron    = 0.10;
}

