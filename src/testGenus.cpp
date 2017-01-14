#include <iostream>
#include "Genus.h"

//
// Created by gary on 11/01/17.
//
int main() {
    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::nextDouble()" << std::endl;
    std::cout << "Genus rand double (expected 0 <= x <= 1): " << Genus::nextDouble() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::nextInnovation()" << std::endl;
    std::cout << "Genus new Innovation (expected 9): " << Genus::newInnovation() << "\n" << std::endl;

    Species s1;
    Species s2;
    Species s3;
    Genome g2;
    s1.averageFitness = 5;
    s3.averageFitness = 10;
    s1.genomes.push_back(g2);
    s2.genomes.push_back(g2);
    s3.genomes.push_back(g2);
    Genus::species.push_back(s1);
    Genus::species.push_back(s2);
    Genus::species.push_back(s3);

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::addToSpecies()" << std::endl;
    Genome g1;
    Genus::addToSpecies(g1);
    std::cout << "Total species increase (expected 4): " << Genus::species.size() << std::endl;
    std::cout << Genus::backup() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::initialiseGenus()" << std::endl;
    Genus::initializeGenus();
    std::cout << Genus::backup() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::rankGlobally()" << std::endl;
    Genus::rankGlobally();
    std::cout << Genus::backup() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::totalAverageFitness()" << std::endl;
    std::cout << "Total avg fitness (expected 15): " << Genus::totalAverageFitness() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::removeStaleSpecies()" << std::endl;
    int i = 0;
    for (int j = 0; j < Genus::species.size(); j++) {
        Species& s = Genus::species[j];
        for (int k = 0; k < s.genomes.size(); k++) {
            Genome& genome = s.genomes[k];
            genome.fitness = i++;
        }
    }

    Genus::removeStaleSpecies();
    std::cout << Genus::backup() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::removeWeakSpecies()" << std::endl;
    Genus::removeWeakSpecies();
    std::cout << Genus::backup() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::cullSpecies()" << std::endl;
    for (int j = 0; j < Genus::species.size(); j++) {
        Species& s = Genus::species[j];
        Genome g;   //Add extra Genomes for testing
        s.genomes.push_back(g);
    }
    Genus::cullSpecies(true);
    std::cout << Genus::backup() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::newGeneration()" << std::endl;
    Genus::newGeneration();
    std::cout << Genus::backup() << "\n" << std::endl;
    return 0;
}
