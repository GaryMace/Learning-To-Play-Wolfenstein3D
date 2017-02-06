//
// Created by gary on 31/12/16.
//
#ifndef TESTBENCH_GENOME_H
#define TESTBENCH_GENOME_H

#include <vector>
#include <map>

#include "Neuron.h"
#include "Def.h"

//used as indexes to mutationRates arr
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
    std::vector<Gene> genes;
    int maxNeuron;
    int globalRank;
    double fitness;

    Genome clone();
    std::string backup();
    static int compare(const Genome &o1, const Genome &o2);
    static int compareByPointer(const Genome *o1, const Genome *o2);

    void initMutationRates();
    void generateNetwork(); //has Genome param in marIO
    std::vector<bool> evaluateNetwork(int inputs[][SEARCH_GRID]);  //Has network, inputs param in MarIO
    void nodeMutate();  //MarIO: genome
    void linkMutate(bool forceBias);  //MarIO: genome, forceBias(bool)
    void pointMutate(); //MarIO: genome
    void mutate();      //MarIO: genome
    void mutateEnableDisable(bool enable);  //MarIO: genome, enable
    bool containsLink(Gene link);   //MarIO: genes[], link
    double disjoint(Genome genome); //MarIO: genes1[], genes2[]
    int randomNeuron(bool nonInput, bool nonOutput);    //MarIO: genes[], nonInput
    double weights(Genome genome);   //MarIO: genes1[], genes2[]
    bool sameSpecies(Genome genome);    //MarIO: genome1, genome2

    Genome() : maxNeuron(0), globalRank(0), fitness(0.0) {  //Init Genome instance variables
        initMutationRates();    //I'm not overly fond of this but old C++ doesn't allow array initialisations in class constructors
    }
};
#endif  //TESTBENCH_GENOME_H
