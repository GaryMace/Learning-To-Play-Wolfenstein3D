//
// Created by gary on 26/01/17.
//
#include "NEATDoop.h"
#include "wl_def.h"
#include "Genome.h"
#include "Genus.h"
#include <iostream>
/*
=================================
=
= {'-'} initialiseGenus
=
= Set up the neural network given it hasn't been done yet already.
=
=================================
 */
void NEATDoop::initialiseGenus() {
    for (int i = 0; i < POPULATION; i++) {  //initial population
        Genome basic;
        basic.maxNeuron = TOTAL_INPUTS;     // See wl_def.h for a description on this constant.
        //std::cout << basic.backup()<< std::endl;
        basic.mutate();
        //std::cout << basic.backup() <<std::endl;
        //std::cout << Genus::species.size() << std::endl;
        Genus::addToSpecies(basic);
    }
    
    //initialiseRun();
}

/*
=================================
=
= {'-'} InitialiseRun
=
= Generate the input, output and hidden neurons for the current Genome of a Species
= and then determine what Doop does based on what he can see.
=
=================================
 */
void NEATDoop::initialiseRun() {
    timeout = TIMEOUT;
    int rightmist = 0;  //???
    clearControls();

    Species& species = *Genus::currSpeciesItr;
    Genome& genome = *Genus::currGenomeItr;
    genome.generateNetwork();
    evaluateCurrent();
}

/*
=================================
=
= {'-'} clearControls
=
= Reset the controller buttons so no accidental buttons are activate next frame.
=
=================================
 */
void NEATDoop::clearControls() {
    std::vector<bool> controls;
    for (int i = 0; i < OUTPUTS; i++)
        controls[i] = false;

    //set this equal to the buttonstate[] array later
    //TODO: figure out later the relevance of this
}

/*
=================================
=
= {'-'} evaluateCurrent
=
= Evaluate the current Genome of a Species with the inputs (what Doop can see at this point in time),
= receive what buttons the AI thinks should be pressed and pass these controls to the game.
=
=================================
 */
void NEATDoop::evaluateCurrent() {
    Species& species = *Genus::currSpeciesItr;;
    Genome& genome = *Genus::currGenomeItr;
    bool controls[] = genome.evaluateNetwork(inputs);

    setUpController(controls);
}

int mydirscan[4];   //TODO: remove and replace with dirscan
/*
=================================
=
= {'-'} setUpController
=
= Receives a bool vector from the output of the neural network. It then iterates through this vector
= and activates the appropriate controls for the actual game based on these bool values.
=
=================================
 */
void NEATDoop::setUpController(bool* controls) {
    for (int i = 0; i < OUTPUTS; i++) {
        if (controls[i]) {
            switch (i) {        //If an output val from network is true, switch on it set the appropriate control
                case FORWARD:
                    mydirscan[di_north] = true;
                    break;
                case BACK:
                    mydirscan[di_south] = true;
                    break;
                case TURN_LEFT:
                    mydirscan[di_west] = true;
                    break;
                case TURN_RIGHT:
                    mydirscan[di_east] = true;
                    break;                          // dirscan[bt_..] is for direction changing inputs
                case SHOOT:
                    buttonstate[bt_attack] = true;     //TODO: remove buttoons and replace with buttonstate[..]
                    break;
                case OPEN_DOOR:
                    buttonstate[bt_use] = true;
                    break;
                case WEAPON1:
                    buttonstate[bt_readyknife] = true;
                    break;
                case WEAPON2:
                    buttonstate[bt_readypistol] = true;
                    break;
                case WEAPON3:
                    buttonstate[bt_readymachinegun] = true;
                    break;
                case WEAPON4:
                    buttonstate[bt_readychaingun] = true;
                    break;
            }
        } else
            buttonstate[i] = false;    //all other controls we don't care about set to false, i.e. pausing the game
    }
}

/*
=================================
=
= {'-'} nextGenome
=
= Iterate through this species' genomes. If we come to the end of a species' genomes we go to the next
= species and start over.
=
=================================
 */
void NEATDoop::nextGenome() {
    Genus::currGenome++;
    Genus::currGenomeItr++;

    if (Genus::currGenomeItr == Genus::currSpeciesItr->genomes.end()) {
        Genus::currSpeciesItr++;
        if (Genus::currSpeciesItr == Genus::species.end()) {
            Genus::newGeneration();
            Genus::currSpeciesItr = NULL;
            Genus::currGenomeItr = NULL;
        } else
            Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();
    }

    /*if (Genus::currGenome >= Genus::species[Genus::currSpecies].genomes.size()) {    //TODO: >= or > ?
        Genus::currGenome = 0;
        Genus::currSpecies++;
        if (Genus::currSpecies >= Genus::species.size()) {
            Genus::newGeneration();
            Genus::currSpecies = 0;
        }
    }*/
}
