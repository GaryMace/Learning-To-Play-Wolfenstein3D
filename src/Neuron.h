//
// Created by gary on 16/11/16.
//
#ifndef TESTBENCH_NEURON_H
#define TESTBENCH_NEURON_H

#include <list>
#include "Gene.h"

class Neuron {
public:
    double value;
    std::list<Gene> inputs;
    std::list<Gene>::iterator geneItr;

    std::string backup();
    static double sigmoid(double x);

    Neuron() : value(0.0) {}
    ~Neuron();
};
#endif  //TESTBENCH_NEURON_H