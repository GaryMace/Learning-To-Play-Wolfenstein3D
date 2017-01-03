//
// Created by gary on 31/12/16.
//
#include <algorithm>
#include "Genome.h"

Genome Genome::clone() {
    Genome genome;

    for (Gene gene : genes)
        genome.genes.push_back(gene);
    genome.maxNeuron = maxNeuron;

    for (int i = 0; i < MUTATION_TYPES; i++)
        genome.mutationRates[i] = mutationRates[i];

    return genome;
}

void Genome::generateNetwork() {
    for (int i = 0; i < INPUTS; i++) {
        Neuron n;
        network.insert(std::make_pair(i, n));
    }
    for (int i = 0; i < OUTPUTS; i++) {
        Neuron n;
        network.insert(std::make_pair(INPUTS + i, n));
    }
    std::sort(genes.begin(), genes.end(), Gene::compare);

    for (Gene gene : genes) {
        if (gene.enabled) {
            map<int, Neuron>::iterator it = network.find(gene.output);
            if (it != network.end()) {
                Neuron n;
                network.insert(std::make_pair(gene.output, n));
            }
            Neuron n = network[gene.output];
            n.inputs.push_back(gene);

            map<int, Neuron>::iterator it2 = network.find(gene.input);
            if (it != network.end()) {
                Neuron n;
                network.insert(std::make_pair(gene.input, n));
            }
        }
    }
}

vector<double> Genome::evaluateNetwork(vector<double> input) {
    for (int i = 0; i < INPUTS; i++)
        network[i].value = input[i];

    for (map<int, Neuron>::iterator it = network.begin(); it != network.end(); it++) {
        int key = it->first;
        if (key < INPUTS + OUTPUTS)
            continue;

        Neuron n1 = it->second;
        double sum = 0.0;
        for (Gene incoming : n1.inputs) {
            Neuron n2 = network[incoming.input];
            sum += incoming.weight * n2.value;
        }

        if (!n1.inputs.empty())
            n1.value = Neuron::sigmoid(sum);
    }

    for (map<int, Neuron>::iterator it = network.begin(); it != network.end(); it++) {
        int key = it->first;
        if (key < INPUTS || key >= INPUTS + OUTPUTS)
            continue;

        Neuron n1 = it->second;
        double sum = 0.0;
        for (Gene incoming : n1.inputs) {
            Neuron n2 = network[incoming.input];
            sum += incoming.weight * n2.value;
        }

        if (!n1.inputs.empty())
            n1.value = Neuron::sigmoid(sum);
    }

    vector<double> output;
    for (int i = 0; i < OUTPUTS; i++)
        output.push_back(network[INPUTS + i].value);

    return output;
}

void Genome::nodeMutate() {
    if (genes.empty())
        return;

    Gene gene = genes[rand() % genes.size()];
    if (!gene.enabled)
        return;
    gene.enabled = false;
    maxNeuron++;

    Gene gene1 = gene.clone();
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
    for (Gene gene : genes) {
        r = ((double) rand() / RAND_MAX) + 1;
        if (r < PERTURBATION_CHANCE)
            gene.weight += (((double) rand() / RAND_MAX) + 1) * step * 2.0 - step;
        else
            gene.weight = (((double) rand() / RAND_MAX) + 1) * 4.0 - 2.0;
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
    newLink.weight = rand() * 4 - 2;
    genes.push_back(newLink);
}

void Genome::mutate() {
    for (int i = 0; i < sizeof(mutationRates); i++) {
        if (rand() % 2 == 0) {
            mutationRates[i] *= 0.95;
        } else {
            mutationRates[i] *= 1.05263;
        }
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
            muateEnableDisable(true);
        prob--;
    }

    prob = mutationRates[DISABLE];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            muateEnableDisable(false);
        prob--;
    }
}

void Genome::muateEnableDisable(bool enable) {
    vector<Gene> candidates;

    for (Gene gene : genes)
        if (gene.enabled != enable)
            candidates.push_back(gene);

    if (candidates.empty())
        return;

    Gene gene = candidates[rand() % candidates.size()];
    gene.enabled = !gene.enabled;
}

bool Genome::containsLink(Gene link) {
    for (Gene gene : genes)
        if (gene.input == link.input && gene.output == link.output)
            return true;
    return false;
}

double Genome::disjoint(Genome genome) {
    double disjointGenes = 0.0;

    for (Gene gene1 : genes) {
        for (Gene gene2 : genome.genes) {
            if (gene1.innovation == gene2.innovation) {
                break;
            }
            disjointGenes++;
        }
    }

    return disjointGenes / max(sizeof(genes), sizeof(genome.genes));
}

int Genome::randomNeuron(bool nonInput, bool nonOutput) {
    vector<int> neurons;    //since list is a doubly linked list, can't access nth item easy
    srand(time(NULL));

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
                sum += abs(gene1.weight - gene2.weight);
                coincident++;
                break;
            }
        }
    }
    return sum / coincident;
}

bool Genome::sameSpecies(Genome genome) {
    double dd = disjoint(genome) * DELTA_DISJOINT;
    double dw = weights(genome) * DELTA_WEIGHTS;

    return dd + dw < DELTA_THRESHOLD;
}

