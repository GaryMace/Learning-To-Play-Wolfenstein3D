//
// Created by gary on 16/11/16.
//
#ifndef TESTBENCH_NEURON_H
#define TESTBENCH_NEURON_H

#include <vector>
#include "Gene.h"

class Neuron {
public:
    double value = 0.0;
    std::vector<Gene> inputs;

    static double sigmoid(double x);
};
#endif  //TESTBENCH_NEURON_H