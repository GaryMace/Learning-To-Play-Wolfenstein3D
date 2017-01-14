//
// Created by gary on 31/12/16.
//
#include <algorithm>
#include <sstream>
#include <iostream>
#include "Genome.h"
#include "Genus.h"

/*
 * Outputs string representation of the Genome class
 */
std::string Genome::backup() {
    std::string out = "\n\t\t\t\tGenome{";
    std::string str;

    str = static_cast<std::ostringstream*>(&(std::ostringstream() << fitness))->str();
    out += "fitness=" + str + ",";
    str = static_cast<std::ostringstream*>(&(std::ostringstream() << globalRank))->str();
    out += "\n\t\t\t\t\tglobalRank=" + str + ",";
    str = static_cast<std::ostringstream*>(&(std::ostringstream() << maxNeuron))->str();
    out += "\n\t\t\t\t\tmaxNeuron=" + str + ",";

    out += "\n\t\t\t\t\tmutationRates={";
    for (int i = 0; i < MUTATION_TYPES; i++) {
        double rate = mutationRates[i];
        str = static_cast<std::ostringstream*>(&(std::ostringstream() << rate))->str();
        if (i == MUTATION_TYPES - 1)
            out += str + "},";
        else
            out += str + ",";
    }

    out += "\n\t\t\t\t\tgenes={";
    for (int i = 0; i < genes.size(); i++) {
        Gene gene = genes[i];
        if (i == genes.size() - 1)
            out += gene.backup();
        else
            out += gene.backup() + ",";
    }
    out += "\n\t\t\t\t\t},";

    out += "\n\t\t\t\t\tnetwork={";
    for (std::map<int, Neuron>::iterator it = network.begin(); it != network.end(); it++) {
        str = static_cast<std::ostringstream*>(&(std::ostringstream() << it->first))->str();
        out += "\n\t\t\t\t\t{key=" + str + ",value=" + it->second.backup() + "\t\t},";
    }
    if (!network.empty())
        out = out.substr(0, out.length() - 1);  //Remove last ","

    out += "}\n\t\t\t\t}";  //close network then Genome brackets

    return out;
}

//Duplicate this Genome into a new identical one that has a different reference
Genome Genome::clone() {
    Genome genome;

    for (Gene gene : genes)
        genome.genes.push_back(gene);
    genome.maxNeuron = maxNeuron;

    for (int i = 0; i < MUTATION_TYPES; i++)
        genome.mutationRates[i] = mutationRates[i];

    return genome;
}

//returns 0 if cmp == 0, 1 if cmp > 0 and -1 if cmp < 0
int Genome::compare(const Genome &o1, const Genome &o2) {
    double cmp = o2.fitness - o1.fitness;
    return cmp == 0.0 ? 0 : cmp > 0.0 ? 1 : -1;
}

int Genome::compareByPointer(const Genome *o1, const Genome *o2) {
    double cmp = o2->fitness - o1->fitness;
    return cmp == 0.0 ? 0 : cmp > 0.0 ? 1 : -1;
}

//#fixed
void Genome::initMutationRates() {
    this->mutationRates[CONNECTIONS] = MUTATE_CONNECTIONS_CHANCE;
    this->mutationRates[LINK] = MUTATE_LINK_CHANCE;
    this->mutationRates[BIAS] = MUTATE_BIAS_CHANCE;
    this->mutationRates[NODE] = MUTATE_NODE_CHANCE;
    this->mutationRates[ENABLE] = MUTATE_ENABLE_CHANCE;
    this->mutationRates[DISABLE] = MUTATE_DISABLE_CHANCE;
    this->mutationRates[STEP] = STEP_SIZE;
}

