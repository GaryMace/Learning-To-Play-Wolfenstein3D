//
// Created by gary on 08/01/17.
//
#include <iostream>
#include "Neuron.h"

int main() {
    Neuron n;

    for (int i = 0;  i < 10; i++)  {
        Gene g;
        g.innovation = i;
        n.inputs.push_back(g);
    }

    std::cout << "Neuron genes (expected 0 1 ... 9): ";
    for (n.geneItr = n.inputs.begin(); n.geneItr != n.inputs.end(); n.geneItr++) {
        std::cout << n.geneItr->innovation << " ";
    }
    std::cout << std::endl;

    std::cout << "Neuron sigmoid of 1 (expected 0.985..): " <<  Neuron::sigmoid(1) << std::endl;

    std::cout << n.backup() << std::endl;
    return 0;
}