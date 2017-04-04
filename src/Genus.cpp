//
// Created by gary on 02/01/17.
//
#include "Genus.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <windows.h>

// Initialise static members
std::list<Species>::iterator Genus::currSpeciesItr;
std::list<Genome>::iterator Genus::currGenomeItr;
std::list<Species> Genus::species;
int Genus::innovation = OUTPUTS;
double Genus::maxFitness = 0.0;
int Genus::currSpecies = 0;
int Genus::generation = 0;
int Genus::currGenome = 0;

/*
=================================
=
= {'-'} Genus::backup
=
= Outputs a json inspired string representation of the population.
=
=================================
 */
std::string Genus::backup() {
    std::list<Species>::iterator speciesItr;
    int i = 0;
    std::string out = "Genus{";
    std::string str;

    str = static_cast<std::ostringstream*>(&(std::ostringstream() << generation))->str();
    out += "generation=" + str + ",\n";
    str = static_cast<std::ostringstream*>(&(std::ostringstream() << maxFitness))->str();
    out += "\tmaxFitness=" + str + ",\n";

    out += "\tspecies={";
    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        if (i == Genus::species.size() - 1)
            out += speciesItr->backup() + "\n\t}";
        out += speciesItr->backup() + ",";
        i++;
    }
        
    out = out.substr(0, out.length() - 1);  //Remove last ","
    out += "\n\t}";

    return out + "\n}";
}

/*
=================================
=
= {'-'} Genus::newInnovation
=
= Used to uniquely label every Gene in the entire population
=
=================================
 */
int Genus::newInnovation() {
    return ++Genus::innovation;
}

/*
=================================
=
= {'-'} Genus::nextDouble
=
= Returns a double in range 0 and 1
=
=================================
 */
double Genus::nextDouble() {
    return ((double)rand() / RAND_MAX);
}

/*
=================================
=
= {'-'} Genus::addToSpecies
=
= Adds the given Genome to a Species whose members networks are similar to the given one. If no such Species exists 
= it will create a new Species and add the Genome to it.
=
=================================
 */
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

/*
=================================
=
= {'-'} Genus::cullSpecies
=
= This function will either reduce the size of Species to half its previous size or reduce the size to just one.
=
=================================
 */
void Genus::cullSpecies(bool cutToOne) {
    std::list<Species>::iterator speciesItr;
    std::list<Genome>::iterator survivedItr;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        std::list<Genome> survived;
        Species& species = *speciesItr;
        species.genomes.sort(Genome::compare);                          //sort based on fitness

        double remaining  = std::ceil(species.genomes.size() / 2.0);   
        if (cutToOne)
            remaining = 1.0;

        survivedItr = species.genomes.begin();
        while (survived.size() < remaining && survivedItr != species.genomes.end()) {
            survived.push_back(*survivedItr);
            survivedItr++;

        }

        species.genomes.clear();
        species.genomes = survived;
    }
}

/*
=================================
=
= {'-'} Genus::newGeneration
=
= Creates a new generation of individuals once all members of the current population have been evaluated.
=
=================================
 */
void Genus::newGeneration() {
    std::list<Species>::iterator speciesItr;
    Genus::cullSpecies(false);                                                              //Cull bottom half of each species
    Genus::rankGlobally();
    Genus::removeStaleSpecies();
    Genus::rankGlobally();
    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        Species& species = *speciesItr;
        species.calculateAverageFitness();                                                  //Total of global rank / genome size
    }
    Genus::removeWeakSpecies();
    double sum = Genus::totalAverageFitness();                                              //All average fitnesses added
    if (sum == 0)                                                                           //Prevents div by 0 error
       sum = 1;

    std::list<Genome> children;
    std::list<Genome>::iterator genomeItr;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        Species& species = *speciesItr;
        double breed = std::floor(species.averageFitness / sum * POPULATION) - 1;
        for (int j = 0; j < breed; j++)
            children.push_back(species.breedChild());
    }
    Genus::cullSpecies(true);                                                               //Cull all but the top member of each species
    while (children.size() + species.size() < POPULATION) {
        int randSpecies = rand() % (int) species.size();
        Species *species1;
        
        for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {     //Get the random Species from population
            if (randSpecies-- == 0) {
                species1 = &(*speciesItr);
                break;
            }
        }
        children.push_back(species1->breedChild());                                         //Generate a child from the selected species and bring it forward to next generation
    }
    for (genomeItr = children.begin(); genomeItr != children.end(); genomeItr++) {          //Add the new members to the population
        Genus::addToSpecies(*genomeItr);   
    }

    Genus::generation++;
}

/*
=================================
=
= {'-'} Genus::rankGlobally
=
= Rank the Genomes of every species accoring to their fitnesses. The best performer in the entire population 
= gets a global rank of 0
=
=================================
 */
void Genus::rankGlobally() {
    std::list<Species>::iterator speciesItr;
    std::list<Genome>::iterator genomeItr;
    std::list<Genome*> global;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++)
        for (genomeItr = speciesItr->genomes.begin(); genomeItr != speciesItr->genomes.end(); genomeItr++)
            global.push_back(&(*genomeItr));        //Push address to original Genomes

    global.sort(Genome::compareByPointer);

    int idx = 1;
    for (std::list<Genome*>::iterator globalItr = global.begin(); globalItr!= global.end(); globalItr++) {
        Genome &g = *(&**globalItr);
        g.globalRank = idx++;
    }
}

/*
=================================
=
= {'-'} Genus::removeStaleSpecies
=
= If either a species is too stale (the new members of the current generation are worse at playing Wolfenstein than the current
= best performer) then give it the chop.
=
=================================
 */
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
    Genus::species.clear();
    Genus::species = survivors;
}

/*
=================================
=
= {'-'} Genus::removeWeakSpecies
=
= If a Species is considered too weak. meaning the sum of its members fitnesses is miniscule, 
= then give it the chop
=
=================================
 */
void Genus::removeWeakSpecies() {
    std::list<Species>::iterator speciesItr;
    std::list<Species> survivors;

    double sum = Genus::totalAverageFitness();
    if (sum == 0) 
       sum = 1;
    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++) {
        double breed = std::floor(speciesItr->averageFitness / sum * POPULATION);
        if (breed >= 1.0)
            survivors.push_back(*speciesItr);
    }

    Genus::species.clear();
    Genus::species = survivors;
}

/*
=================================
=
= {'-'} Genus::totalAverageFitness
=
= Returns the sum of all species average fitnesses.
=
=================================
 */
double Genus::totalAverageFitness() {
    std::list<Species>::iterator speciesItr;
    double total = 0;

    for (speciesItr = species.begin(); speciesItr != species.end(); speciesItr++)
        total += speciesItr->averageFitness;

    return total;
}
