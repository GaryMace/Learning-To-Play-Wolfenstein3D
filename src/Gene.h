//
// Created by gary on 07/01/17.
//
#ifndef TESTBENCH_GENE_H
#define TESTBENCH_GENE_H

#include <string>

//TODO: set defualt values!
class Gene {
public:
    int input;  //in-node
    int output; //out-node
    double weight;  //connection weight
    bool enabled = true;    //enable-bit
    int innovation; //connection ID

    Gene clone();
    std::string backup();
    static bool compare(const Gene &g1, const Gene &g2);
};

#endif //TESTBENCH_GENE_H
