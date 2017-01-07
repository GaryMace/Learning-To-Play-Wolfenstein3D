//
// Created by gary on 16/11/16.
//
#include <cmath>
#include <vector>
#include "Gene.h"

using namespace std;

class Neuron {
public:
    double value = 0.0;
    vector<Gene> inputs;

    static double sigmoid(double x);
};

double Neuron::sigmoid(double x) {
    return 2.0 / (1.0 + exp(-4.9 * x)) - 1.0;
}