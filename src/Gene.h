//
// Created by gary on 07/01/17.
//
#ifndef TESTBENCH_GENE_H
#define TESTBENCH_GENE_H

#include <string>

class Gene {
public:
    int input;              //In-neuron
    int output;             //Out-neuron
    double weight;          //Connection weight
    bool enabled;           //Enable-bit
    int innovation;         //Unique connection ID

    Gene clone();
    std::string backup();
    std::string encode();
    static bool compare(const Gene &g1, const Gene &g2);

    Gene() : enabled(true), innovation(0), input(0), output(0), weight(0.0){}   //Init Gene instance variables
};

#endif //TESTBENCH_GENE_H
