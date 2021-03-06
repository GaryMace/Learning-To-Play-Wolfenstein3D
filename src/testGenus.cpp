#include <iostream>
#include "Genus.h"

//
// Created by gary on 11/01/17.
//
int main() {
    std::list<Species>::iterator speciesItr;
    std::list<Genome>::iterator genomeItr;
    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::nextDouble()" << std::endl;
    std::cout << "Genus rand double (expected 0 <= x <= 1): " << Genus::nextDouble() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::nextInnovation()" << std::endl;
    std::cout << "Genus new Innovation (expected 10): " << Genus::newInnovation() << "\n" << std::endl;

    Species s1;
    Species s2;
    Species s3;
    Genome g2;
    s1.averageFitness = 5;
    s3.averageFitness = 10;
    s1.genomes.push_back(g2);
    s2.genomes.push_back(g2);
    s3.genomes.push_back(g2);
    s1.topFitness = 100;
    s2.staleness = 100;
    s3.staleness =100;
    Genus::species.push_back(s1);
    Genus::species.push_back(s2);
    Genus::species.push_back(s3);

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::addToSpecies()" << std::endl;
    Genome g1;
    Genus::addToSpecies(g1);
    std::cout << "Total species increase (expected 3): " << Genus::species.size() << std::endl;
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

    for (speciesItr = Genus::species.begin(); speciesItr != Genus::species.end(); speciesItr++) {
        Species* s = &(*speciesItr);
    //for (int j = 0; j < Genus::species.size(); j++) {
        for (genomeItr = s->genomes.begin(); genomeItr != s->genomes.end(); genomeItr++) {
            //for (int k = 0; k < s.genomes.size(); k++) {
            genomeItr->fitness = i++;
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
    for (speciesItr = Genus::species.begin(); speciesItr != Genus::species.end(); speciesItr++) {
        //for (int j = 0; j < Genus::species.size(); j++) {
        Species* s = &(*speciesItr);
        Genome g;   //Add extra Genomes for testing
        s->genomes.push_back(g);
    }
    Genus::cullSpecies(true);
    std::cout << Genus::backup() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genus::newGeneration()" << std::endl;
    Genus::newGeneration();
    std::cout << Genus::backup() << "\n" << std::endl;
    return 0;
}
