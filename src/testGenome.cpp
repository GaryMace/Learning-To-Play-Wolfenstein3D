//
// Created by gary on 08/01/17.
//
#include <iostream>
#include "Genome.h"

int main() {
    Genome g1;
    Genome g2;
    g2 = g1.clone();

    bool identical = true;
    for (int i = 0; i < MUTATION_TYPES; i++)
        if (g1.mutationRates[i] != g2.mutationRates[i])
            identical = false;
    std::cout << "Test Genome::clone (expected 1): " << identical << std::endl;

    std::cout << "g1 network size on init (expected 0): " << g1.network.size() << std::endl;
    g1.generateNetwork();

    std::vector<double> inputs = {1,2,3,4};
    std::vector<double> out = g1.evaluateNetwork(inputs);
    std::cout << "g1 evalNetwork size (expected 8): " << out.size() << std::endl;
    std::cout << "g1 output vals (expected eight default vals i.e. 0): ";
    for (int i = 0; i < out.size(); i++)
        std::cout << out[i] << " ";
    std::cout << std::endl;

    std::cout << "g1 network size instantiated (expected 12): " << g1.network.size() << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::compare()" << std::endl;
    g1.fitness = 5;
    g2.fitness = 4;
    std::cout << "Compare g1 & g2 (expected -1): " << Genome::compare(g1, g2) << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::nodeMutate()" << std::endl;
    Gene g;
    g1.genes.push_back(g);

    std::cout << std::endl;
    std::cout << "g1 before node mutate: " << std::endl;
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << std::endl;
    }
    std::cout << "g1 after node mutate: " << std::endl;
    g1.nodeMutate();
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << "\n" << std::endl;
    }

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::randomNeuron()" << std::endl;
    std::cout << "g1 random neuron: " << g1.randomNeuron(false, true) << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::pointMutate()" << std::endl;
    std::cout << "g1 before point mutate: " << std::endl;
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << std::endl;
    }
    g1.pointMutate();
    std::cout << "g1 after point mutate: " << std::endl;
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << "\n" << std::endl;
    }
    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::linkMutate()" << std::endl;
    std::cout << "g1 before link mutate(forceBias - false): " << std::endl;
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << std::endl;
    }
    g1.linkMutate(false);
    std::cout << "g1 after link mutate(forceBias - false): " << std::endl;
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << std::endl;
    }
    std::cout << "g1 before link mutate(forceBias - true): " << std::endl;
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << std::endl;
    }    g1.linkMutate(true);
    std::cout << "g1 after link mutate(forceBias - true): " << std::endl;
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << std::endl;
    }
    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::mutateEnableDisable()" << std::endl;
    g1.mutateEnableDisable(true);
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << std::endl;
    }
    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::mutate()" << std::endl;
    g1.mutate();
    for (int i = 0; i < g1.genes.size(); i++) {
        Gene gene = g1.genes[i];
        std::cout << gene.backup() << std::endl;
    }
    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::containsLink()" << std::endl;
    Gene testGene;
    testGene.input = 1;
    testGene.output = 0;
    std::cout << "g1 contains testGene (expected 1): " << g1.containsLink(testGene) << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::disjoint()" << std::endl;
    std::cout << "g1 disjoint from g2 (expected 1): " << g1.disjoint(g2) << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::weights()" << std::endl;
    std::cout << "g1 weights to g2 (expected 0): " << g1.weights(g2) << "\n" << std::endl;

    std::cout << "/////////////////////////////////////////" << std::endl;
    std::cout << "// Genome::sameSpecies()" << std::endl;
    std::cout << "g1 same species as g2 (expected 0): " << g1.sameSpecies(g2) << std::endl;

    std::cout << g1.backup() << std::endl;
    return 0;
}