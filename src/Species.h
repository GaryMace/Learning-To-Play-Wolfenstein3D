//
// Created by gary on 07/01/17.
//
#ifndef TESTBENCH_SPECIES_H
#define TESTBENCH_SPECIES_H

#include "Genome.h"

class Species {
public:
    std::list<Genome> genomes;
    std::list<Genome>::iterator genomeItr;
    double topFitness;
    double averageFitness;
    int staleness;

    std::string backup();

    Genome breedChild();
    void calculateAverageFitness();
    Genome crossover(Genome g1, Genome g2);
    Species() : topFitness(0.0), averageFitness(0.0), staleness(0) { }
};
#endif  //TESTBENCH_SPECIES_H
