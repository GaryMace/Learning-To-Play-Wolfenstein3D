//
// Created by gary on 31/12/16.
//
#ifndef TESTBENCH_GENOME_H
#define TESTBENCH_GENOME_H

#include <list>
#include <map>

#include "Neuron.h"
#include "Def.h"
#include "wl_def.h"

//Used as meaningful indexes into the Mutation rates array.
#define CONNECTIONS 0
#define LINK 1
#define BIAS  2
#define NODE  3
#define ENABLE  4
#define DISABLE  5
#define STEP 6

class Genome {
public:
    double mutationRates[MUTATION_TYPES];                               //Mutation rates for Genetic Algorithms
    std::map<int, Neuron> network;                                      //The Phenotype of the Genome
    std::list<Gene> genes;                                              //An encoding of this Genome's network represented as a list of Genes
    int maxNeuron;                                                      //Max Neuron of 274 indicates no Hidden Layer, any higher and this Genome has some Hidden Neurons
    int globalRank;                                                     //A ranking of this Genome relative to all other Genomes in Population
    double fitness;                                                     //This will be set in the game loop

    Genome clone();
    std::string backup();
    std::string encode();
    static int compare(const Genome &o1, const Genome &o2);
    static int compareByPointer(const Genome *o1, const Genome *o2);

    void initMutationRates();
    void generateNetwork();
    bool* evaluateNetwork(int inputs[][SEARCH_GRID]);
    void nodeMutate();
    void linkMutate(bool forceBias);
    void pointMutate();
    void mutate();
    void mutateEnableDisable(bool enable);
    bool containsLink(Gene link);
    double disjoint(Genome genome);
    int randomNeuron(bool nonInput);
    double weights(Genome genome);
    bool sameSpecies(Genome genome);

    Genome() : maxNeuron(TOTAL_INPUTS - 1), globalRank(0), fitness(0) { //Init Genome instance variables
        initMutationRates();                                            //I'm not overly fond of this but old C++ doesn't allow array initialisations in class constructors
    }
};
#endif  //TESTBENCH_GENOME_H
