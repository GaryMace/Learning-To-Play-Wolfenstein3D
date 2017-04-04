//
// Created by gary on 02/01/17.
//
#ifndef TESTBENCH_GENUS_H
#define TESTBENCH_GENUS_H

#include <stdlib.h>   
#include <time.h>      

#include "Species.h"

class Genome;                                               //Forward declaration

class Genus {
private:
    static int innovation;                                  //Instantiating static field here would cause multiple instantiation

public: 
    static std::list<Species> species;                      //All species in population
    static std::list<Species>::iterator currSpeciesItr;     //Maintain a ref to current species/ genome for faster access
    static std::list<Genome>::iterator currGenomeItr;
    static int generation;                                  //Current Generation
    static double maxFitness;                               //Maxfitness across all indiviudals in population
    static int currSpecies;                             
    static int currGenome;

    static std::string backup();
    static int newInnovation();
    static double nextDouble();
    static void addToSpecies(Genome child);
    static void cullSpecies(bool cutToOne);
    static void newGeneration();
    static void rankGlobally();
    static void removeStaleSpecies();
    static void removeWeakSpecies();
    static double totalAverageFitness();
};
#endif  //TESTBENCH_GENUS_H
