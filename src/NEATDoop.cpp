//
// Created by gary on 26/01/17.
//
#include "NEATDoop.h"
#include "Genome.h"
#include "Genus.h"

void NEATDoop::initialiseGenus() {
    for (int i = 0; i < POPULATION; i++) {  //initial population
        Genome basic;
        basic.maxNeuron = TOTAL_INPUTS;
        basic.mutate();             //TODO: figure out input size, if deterministic
        Genus::addToSpecies(basic);
    }

    initialiseRun();
}

void NEATDoop::initialiseRun() {
    int rightmist = 0;  //???
    int timeout = TIMEOUT;
    clearControls();

    Species& species = Genus::species[Genus::currSpecies];
    Genome& genome = species.genomes[Genus::currGenome];
    genome.generateNetwork();
    evaluateCurrent();
}

void NEATDoop::clearControls() {
    std::vector<bool> controls;
    for (int i = 0; i < OUTPUTS; i++)
        controls[i] = false;

    //set this equal to the buttonstate[] array later
    //TODO: figure out later the relevance of this
}

void NEATDoop::evaluateCurrent() {
    Species& species = Genus::species[Genus::currSpecies];
    Genome& genome = species.genomes[Genus::currGenome];
    std::vector<bool> controls = genome.evaluateNetwork(inputs);

    setUpController(controls);

    if (!genusSetUp) {
        initialiseGenus();
        genusSetUp = true;
    }
}

void NEATDoop::setUpController(std::vector<bool> controls) {
    for (int i = 0; i < controls.size(); i++) {
        if (controls[i]) {
            switch (i) {
                case FORWARD:
                    buttoons[bt_run] = true;    //TODO: I think I need to set dirscan[dir_east] and such in here too!...seeing as i'm simulating keyboard pressing
                    break;
                case BACK:
                    buttoons[bt_run] = true;    //TODO: refactor this!
                    break;
                case TURN_LEFT:
                    buttoons[bt_strafeleft] = true;
                    break;
                case TURN_RIGHT:
                    buttoons[bt_straferight] = true;
                    break;
                case SHOOT:
                    buttoons[bt_attack] = true;
                    break;
                case OPEN_DOOR:
                    buttoons[bt_use] = true;
                    break;
                case WEAPON1:
                    buttoons[bt_readyknife] = true;
                    break;
                case WEAPON2:
                    buttoons[bt_readypistol] = true;
                    break;
                case WEAPON3:
                    buttoons[bt_readymachinegun] = true;
                    break;
                case WEAPON4:
                    buttoons[bt_readychaingun] = true;
                    break;
            }
        } else {
            buttoons[i] = false;
        }
    }
}

void NEATDoop::nextGenome() {
    Genus::currGenome++;

    if (Genus::currGenome >= Genus::species[Genus::currSpecies].genomes.size()) {    //TODO: >= or > ?
        Genus::currGenome = 0;
        Genus::currSpecies++;
        if (Genus::currSpecies >= Genus::species.size()) {
            Genus::newGeneration();
            Genus::currSpecies = 0;
        }
    }
}