//
// Created by gary on 02/01/17.
//
#include "Genus.h"
#include <algorithm>

// instantiate static members
int Genus::innovation = OUTPUTS;
int Genus::generation = 0;
double Genus::maxFitness = 0.0;
std::vector<Species> Genus::species;


int Genus::newInnovation() {
    return ++innovation;
}

double Genus::nextDouble() {
    return ((double)rand() / RAND_MAX);
}

void Genus::addToSpecies(Genome child) {
    for (Species species : Genus::species) {
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
    for (Species species : Genus::species) {
        sort(species.genomes.begin(), species.genomes.end(), Genome::compare);  //sort based on fitness function

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
    for (Species species : Genus::species)
        species.calculateAverageFitness();
    removeWeakSpecies();

    double sum = totalAverageFitness();
    std::vector<Genome> children;
    for (Species species : Genus::species) {
        double breed = std::floor(species.averageFitness / sum * POPULATION) - 1.0;
        for (int i = 0; i < breed; i++)
            children.push_back(species.breedChild());
    }

    cullSpecies(true);  //Cull all but the top member of each species
    while (children.size() + species.size() < POPULATION) {
        Species species = Genus::species[rand() % Genus::species.size()];
        children.push_back(species.breedChild());
    }
    for (Genome child : children)
        addToSpecies(child);
    generation++;
}

void Genus::rankGlobally() {
    std::vector<Genome> global;
    for (Species species : Genus::species)
        for (Genome genome : species.genomes)
            global.push_back(genome);

    std::sort(global.begin(), global.end(), Genome::compare);

    for (int i = 0; i < global.size(); i++)
        global[i].globalRank = i;
}

void Genus::removeStaleSpecies() {
    std::vector<Species> survivors;

    for (Species species : Genus::species) {
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
    for (Species species : Genus::species) {
        double breed = floor(species.averageFitness / sum * POPULATION);    // A/B * C or A/(B * C) ?
        if (breed >= 1.0)
            survivors.push_back(species);
    }

    species.clear();
    species.insert(species.begin(), survivors.begin(), survivors.end());
}

double Genus::totalAverageFitness() {
    double total = 0;

    for (Species species : Genus::species)
        total += species.averageFitness;

    return total;
}