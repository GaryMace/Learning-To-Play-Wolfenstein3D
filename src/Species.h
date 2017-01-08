//
// Created by gary on 07/01/17.
//
#ifndef TESTBENCH_SPECIES_H
#define TESTBENCH_SPECIES_H

#include "Genome.h"

class Species {
public:
    std::vector<Genome> genomes;
    double topFitness = 0.0;
    double averageFitness = 0.0;
    int staleness = 0;

    Genome breedChild();
    void calculateAverageFitness();
    Genome crossover(Genome g1, Genome g2);
};
#endif  //TESTBENCH_SPECIES_H
