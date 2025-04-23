#pragma once

#include <random>
#include <iterator>

// REVIEW tam thoi de vay
namespace RNG {
    inline std::random_device rd;
    inline std::mt19937 gen(rd());
    inline std::uniform_real_distribution<double> real_dist(0.0f, 1.0f);

    inline int get_int() {
        return gen();
    }

    inline int get_int_in_range(int min, int max) {
        std::uniform_int_distribution<> int_dist(min, max);
        return int_dist(gen);
    }

    inline double get_double() {
        return real_dist(gen);
    }

    inline float get_double_in_range(double min, double max) {
        return min + real_dist(gen)*(max-min);
    }

    template <typename T>
    T choose(double prob, const T &val1, const T &val2) {
        return (get_double() < prob) ? val1 : val2;
    }

    // Ham chon mot phan tu ngau nhien trong tap hop tra ve con tro toi phan tu do
    // https://stackoverflow.com/questions/6942273/how-to-get-a-random-element-from-a-c-container
    template<typename Iter>
    Iter select_randomly(Iter start, Iter end) {
        std::advance(start, get_int_in_range(0, std::distance(start, end) - 1));
        return start;
    }

}
