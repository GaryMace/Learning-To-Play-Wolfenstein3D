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
    double mutationRates[MUTATION_TYPES] = {
            MUTATE_CONNECTIONS_CHANCE, MUTATE_LINK_CHANCE, MUTATE_BIAS_CHANCE,
            MUTATE_NODE_CHANCE, MUTATE_ENABLE_CHANCE, MUTATE_DISABLE_CHANCE,
            STEP_SIZE };
    std::map<int, Neuron> network;
    std::vector<Gene> genes;
    int maxNeuron = 0;
    int globalRank = 0;
    double fitness = 0.0;

    Genome clone();
    std::string backup();
    static int compare(const Genome &o1, const Genome &o2);

    void generateNetwork(); //has Genome param in marIO
    std::vector<double> evaluateNetwork(std::vector<double> inputs);  //Has network, inputs param in MarIO
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
};
#endif  //TESTBENCH_GENOME_H
