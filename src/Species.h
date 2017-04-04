//
// Created by gary on 07/01/17.
//
#ifndef TESTBENCH_SPECIES_H
#define TESTBENCH_SPECIES_H

#include "Genome.h"

class Species {
public:
    std::list<Genome> genomes;                  //Members of the current Species
    Genome topGenome;                           //Best performer for the current Species
    double topFitness;                          //Fitness of top performer
    double averageFitness;                      //Average fitness of all this Species members
    int staleness;                              //Measure of poor performance accross all indiviudals in the current Species

    std::string backup();
    Genome breedChild();
    void calculateAverageFitness();
    Genome crossover(Genome g1, Genome g2);
    Species() : topFitness(0.0), averageFitness(0.0), staleness(0) { }
};
#endif  //TESTBENCH_SPECIES_H
