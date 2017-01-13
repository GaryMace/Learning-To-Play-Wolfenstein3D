//
// Created by gary on 16/11/16.
//
#ifndef TESTBENCH_NEURON_H
#define TESTBENCH_NEURON_H

#include <vector>
#include "Gene.h"

class Neuron {
public:
    double value;
    std::vector<Gene> inputs;

    std::string backup();
    static double sigmoid(double x);
};
#endif  //TESTBENCH_NEURON_H