void Genome::generateNetwork() {
    for (int i = 0; i < INPUTS; i++) {  //Make Neurons for all inputs
        Neuron n;
        network.insert(std::make_pair(i, n));
    }
    for (int i = 0; i < OUTPUTS; i++) { //Make Neurons for all outputs
        Neuron n;
        network.insert(std::make_pair(INPUTS + i, n));  //notice output neurons start right after input neurons here
    }
    std::sort(genes.begin(), genes.end(), Gene::compare);   //Order genes based on output

    for (Gene gene : genes) {
        if (gene.enabled) { //If this Gene is enabled
            std::map<int, Neuron>::iterator it = network.find(gene.output);
            if (it != network.end()) {  //If key gene.output doesn't exist
                Neuron n;
                network.insert(std::make_pair(gene.output, n)); //make new entry
            }
            Neuron& n = network[gene.output];   //add this Gene to entry
            n.inputs.push_back(gene);

            std::map<int, Neuron>::iterator it2 = network.find(gene.input);
            if (it != network.end()) {  //If key gene.input doesn't exist
                Neuron n2;
                network.insert(std::make_pair(gene.input, n2));
            }
        }
    }
}

std::vector<double> Genome::evaluateNetwork(std::vector<double> input) {
    for (int i = 0; i < INPUTS; i++)
        network[i].value = input[i];    //Change input values to network

    for (std::map<int, Neuron>::iterator it = network.begin(); it != network.end(); it++) {
        if (it->first < INPUTS + OUTPUTS) //Neuron val > (inputs + outputs)
            continue;

        Neuron& n1 = it->second;    //Neuron& grabs the address of the value from the hashmap
        double sum = 0.0;
        for (Gene incoming : n1.inputs) {   //for each Gene of Neurons inputs
            Neuron n2 = network[incoming.input];
            sum += incoming.weight * n2.value;  //Get sum on Neuron
        }
        if (!n1.inputs.empty())
            n1.value = Neuron::sigmoid(sum);    //which is needed for this step! i.e. in-place map edit
    }

    for (std::map<int, Neuron>::iterator it = network.begin(); it != network.end(); it++) {
        int key = it->first;
        if (key < INPUTS || key >= INPUTS + OUTPUTS)
            continue;

        Neuron& n1 = it->second;
        double sum = 0.0;
        for (Gene incoming : n1.inputs) {   //for each input Gene of Neuron
            Neuron n2 = network[incoming.input];
            sum += incoming.weight * n2.value;
        }

        if (!n1.inputs.empty())
            n1.value = Neuron::sigmoid(sum);
    }

    std::vector<double> output;
    for (int i = 0; i < OUTPUTS; i++)
        output.push_back(network[INPUTS + i].value);
    return output;
}

void Genome::nodeMutate() {
    if (genes.empty())
        return;

    Gene &gene = genes[rand() % genes.size()];
    if (!gene.enabled)
        return;

    gene.enabled = false;
    maxNeuron++;

    Gene gene1 = gene.clone();  //TODO: is this not just making a copy and putting it at the end?
    gene1.output = maxNeuron;
    gene1.weight = 1.0;
    gene1.innovation = Genus::newInnovation();
    gene1.enabled = true;
    genes.push_back(gene1);

    Gene gene2 = gene.clone();
    gene2.input = maxNeuron;
    gene2.innovation = Genus::newInnovation();
    gene2.enabled = true;
    genes.push_back(gene2);
}

void Genome::pointMutate() {
    double step = mutationRates[STEP];

    for (Gene& gene : genes) {
        if (Genus::nextDouble() < PERTURBATION_CHANCE)
            gene.weight += Genus::nextDouble() * step * 2.0 - step;
        else
            gene.weight = Genus::nextDouble() * 4.0 - 2.0;
    }
}

void Genome::linkMutate(bool forceBias) {
    int neuron1 = randomNeuron(false, true);
    int neuron2 = randomNeuron(true, false);

    if (neuron1 <= INPUTS && neuron2 <= INPUTS) //Both input nodes, exit
        return;
    if (neuron2 <= INPUTS) {    //swap output and input
        int tempNeuron = neuron1;
        neuron1 = neuron2;
        neuron2 = tempNeuron;
    }

    Gene newLink;
    newLink.input = neuron1;
    newLink.output = neuron2;

    if (forceBias)
        newLink.input = INPUTS; //TODO: Check back on this part,should be =INPUTS I think

    if (containsLink(newLink))
        return;

    newLink.innovation = Genus::newInnovation();
    newLink.weight = Genus::nextDouble() * 4.0 - 2.0;
    genes.push_back(newLink);
}

