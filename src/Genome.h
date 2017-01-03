#include <list>
#include <vector>
#include <map>

#include "Neuron.h"
#include "Genus.h"
using namespace std;
//
// Created by gary on 31/12/16.
//
#define CONNECTIONS 0
#define LINK 1
#define BIAS  2
#define NODE  3
#define ENABLE  4
#define DISABLE  5

class Genome {
private:
    vector<Gene> genes;
    double fitness;
    int maxNeuron;
    int globalRank;
    double mutationRates[7] =
            { MUTATE_CONNECTIONS_CHANCE, MUTATE_LINK_CHANCE, MUTATE_BIAS_CHANCE,
            MUTATE_NODE_CHANCE, MUTATE_ENABLE_CHANCE, MUTATE_DISABLE_CHANCE,
            STEP_SIZE };
    map<int, Neuron> network;
public:
    Genome clone();
    void generateNetwork(); //has Genome param in marIO
    vector<double> evaluateNetwork(vector<double> inputs);  //Has network, inputs param in MarIO
    void nodeMutate();  //MarIO: genome
    void linkMutate(bool forceBias);  //MarIO: genome, forceBias(bool)
    void pointMutate(); //MarIO: genome
    void mutate();      //MarIO: genome
    void muateEnableDisable(bool enable);  //MarIO: genome, enable
    bool containsLink(Gene link);   //MarIO: genes[], link
    double disjoint(Genome genome); //MarIO: genes1[], genes2[]
    int randomNeuron(bool nonInput, bool nonOutput);    //MarIO: genes[], nonInput
    double weights(Genome genome);   //MarIO: genes1[], genes2[]
    bool sameSpecies(Genome genome);    //MarIO: genome1, genome2
};