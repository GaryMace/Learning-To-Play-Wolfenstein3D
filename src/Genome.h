//
// Created by gary on 31/12/16.
//
#ifndef TESTBENCH_GENOME_H
#define TESTBENCH_GENOME_H

#include <list>
#include <map>

#include "Neuron.h"
#include "Def.h"

//used as indexes to mutationRates arr
//TODO: is this wasteful? can't we just put it in Def.h? why does every genome get these....??
#define CONNECTIONS 0
#define LINK 1
#define BIAS  2
#define NODE  3
#define ENABLE  4
#define DISABLE  5
#define STEP 6

class Genome {
public:
    double mutationRates[MUTATION_TYPES];
    std::map<int, Neuron> network;
    std::list<Gene> genes;
    std::list<Gene>::iterator geneItr;
    int maxNeuron;
    int globalRank;
    double fitness; //this will be set in the game loop

    Genome clone();
    std::string backup();
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
    int randomNeuron(bool nonInput, bool nonOutput);
    double weights(Genome genome);
    bool sameSpecies(Genome genome);

    Genome() : maxNeuron(0), globalRank(0), fitness(0) {  //Init Genome instance variables
        initMutationRates();    //I'm not overly fond of this but old C++ doesn't allow array initialisations in class constructors
    }
};
#endif  //TESTBENCH_GENOME_H
