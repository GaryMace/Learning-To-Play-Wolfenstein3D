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

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Species::crossover()" << std::endl;
    g2.fitness = 10;
    Genome child = s.crossover(g1, g2);
    std::cout << "Child generated: " << child.backup() << "\nEnd of child\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Species::breedChild()" << std::endl;
    Genome offspring = s.breedChild();
    std::cout << "Offspring generated: " << offspring.backup() << "\nEnd of Offspring\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Species::calculateAverageFitness()" << std::endl;
    s.genomes[0].globalRank = 3;
    s.genomes[1].globalRank = 5;
    s.calculateAverageFitness();
    std::cout << "Average fitness of Species (expected 4): " << s.averageFitness << std::endl;
    return 0;
}