void Genome::mutate() {
    for (int i = 0; i < MUTATION_TYPES; i++) {
        if (rand() % 2 == 1)   //if true
            mutationRates[i] *= 0.95;   //TODO: validation of these values?
        else
            mutationRates[i] *= 1.05263;
    }

    if (Genus::nextDouble() < mutationRates[CONNECTIONS])
        pointMutate();

    double prob = mutationRates[LINK];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            linkMutate(false);
        prob--;
    }

    prob = mutationRates[BIAS];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            linkMutate(true);
        prob--;
    }

    prob = mutationRates[NODE];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            nodeMutate();
        prob--;
    }

    prob = mutationRates[ENABLE];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            mutateEnableDisable(true);
        prob--;
    }

    prob = mutationRates[DISABLE];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            mutateEnableDisable(false);
        prob--;
    }
}

void Genome::mutateEnableDisable(bool enable) {
    std::vector<Gene*> candidates;  //vector of pointers

    for (Gene& gene : genes)    //for each actual obj ref
        if (gene.enabled != enable)
            candidates.push_back(&gene);    //store obj ref

    if (candidates.empty())
        return;

    //pointer to obj ref from original vector
    Gene *gene = candidates[rand() % candidates.size()]; //in-place vector edit
    gene->enabled = !gene->enabled; //change value via pointer
}

bool Genome::containsLink(Gene link) {
    for (Gene gene : genes)
        if (gene.input == link.input && gene.output == link.output)
            return true;
    return false;
}

double Genome::disjoint(Genome genome) {
    double disjointGenes = 0.0;
    bool isDisjoint = true;

    for (Gene gene1 : genes) {
        for (Gene gene2 : genome.genes) {
            if (gene1.innovation == gene2.innovation) {
                isDisjoint = false;
                break;
            }
        }
        if (isDisjoint) {
            disjointGenes++;
            isDisjoint = true;
        }
    }

    return disjointGenes / std::max(genes.size(), genome.genes.size());
}

int Genome::randomNeuron(bool nonInput, bool nonOutput) {
    std::vector<int> neurons;    //since list is a doubly linked list, can't access nth item easy
    srand(time(NULL));  //TODO: is this necessary?

    if (!nonInput)
        for (int i = 0; i < INPUTS; ++i)
            neurons.push_back(i);

    if (!nonOutput)
        for (int i = 0; i < OUTPUTS; ++i)
            neurons.push_back(INPUTS + i);

    for (Gene gene : genes) {
        if ((!nonInput || gene.input >= INPUTS)
            && (!nonOutput || gene.input >= INPUTS + OUTPUTS))
            neurons.push_back(gene.input);
        if ((!nonInput || gene.output >= INPUTS)
            && (!nonOutput || gene.output >= INPUTS + OUTPUTS))
            neurons.push_back(gene.output);
    }

    return neurons[rand() % neurons.size()];    //Does this need to be a pointer or just the val?
}

double Genome::weights(Genome genome) {
    double sum = 0.0;
    double coincident = 0.0;

    for (Gene gene1 : genes) {
        for (Gene gene2 : genome.genes) {
            if (gene1.innovation == gene2.innovation) {
                sum += std::fabs(gene1.weight - gene2.weight); //std::fabs is abs() on a float
                coincident++;
                break;
            }
        }
    }
    if (sum == 0 && coincident == 0)    //since  0.0/0.0 in C++ is -nan ...
        return 0;
    else
        return sum / coincident;
}

bool Genome::sameSpecies(Genome genome) {
    double dd = disjoint(genome) * DELTA_DISJOINT;
    double dw = weights(genome) * DELTA_WEIGHTS;

    return dd + dw < DELTA_THRESHOLD;
}