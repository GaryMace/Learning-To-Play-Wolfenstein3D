//
// Created by gary on 02/01/17.
//
#ifndef TESTBENCH_GENUS_H
#define TESTBENCH_GENUS_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Species.h"

class Genome;   //forward declaration

class Genus {
private:
    static int innovation;  //instantiating static field here would cause multiple instantiation

public:
    static std::vector<Species> species;
    static int generation;
    static double maxFitness;

    static int newInnovation();
    static double nextDouble();
    static void addToSpecies(Genome child);
    static void cullSpecies(bool cutToOne);
    static void initializeGenus();
    static void newGeneration();
    static void rankGlobally();
    static void removeStaleSpecies();
    static void removeWeakSpecies();
    static double totalAverageFitness();
};
#endif  //TESTBENCH_GENUS_H
