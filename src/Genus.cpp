//
// Created by gary on 02/01/17.
//
#include "Genus.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>

// instantiate static members
std::list<Species>::iterator Genus:: currSpeciesItr;
std::list<Genome>::iterator Genus::currGenomeItr;
std::list<Species>::iterator Genus::speciesItr;
std::list<Species> Genus::species;
int Genus::innovation = OUTPUTS;
double Genus::maxFitness = 0.0;
int Genus::currSpecies = 0;
int Genus::generation = 0;
int Genus::currGenome = 0;

void Genus::loadSaveState(std::string saveState) {

}

std::string Genus::backup() {
    std::string out = "Genus{";
    std::string str;

    str = static_cast<std::ostringstream*>(&(std::ostringstream() << generation))->str();
    out += "generation=" + str + ",\n";
    str = static_cast<std::ostringstream*>(&(std::ostringstream() << maxFitness))->str();
    out += "\tmaxFitness=" + str + ",\n";
    out += "\tspecies={";
    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++)
        //if (i == Genus::species.size() - 1)
        //    out += species.backup() + "\n\t}";
        out += speciesItr->backup() + ",";
    out = out.substr(0, out.length() - 1);  //Remove last ","
    out += "\n\t}";

    return out + "\n}";
}

int Genus::newInnovation() {
    return ++Genus::innovation;
}

double Genus::nextDouble() {
    return ((double)rand() / RAND_MAX);
}

void Genus::addToSpecies(Genome child) {
    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        if (child.sameSpecies(*speciesItr->genomes.begin())) {  //child.sameSpecies(First node in list in curr species)
            speciesItr->genomes.push_back(child);
            return;
        }
    }
    Species childSpecies;
    childSpecies.genomes.push_back(child);
    species.push_back(childSpecies);
}

void Genus::cullSpecies(bool cutToOne) {
    std::list<Genome>::iterator removeItr;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        Species& species = *speciesItr;
        species.genomes.sort(Genome::compare);   //sort based on fitness

        double remaining  = std::ceil(species.genomes.size() / 2.0); //TODO: why does this need to be a double?
        if (cutToOne)
            remaining = 1.0;

        removeItr = species.genomes.end();
        while (species.genomes.size() > remaining) {
            if (removeItr != species.genomes.end())
                species.genomes.erase(removeItr);   //Remove weak species from end
            removeItr--;
        }
    }
}

void Genus::newGeneration() {
    Genus::cullSpecies(false); //Cull bottom half of each species
    Genus::rankGlobally();
    Genus::removeStaleSpecies();
    Genus::rankGlobally();
    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        Species& species = *speciesItr;
        species.calculateAverageFitness();
    }
    Genus::removeWeakSpecies();

    double sum = Genus::totalAverageFitness();
    std::list<Genome> children;
    std::list<Genome>::iterator genomeItr;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        Species& species = *speciesItr;
        double breed = std::floor(species.averageFitness / sum * POPULATION) - 1.0;
        for (int j = 0; j < breed; j++)
            children.push_back(species.breedChild());
    }

    Genus::cullSpecies(true);  //Cull all but the top member of each species
    while (children.size() + species.size() < POPULATION) {
        int randSpecies = 0 + (rand() % (int)(species.size() - 0 + 1));
        Species *species1;
       /* std::list<Species>::iterator it = species.begin();

        while(randSpecies > 0) {
            randSpecies--;
            it++;
        }
        Species *species = &(*it);
        children.push_back(species->breedChild());*/

        for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
            if (--randSpecies <= 0) {
                species1 = &(*speciesItr);
                break;
            }
        }
        children.push_back(species1->breedChild());
    }

    for (genomeItr = children.begin(); genomeItr != children.end(); genomeItr++)
        Genus::addToSpecies(*genomeItr);

    //for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++)
    //    std::cout << speciesItr->backup() << std::endl;
    Genus::generation++;
    //initialiseRun() ????
}

void Genus::rankGlobally() {
    std::list<Genome>::iterator genomeItr;
    std::list<Genome*> global;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++)
        for (genomeItr = speciesItr->genomes.begin(); genomeItr != speciesItr->genomes.end(); genomeItr++)
            global.push_back(&(*genomeItr));  //Push address to original Genomes

    //Sort by pointer, since we're directly editing original list later
//    std::sort(global.begin(), global.end(), Genome::compareByPointer);
    global.sort(Genome::compareByPointer);
/*
    for (int i = 0; i < global.size(); i++)
        global[i]->globalRank = i;  //Direct edits on original list*/
    int idx = 0;
    for (std::list<Genome*>::iterator globalItr = global.begin(); globalItr!= global.end(); globalItr++) {
        Genome *g = &(**globalItr);
        g->globalRank = idx++;
    }
}

void Genus::removeStaleSpecies() {
    std::list<Species> survivors;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        speciesItr->genomes.sort(Genome::compare);

        if (speciesItr->genomes.begin()->fitness > speciesItr->topFitness) {
            speciesItr->topFitness = speciesItr->genomes.begin()->fitness;
            speciesItr->staleness = 0;
        } else
            speciesItr->staleness++;

        if (speciesItr->staleness < STALE_SPECIES || speciesItr->topFitness >= maxFitness)
            survivors.push_back(*speciesItr);
    }

    species.clear();
    species = survivors;
    //species.insert(species.begin(), survivors.begin(), survivors.end());    //Copy list B into list A
}

void Genus::removeWeakSpecies() {
    std::list<Species> survivors;

    double sum = Genus::totalAverageFitness();
    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        double breed = std::floor(speciesItr->averageFitness / sum * POPULATION);    // A/B * C or A/(B * C) ?
        if (breed >= 1.0)
            survivors.push_back(*speciesItr);
    }

    species.clear();
    species = survivors;
    //species.insert(species.begin(), survivors.begin(), survivors.end());
}

double Genus::totalAverageFitness() {
    double total = 0;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++)
        total += speciesItr->averageFitness;

    return total;
}