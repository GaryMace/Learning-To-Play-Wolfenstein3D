//
// Created by gary on 31/12/16.
//
#include <algorithm>
#include <iostream>
#include "Genome.h"
#include "Genus.h"

std::string Genome::backup() {
    std::string out = "\n\tGenome{";

    out += "fitness=" + std::to_string(fitness) + ",";
    out += "\n\t\tglobalRank=" + std::to_string(globalRank) + ",";
    out += "\n\t\tmaxNeuron=" + std::to_string(maxNeuron) + ",";

    out += "\n\t\tmutationRates={";
    for (int i = 0; i < MUTATION_TYPES; i++) {
        double rate = mutationRates[i];
        if (i == MUTATION_TYPES - 1)
            out += std::to_string(rate) + "},";
        else
            out += std::to_string(rate) + ",";
    }

    out += "\n\t\tgenes={";
    for (int i = 0; i < genes.size(); i++) {
        Gene gene = genes[i];
        if (i == genes.size() - 1)
            out += gene.backup();
        else
            out += gene.backup() + ",";
    }
    out += "},";

    out += "\n\t\tnetwork={";
    for (std::map<int, Neuron>::iterator it = network.begin(); it != network.end(); it++)
        out += "\n\t\t{key=" + std::to_string(it->first) + ",value=" + it->second.backup() + "\t\t},";
    if (!network.empty())
        out = out.substr(0, out.length() - 1);  //Remove last ","

    out += "}\n\t}";

    return out;
}

Genome Genome::clone() {
    Genome genome;

    for (Gene gene : genes)
        genome.genes.push_back(gene);
    genome.maxNeuron = maxNeuron;

    for (int i = 0; i < MUTATION_TYPES; i++)
        genome.mutationRates[i] = mutationRates[i];

    return genome;
}

int Genome::compare(const Genome &o1, const Genome &o2) {
    double cmp = o2.fitness - o1.fitness;
    return cmp == 0.0 ? 0 : cmp > 0.0 ? 1 : -1;
}

void Genome::generateNetwork() {
    for (int i = 0; i < INPUTS; i++) {  //Make Neurons for all inputs
        Neuron n;
        network.insert(std::make_pair(i, n));
    }
    for (int i = 0; i < OUTPUTS; i++) { //Make Neurons for all outputs
        Neuron n;
        network.insert(std::make_pair(INPUTS + i, n));
    }
    std::sort(genes.begin(), genes.end(), Gene::compare);   //Order genes based on output

    for (Gene gene : genes) {   //TODO: be careful, do we want copies or pointers?
        if (gene.enabled) {
            std::map<int, Neuron>::iterator it = network.find(gene.output);
            if (it != network.end()) {
                Neuron n;
                network.insert(std::make_pair(gene.output, n));
            }
            Neuron n = network[gene.output];
            n.inputs.push_back(gene);

            std::map<int, Neuron>::iterator it2 = network.find(gene.input);
            if (it != network.end()) {
                Neuron n2;
                network.insert(std::make_pair(gene.input, n2));
            }
        }
    }
}

std::vector<double> Genome::evaluateNetwork(std::vector<double> input) {
    for (int i = 0; i < INPUTS; i++)
        network[i].value = input[i];

    for (std::map<int, Neuron>::iterator it = network.begin(); it != network.end(); it++) {
        int key = it->first;
        if (key < INPUTS + OUTPUTS)
            continue;

        Neuron& n1 = it->second;    //Neuron& grabs the address of the value from the hashmap
        double sum = 0.0;
        for (Gene incoming : n1.inputs) {
            Neuron n2 = network[incoming.input];
            sum += incoming.weight * n2.value;
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
        for (Gene incoming : n1.inputs) {
            Neuron n2 = network[incoming.input];
            sum += incoming.weight * n2.value;
        }

        if (!n1.inputs.empty())
            n1.value = Neuron::sigmoid(sum);
    }

    std::vector<double> output;
    for (int i = 0; i < OUTPUTS; i++)
        output.push_back(network[INPUTS + i].value);

    std::cout << backup() << std::endl;
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
    double r;
    double step = mutationRates[6];

    for (Gene& gene : genes) {
        r = Genus::nextDouble();    //TODO: why are we rerandomising again?
        if (r < PERTURBATION_CHANCE)
            gene.weight += Genus::nextDouble() * step * 2.0 - step;
        else
            gene.weight = Genus::nextDouble() * 4.0 - 2.0;
    }
}

void Genome::linkMutate(bool forceBias) {
    int neuron1 = randomNeuron(false, true);
    int neuron2 = randomNeuron(true, false);

    Gene newLink;
    newLink.input = neuron1;
    newLink.output = neuron2;

    if (forceBias)
        newLink.input = INPUTS - 1; //TODO: Check back on this part

    if (containsLink(newLink))
        return;

    newLink.innovation = Genus::newInnovation();
    newLink.weight = rand() * 4 - 2;    //TODO: Rand here should be modded to something
    genes.push_back(newLink);
}

void Genome::mutate() {
    for (int i = 0; i < MUTATION_TYPES; i++) {
        if (rand() % 2 == 1)   //if true
            mutationRates[i] *= 0.95;
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

    return neurons[rand() % neurons.size()];
}

double Genome::weights(Genome genome) {
    double sum = 0.0;
    double coincident = 0.0;

    for (Gene gene1 : genes) {
        for (Gene gene2 : genome.genes) {
            if (gene1.innovation == gene2.innovation) {
                sum += abs(gene1.weight - gene2.weight);    //TODO: what's with this warning?
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