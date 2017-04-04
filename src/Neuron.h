//
// Created by gary on 16/11/16.
//
#ifndef TESTBENCH_NEURON_H
#define TESTBENCH_NEURON_H

#include <list>
#include "Gene.h"

class Neuron {
public:
    double value;				//Value on neuron, if > 0 then it is considered activated
    std::list<Gene> inputs;		//Gene's whose out-neurons connect to this Neuron

    std::string backup();
    static double sigmoid(double x);

    Neuron() : value(0.0) {}
};
#endif  //TESTBENCH_NEURON_H