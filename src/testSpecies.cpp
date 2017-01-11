//
// Created by gary on 10/01/17.
//
#include <iostream>
#include "Species.h"

int main() {
    Species s;
    Genome g1;
    Genome g2;
    Neuron n;
    std::vector<Gene> test;

    for (int i = 0; i < 4; i++) {
        Gene g;
        g.output = i;
        g1.genes.push_back(g);
        g1.network.insert(std::make_pair(g.output, n));
        g.output = i*2;
        g2.genes.push_back(g);
        g2.network.insert(std::make_pair(g.output, n));
    }
    s.genomes.push_back(g1);
    s.genomes.push_back(g2);

    std::cout << s.backup() << std::endl;
    return 0;
}