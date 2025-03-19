#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include <variant>
#include <cmath>

// Dinh nghia cac ham kich hoat
struct relu{
    double operator()(double x) const {
        return x >0 ? x : 0; // Neu (x>0) tra x con ko ra 0
    } 
};

struct sigmoid {
    double operator() (double x) const{
        return 1.0 / (1.0 + std::exp(-x)); //f(x) = 1/(1+e^-x)
    }   
};

struct Tanh{
    double operator() (double x) const{
        return std::tanh(x); // Ham tanh(x) = (e^x - e^-x) / (e^x +e^-x)
    }
};

using Activation = std:: variant<relu, sigmoid, Tanh>;

struct ActivationFn{
    double operator()(const Activation& act, double x) const {
        return std::visit([&](auto&& func) { return func(x); }, act);
    }

};

#endif