//
// Created by gary on 02/01/17.
//
#ifndef TESTBENCH_GENUS_H
#define TESTBENCH_GENUS_H

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "Species.h"
#include "Genome.h"

#endif  //TESTBENCH_GENUS_H

#define POPULATION 300  //TODO: refactor
#define STALE_SPECIES 15
#define TIMEOUT 20
#define MUTATION_TYPES 7    //consider moving later
#define INPUTS 4    //refactor
#define OUTPUTS 8   //num buttons, refactor

#define DELTA_DISJOINT 2.0
#define DELTA_WEIGHTS  0.4
#define DELTA_THRESHOLD 1.0

#define MUTATE_CONNECTIONS_CHANCE 0.25
#define MUTATE_LINK_CHANCE 2.0
#define MUTATE_NODE_CHANCE 0.5
#define MUTATE_BIAS_CHANCE 0.4
#define MUTATE_ENABLE_CHANCE 0.2
#define MUTATE_DISABLE_CHANCE 0.4
#define CROSSOVER_CHANCE 0.75
#define PERTURBATION_CHANCE 0.9
#define STEP_SIZE 0.1

#define MAX_NODES 10000

class Genus {
private:
    static int innovation;  //instantiating static field here would cause multiple instantiation

public:
    static vector<Species> species;
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