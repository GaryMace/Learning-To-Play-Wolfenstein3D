//
// Created by gary on 02/01/17.
//
#include "Genus.h"
#include <algorithm>
#include <iostream>
#include <sstream>

// instantiate static members
int Genus::innovation = OUTPUTS;
int Genus::generation = 0;
double Genus::maxFitness = 0.0;
std::vector<Species> Genus::species;

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
    for (int i = 0; i < Genus::species.size(); i++) {
        Species species = Genus::species[i];
        if (i == Genus::species.size() - 1)
            out += species.backup() + "\n\t}";
        else
            out += species.backup() + ",";
    }

    return out + "\n}";
}

int Genus::newInnovation() {
    return ++innovation;
}

double Genus::nextDouble() {
    return ((double)rand() / RAND_MAX);
}

void Genus::addToSpecies(Genome child) {
    for (int i = 0; i < Genus::species.size(); i++) {
        Species species = Genus::species[i];
        if (child.sameSpecies(species.genomes[0])) {
            species.genomes.push_back(child);
            return;
        }
    }

    Species childSpecies;
    childSpecies.genomes.push_back(child);
    species.push_back(childSpecies);
}

void Genus::cullSpecies(bool cutToOne) {
    for (int i = 0; i < Genus::species.size(); i++) {
        Species& species = Genus::species[i];
        std::sort(species.genomes.begin(), species.genomes.end(), Genome::compare);  //sort based on fitness function

        double remaining  = std::ceil(species.genomes.size() / 2.0); //TODO: why does this need to be a double?
        if (cutToOne)
            remaining = 1.0;

        while (species.genomes.size() > remaining)
            species.genomes.erase(species.genomes.end());   //Remove weak species from end
    }
}

void Genus::initializeGenus() {
    for (int i = 0; i < POPULATION; i++) {  //initial population
        Genome basic;
        basic.maxNeuron = INPUTS;   //inputs is dynamic in our case ? maybe not..
        basic.mutate();             //TODO: figure out input size, if deterministic
        addToSpecies(basic);
    }
}

void Genus::newGeneration() {
    cullSpecies(false); //Cull bottom half of each species
    rankGlobally();
    removeStaleSpecies();
    rankGlobally();
    for (int i = 0; i < Genus::species.size(); i++) {
        Species& species = Genus::species[i];
        species.calculateAverageFitness();
    }
    removeWeakSpecies();

    double sum = totalAverageFitness();
    std::vector<Genome> children;
    for (int i = 0; i < Genus::species.size(); i++) {
        Species& species = Genus::species[i];
        double breed = std::floor(species.averageFitness / sum * POPULATION) - 1.0;
        for (int j = 0; j < breed; j++)
            children.push_back(species.breedChild());
    }

    cullSpecies(true);  //Cull all but the top member of each species
    while (children.size() + species.size() < POPULATION) {
        Species& species = Genus::species[rand() % Genus::species.size()];
        children.push_back(species.breedChild());
    }
    for (int i = 0; i < children.size(); i++) {
        Genome child = children[i];
        addToSpecies(child);
    }
    generation++;
    //initialiseRun() ????
}

void Genus::rankGlobally() {
    std::vector<Genome*> global;
    for (int i = 0; i < Genus::species.size(); i++) {
        Species& species = Genus::species[i];
        for (int j = 0; j < species.genomes.size(); j++) {
            Genome& genome = species.genomes[j];
            global.push_back(&genome);  //Push address to original Genomes
        }
    }

    //Sort by pointer, since we're directly editing original list later
    std::sort(global.begin(), global.end(), Genome::compareByPointer);

    for (int i = 0; i < global.size(); i++)
        global[i]->globalRank = i;  //Direct edits on original list
}

void Genus::removeStaleSpecies() {
    std::vector<Species> survivors;

    for (int i = 0; i < Genus::species.size(); i++) {
        Species species = Genus::species[i];
        std::sort(species.genomes.begin(), species.genomes.end(), Genome::compare);

        if (species.genomes[0].fitness > species.topFitness) {
            species.topFitness = species.genomes[0].fitness;
            species.staleness = 0;
        } else
            species.staleness++;

        if (species.staleness < STALE_SPECIES || species.topFitness >= maxFitness)
            survivors.push_back(species);
    }

    species.clear();
    species.insert(species.begin(), survivors.begin(), survivors.end());    //Copy list B into list A
}

void Genus::removeWeakSpecies() {
    std::vector<Species> survivors;

    double sum = totalAverageFitness();
    for (int i = 0; i < Genus::species.size(); i++) {
        Species species = Genus::species[i];
        double breed = std::floor(species.averageFitness / sum * POPULATION);    // A/B * C or A/(B * C) ?
        if (breed >= 1.0)
            survivors.push_back(species);
    }

    species.clear();
    species.insert(species.begin(), survivors.begin(), survivors.end());
}

double Genus::totalAverageFitness() {
    double total = 0;

    for (int i = 0; i < Genus::species.size(); i++) {
        Species species = Genus::species[i];
        total += species.averageFitness;
    }

    return total;
}