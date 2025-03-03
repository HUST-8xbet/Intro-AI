#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<chrono>
#include<random>
#include<mutex>
#include<thread>
#include<vector>
#include<list>
#include<unordered_map>
#include<queue>
#include<future>
#include <cassert>

//Lai ghep GA
struct NeuronGene{
    int neuron_id;
    double bias;
    Activation activation;
};

struct Link_ID{
    int input_id;
    int output_id;
};

struct Link_Gene{
    Link_ID linkid;
    double weight;
    bool is_enable;
};