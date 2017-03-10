//
// Created by gary on 02/01/17.
//
#include "Genus.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <windows.h>

// instantiate static members
std::list<Species>::iterator Genus::currSpeciesItr;
std::list<Genome>::iterator Genus::currGenomeItr;
std::list<Species> Genus::species;
int Genus::innovation = OUTPUTS;
double Genus::maxFitness = 0.0;
int Genus::currSpecies = 0;
int Genus::generation = 0;
int Genus::currGenome = 0;
Genome Genus::best;

void Genus::loadSaveState(std::string saveState) {

}

std::string Genus::backup() {
    std::list<Species>::iterator speciesItr;
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
    std::list<Species>::iterator speciesItr;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        if (child.sameSpecies(*speciesItr->genomes.begin())) {  //child.sameSpecies(First node in list in curr species)
            speciesItr->genomes.push_back(child);
            return;
        }
    }
    Species childSpecies;
    childSpecies.genomes.push_back(child);
    childSpecies.topGenome = child;                             // First genome of each species is the top genome
    species.push_back(childSpecies);
}

void Genus::cullSpecies(bool cutToOne) {
    std::list<Species>::iterator speciesItr;
    std::list<Genome>::iterator removeItr;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        Species& species = *speciesItr;
        species.genomes.sort(Genome::compare);   //sort based on fitness

        double remaining  = std::ceil(species.genomes.size() / 2.0); //TODO: why does this need to be a double?
        if (cutToOne)
            remaining = 1.0;

        removeItr = species.genomes.end();
        while (species.genomes.size() > remaining || removeItr != species.genomes.begin()) {
            if (removeItr != species.genomes.end())
                species.genomes.erase(removeItr);   //Remove weak species from end
            removeItr--;
        }
    }
}

void Genus::newGeneration() {
    std::list<Species>::iterator speciesItr;
    
    Genus::cullSpecies(false); //Cull bottom half of each species
    std::cout << "start rank glob" << std::endl;
    Genus::rankGlobally();
    std::cout << "Emd rank" << std::endl;
    Genus::removeStaleSpecies();
    Genus::rankGlobally();
    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        Species& species = *speciesItr;
        species.calculateAverageFitness();
    }
    Genus::removeWeakSpecies();
    
    double sum = Genus::totalAverageFitness();
    if (sum == 0){ 
       sum = 1;
    }
    std::list<Genome> children;
    std::list<Genome>::iterator genomeItr;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        Species& species = *speciesItr;
        double breed = std::floor(species.averageFitness / sum * POPULATION) - 1;
        for (int j = 0; j < breed; j++)
            children.push_back(species.breedChild());
    }

    Genus::cullSpecies(true);  //Cull all but the top member of each species
    while (children.size() + species.size() < POPULATION) {
        int randSpecies = rand() % (int) species.size();
        Species *species1;

        for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
            if (randSpecies-- == 0) {
                species1 = &(*speciesItr);
                break;
            }
        }
        children.push_back(species1->breedChild());
    }
    for (genomeItr = children.begin(); genomeItr != children.end(); genomeItr++) {
        Genus::addToSpecies(*genomeItr);   
    }

    Genus::generation++;
}

void Genus::rankGlobally() {
    std::list<Species>::iterator speciesItr;
    std::list<Genome>::iterator genomeItr;
    std::list<Genome*> global;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++)
        for (genomeItr = speciesItr->genomes.begin(); genomeItr != speciesItr->genomes.end(); genomeItr++)
            global.push_back(&(*genomeItr));  //Push address to original Genomes

    global.sort(Genome::compareByPointer);

    int idx = 1;
    for (std::list<Genome*>::iterator globalItr = global.begin(); globalItr!= global.end(); globalItr++) {
        Genome &g = *(&**globalItr);
        g.globalRank = idx++;
    }
}

void Genus::removeStaleSpecies() {
    std::list<Species>::iterator speciesItr;
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
}

void Genus::removeWeakSpecies() {
    std::list<Species>::iterator speciesItr;
    std::list<Species> survivors;

    double sum = Genus::totalAverageFitness();
    if (sum == 0) 
       sum = 1;
    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        double breed = std::floor(speciesItr->averageFitness / sum * POPULATION);    // A/B * C or A/(B * C) ?
        if (breed >= 1.0)
            survivors.push_back(*speciesItr);
    }

    species.clear();
    species = survivors;
}

double Genus::totalAverageFitness() {
    std::list<Species>::iterator speciesItr;
    double total = 0;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++)
        total += speciesItr->averageFitness;

    return total;
}
