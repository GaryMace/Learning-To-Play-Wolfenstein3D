//
// Created by gary on 07/01/17.
//
#ifndef TESTBENCH_SPECIES_H
#define TESTBENCH_SPECIES_H

#include "Genome.h"
#include "Genus.h"

#endif  //TESTBENCH_SPECIES_H

class Species {
public:
    vector<Genome> genomes;
    double topFitness = 0.0;
    double averageFitness = 0.0;
    int staleness = 0;

    Genome breedChild();
    void calculateAverageFitness();
    Genome crossover(Genome g1, Genome g2);
};

Genome Species::breedChild() {
    Genome child;

    if (Genus::nextDouble() < CROSSOVER_CHANCE) {
        Genome g1 = genomes[rand() % genomes.size()];
        Genome g2 = genomes[rand() % genomes.size()];
        child = crossover(g1, g2);
    } else
        child = genomes[rand() % genomes.size()].clone();
    child.mutate();

    return child;
}

void Species::calculateAverageFitness() {
    double total = 0.0;

    for (Genome genome : genomes)
        total += genome.globalRank;

    averageFitness = total / genomes.size();
}

Genome Species::crossover(Genome g1, Genome g2) {
    if (g2.fitness > g1.fitness) {
        Genome tmp = g1;
        g1 = g2;
        g2 = tmp;
    }
    Genome child;   //The new child
    bool skipDoup = false;  //Some genes are identical across g1 & g2, this avoids adding them twice

    for (Gene gene1 : g1.genes) {
        for (Gene gene2 : g2.genes) {
            if (gene1.innovation == gene2.innovation) {
                if (rand() % 2 == 1 && gene2.enabled) {  //if true
                    child.genes.push_back(gene2.clone());
                    skipDoup = true;
                    break;
                }
            }
        }
        if (!skipDoup)   //prevents adding the same gene twice
            child.genes.push_back(gene1.clone());
        else
            skipDoup = false;
    }
    child.maxNeuron = max(g1.maxNeuron, g2.maxNeuron);

    for (int i = 0;  i < MUTATION_TYPES; i++)
        child.mutationRates[i] = g1.mutationRates[i];

    return child;
}