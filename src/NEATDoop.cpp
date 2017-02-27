//
// Created by gary on 26/01/17.
//
#include "NEATDoop.h"
#include "wl_def.h"
#include "Genome.h"
#include "Genus.h"
#include <iostream>
#include <cmath>

/*
=================================
=
= {'-'} setGenomeFitness
=
= Called once a genomes' network has finished playing the game. This function gives a score to the Genome
= based on game statistics with the following weightings.
=
= (Low)     Distance from level end
= (Low)     Distance from spawn with respect to time taken
= (Medium)  Number enemies killed on map
= (Medium)  Number unique doors opened
= (High)    Accuracy
= (High)    Number of static item pickups
= (V.High)  Level finished or no?
=
=================================
*/
void NEATDoop::setGenomeFitness() {
    Genome* genome = &(*Genus::currGenomeItr);
    if ((int)player->tilex == spawnxp && (int)player->tiley == spawnyp) {
        genome->fitness = 0;
        return;
    }

    double distFromEnd = MAP_DISTANCE((int)player->tilex, endxp, (int)player->tiley, endyp);
    double distFromSpawn = std::sqrt(std::pow(spawnxp - (int)player->tilex, 2.0) + std::pow(spawnxp - (int)player->tiley, 2.0));
    int sec = gamestate.TimeCount / 70;
    int kills = gamestate.killcount;

    genome->fitness = MAX_DISTANCE - distFromEnd;
    genome->fitness += kills * KILL_REWARD;
    genome->fitness += (distFromSpawn / sec) * TRAVEL_REWARD;
}
/*
 *
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
    Genus::currGenome = 0;
    Genus::currSpecies = 0;
    Genus::currSpeciesItr = Genus::species.begin();
    Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();
    //std::cout << "start run" << std::endl;
    initialiseRun();
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
    //timeout = TIMEOUT;
    int rightmist = 0;  //???
    //clearControls();  //Don't think I need this, doesn't Wolfenstein does it for us?

    Genome* genome = &(*Genus::currGenomeItr);
    genome->generateNetwork();
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
    //TODO: turns out probs dont need it....maybe?
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
    Genome* genome = &(*Genus::currGenomeItr);
    bool *controls = genome->evaluateNetwork(inputs);
    //std::cout << genome->backup() << std::endl;
    //Sleep(5000);

    //for (bool *i = &controls[0]; i != &controls[9]; i++)
    //    std::cout << "Val: " << (int) *i << std::endl;

    //Sleep(1000);
    setUpController(controls);
}

/*
=================================
=
= {'-'} setUpController
=
= Receives a bool pointer to a boolean array from the output of the neural network. It then iterates through memory
= space of the array and activates the appropriate controls for the actual game based on these bool values.
=
=================================
 */
void NEATDoop::setUpController(bool* controls) {
    int i;
    bool *control;

    for (control = &controls[0], i = 0; control != &controls[9]; control++, i++) {
        if ((int) *control) {   //If network says push a button
            switch (i) {        //Find what button to press
                case FORWARD:
                    Keyboard[dirscan[di_north]] = true;  // dirscan[bt_..] is an index into Keyboard arr
                    break;
                case BACK:
                    Keyboard[dirscan[di_south]] = true;
                    break;
                case TURN_LEFT:
                    Keyboard[dirscan[di_west]] = true;
                    break;
                case TURN_RIGHT:
                    Keyboard[dirscan[di_east]] = true;
                    break;
                case SHOOT:
                    buttonstate[bt_attack] = true;
                    break;
                case USE_ACTION:
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
                default:
                    break;
            }
        } else
            buttonstate[i] = false;    //all other controls we don't care about set to false, i.e. pausing the game
    }

    for (bool *usedMem = &controls[0]; usedMem < &controls[9]; usedMem++)   //Free the memory that was allocated
        delete usedMem;

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
        //std::cout << "New Species" << std::endl;
        Genus::currSpeciesItr++;
        Genus::currSpecies++;
        if (Genus::currSpeciesItr == Genus::species.end()) {
            std::cout << "New Generation: " << std::endl;
            Genus::newGeneration();
            Genus::currSpecies = 0;
            Genus::currGenome = 0;
            Genus::currSpeciesItr = Genus::species.begin();
            Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();
        } else {
            //std::cout << "Next Genome" << std::endl;
            Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();
            Genus::currGenome = 0;
        }

        //std::cout << Genus::currGenomeItr->backup() << std::endl;
        //Sleep(5000);
        // timeout = TIMEOUT;
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
