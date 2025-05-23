#pragma once
#include <fstream>
#include <string>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include "Population.hpp"

// NOTE file dữ liệu sẽ được lưu cùng vị trí với file thực thi main.exe
// Tức là ở Intro-AI/build/bin

void save_population_json(const Population &population, const std::string& filename) {
    std::ofstream os(filename);
    cereal::JSONOutputArchive archive(os);
    archive(CEREAL_NVP(population));
}

bool load_population_json(Population &population, const std::string& filename) {
    std::ifstream is(filename);
    if (!is) return false;
    cereal::JSONInputArchive archive(is);
    archive(population);
    return true;
}

void save_population_binary(const Population &population, const std::string& filename) {
    std::ofstream os(filename, std::ios::binary);
    cereal::BinaryOutputArchive archive(os);
    archive(population);
}

bool load_population_binary(Population &population, const std::string& filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is) return false;
    cereal::BinaryInputArchive archive(is);
    archive(population);
    return true;
}

void saveIndividualJson(const Individual& individual, const std::string& filename) {
    std::ofstream os(filename);
    cereal::JSONOutputArchive archive(os);
    archive(CEREAL_NVP(individual));
}

bool loadIndividualJson(Individual& ind, const std::string& filename) {
    std::ifstream is(filename);
    if (!is) return false;
    cereal::JSONInputArchive archive(is);
    archive(ind);
    return true;
}

void saveIndividualBinary(const Individual& ind, const std::string& filename) {
    std::ofstream os(filename, std::ios::binary);
    cereal::BinaryOutputArchive archive(os);
    archive(ind);
}

bool loadIndividualBinary(Individual& ind, const std::string& filename) {
    std::ifstream is(filename, std::ios::binary);
    if (!is) return false;
    cereal::BinaryInputArchive archive(is);
    archive(ind);
    return true;
}