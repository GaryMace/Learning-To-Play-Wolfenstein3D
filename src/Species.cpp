//
// Created by gary on 08/01/17.
//

#include <sstream>
#include "Species.h"
#include "Genus.h"
#include <iostream>

/*
=================================
=
= {'-'} Species::backup
=
= Outputs a JSON inspired representation of a Species
=
=================================
 */
std::string Species::backup() {
    std::list<Genome>::iterator genomeItr;
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

/*
=================================
=
= {'-'} Species::breedChild
=
= Select either two Genomes and perfrom a crossover and return the child or select one random Genome,
= mutate it and return it.
=
=================================
 */
Genome Species::breedChild() {
    std::list<Genome>::iterator genomeItr;
    Genome child;

    if (Genus::nextDouble() < CROSSOVER_CHANCE) {
        int randGenome1 = rand() % (int) genomes.size();
        int randGenome2 = rand() % (int) genomes.size();
        Genome g1;
        Genome g2;
        for (genomeItr = genomes.begin(); genomeItr != genomes.end(); genomeItr++) {
            if (randGenome1-- == 0) {
                g1 = genomeItr->clone();
                break;
            }
        }
        for (genomeItr = genomes.begin(); genomeItr != genomes.end(); genomeItr++) {
            if (randGenome2-- == 0) {
                g2 = genomeItr->clone();
                break;
            }
        }
        child = crossover(g1, g2);
    } else {
        int randGenome = rand() % (int) genomes.size();

        for (genomeItr = genomes.begin(); genomeItr != genomes.end(); genomeItr++)
            if (randGenome-- == 0) {
                child = genomeItr->clone();
                break;
            }
    }
    child.mutate();
    return child;
}

/*
=================================
=
= {'-'} Species::calculateAverageFitness
=
= Simply sums the globalranks of each member of the current species and divides it by the number
= of Genomes in the current Species.
=
=================================
 */
void Species::calculateAverageFitness() {
    std::list<Genome>::iterator genomeItr;
    double total = 0.0;

    for (genomeItr = genomes.begin(); genomeItr != genomes.end(); genomeItr++)
        total += genomeItr->globalRank;

    averageFitness = total / genomes.size();
}

/*
=================================
=
= {'-'} Species::crossover
=
= Create a new child by combining random aspects of two parents.
=
=================================
 */
Genome Species::crossover(Genome g1, Genome g2) {
    std::list<Gene>::iterator geneItr;
    std::list<Gene>::iterator geneItr2;
    
    if (g2.fitness > g1.fitness) {
        Genome tmp = g1;
        g1 = g2;
        g2 = tmp;
    }
    Genome child;   //The new child
    bool skipDoup = false;  //Some genes are identical across g1 & g2, this avoids adding them twice
    for (geneItr = g1.genes.begin(); geneItr != g1.genes.end(); geneItr++) {
        for (geneItr2 = g2.genes.begin(); geneItr2 != g2.genes.end(); geneItr2++) {
            if (geneItr->innovation == geneItr2->innovation) {
                if (rand() % 2 == 1 && geneItr2->enabled) {  //if true
                    child.genes.push_back(geneItr2->clone());
                    skipDoup = true;
                    break;
                }
            }
        }
        if (!skipDoup)   //prevents adding the same gene twice
            child.genes.push_back(geneItr->clone());
        else
            skipDoup = false;
    } 
    child.maxNeuron = std::max(g1.maxNeuron, g2.maxNeuron);

    for (int i = 0;  i < MUTATION_TYPES; i++)
        child.mutationRates[i] = g1.mutationRates[i];

    return child;
}
