#include<iostream>
#include<vector>
#include<fstream>
#include <cereal/archives/binary.hpp>
#include<cereal/archives/json.hpp>
#include<GeneticAlgorithm.hpp>

// TODO viet ham tuan tu hoa du lieu genome

void saveIndividualJson(const Individual& ind, const string& filename) {
    ofstream os(filename);
    cereal::JSONOutputArchive archive(os);
    archive(ind);
}

void loadIndividualJson(Individual& ind, const string& filename) {
    ifstream is(filename);
    cereal::JSONInputArchive archive(is);
    archive(ind);
}

void saveIndividualBinary(const Individual& ind, const string& filename) {
    ofstream os(filename, ios::binary);
    cereal::BinaryOutputArchive archive(os);
    archive(ind);
}

void loadIndividualBinary(Individual& ind, const string& filename) {
    ifstream is(filename, ios::binary);
    cereal::BinaryInputArchive archive(is);
    archive(ind);
}

