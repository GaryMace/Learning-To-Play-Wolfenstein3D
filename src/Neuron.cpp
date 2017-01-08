//
// Created by gary on 08/01/17.
//
#include "Neuron.h"
#include <cmath>

double Neuron::sigmoid(double x) {
    return 2.0 / (1.0 + std::exp(-4.9 * x)) - 1.0;
}
