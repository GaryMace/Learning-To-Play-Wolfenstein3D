//
// Created by gary on 08/01/17.
//

#include <sstream>
#include "Species.h"
#include "Genus.h"

std::string Species::backup() {
    std::string out = "\n\t\tSpecies{";
    std::string str;

    str = static_cast<std::ostringstream*>(&(std::ostringstream() << topFitness))->str();
    out += "topFitness=" + str + ",";
    str = static_cast<std::ostringstream*>(&(std::ostringstream() << averageFitness))->str();
    out += "\n\t\t\taverageFitness=" + str + ",";
    str = static_cast<std::ostringstream*>(&(std::ostringstream() << staleness))->str();
    out += "\n\t\t\tstaleness=" + str + ",";

    out += "\n\t\t\tgenomes={";
    for (genomeItr = genomes.begin(); genomeItr != genomes.end(); genomeItr++)
            out += genomeItr->backup() + ",";
    out = out.substr(0, out.length() - 1);  //Remove last ","

    out += "\n\t\t\t}\n\t\t}";  //close genomes{} then Species brackets

    return out;
}

Genome Species::breedChild() {
    Genome child;

    if (Genus::nextDouble() < CROSSOVER_CHANCE) {
        int randGenome1 =  0 + (rand() % (int)(genomes.size() - 0 + 1));
        int randGenome2 =  0 + (rand() % (int)(genomes.size() - 0 + 1));
        Genome g1;
        Genome g2;
        for (genomeItr = genomes.begin(); genomeItr != genomes.end(); genomeItr++)
            if (--randGenome1 <= 0) {
                g1 = genomeItr->clone();
                break;
            }
        for (genomeItr = genomes.begin(); genomeItr != genomes.end(); genomeItr++)
            if (--randGenome2 <= 0) {
                g2 = genomeItr->clone();
                break;
            }

        child = crossover(g1, g2);
    } else {
        int randGenome = 0 + (rand() % (int)(genomes.size() - 0 + 1));

        for (genomeItr = genomes.begin(); genomeItr != genomes.end(); genomeItr++)
            if (--randGenome <= 0) {
                child = genomeItr->clone();
                break;
            }
    }
    child.mutate();

    return child;
}

void Species::calculateAverageFitness() {
    double total = 0.0;

    for (genomeItr = genomes.begin(); genomeItr != genomes.end(); genomeItr++)
        total += genomeItr->globalRank;

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

    for (g1.geneItr = g1.genes.begin(); g1.geneItr != g1.genes.end(); g1.geneItr++) {
        for (g2.geneItr = g2.genes.begin(); g2.geneItr != g2.genes.end(); g2.geneItr++) {
            if (g1.geneItr->innovation == g2.geneItr->innovation) {
                if (rand() % 2 == 1 && g2.geneItr->enabled) {  //if true
                    child.genes.push_back(g2.geneItr->clone());
                    skipDoup = true;
                    break;
                }
            }
        }
        if (!skipDoup)   //prevents adding the same gene twice
            child.genes.push_back(g1.geneItr->clone());
        else
            skipDoup = false;
    }
    child.maxNeuron = std::max(g1.maxNeuron, g2.maxNeuron);

    for (int i = 0;  i < MUTATION_TYPES; i++)
        child.mutationRates[i] = g1.mutationRates[i];

    return child;
}

void Species::~Species() {
    delete genomes;
    delete genomeItr;
    delete topFitness;
    delete averageFitness;
    delete staleness;
}