//
// Created by gary on 26/01/17.
//
#include "NEATDoop.h"
#include "wl_def.h"
#include "Genome.h"
#include "Genus.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
/*
=================================
=
= {'-'} setGenomeFitness
=
= Called once a genomes' network has finished playing the game. This function gives a score to the Genome
= based on game statistics with the following weightings.
=
= (Low)     Distance from level end                             DONE
= (Low)     Distance from spawn with respect to time taken      DONE
= (Medium)  Number enemies killed on map                        DONE
= (Medium)  Number unique doors opened                          DONE
= (High)    Accuracy
= (High)    Number of static item pickups                       DONE
= (V.High)  Level finished or no?                               DONE
=
=================================
*/
void NEATDoop::setGenomeFitness() {
    Species* species = &(*Genus::currSpeciesItr);
    Genome* genome = &(*Genus::currGenomeItr);

    if ((int)player->tilex == spawnxp && (int)player->tiley == spawnyp) {
        genome->fitness = -1;
        return;
    }

    double distFromEnd = getDistance((int) player->tilex, endxp, (int) player->tiley, endyp);
    double distFromSpawn = getDistance((int) player->tilex, spawnxp, (int) player->tiley, spawnyp);
    int sec = gamestate.TimeCount / 70;                                                                 //70 fps so divide by 70, counter intuitive I know
    int kills = gamestate.killcount;

    genome->fitness = MAX_DISTANCE - distFromEnd;
    if (sec != 0 && distFromSpawn != 0)
       genome->fitness += (distFromSpawn / sec) * TRAVEL_REWARD;

    if (kills > 0)
        genome->fitness += kills * KILL_REWARD;
    if (doorsopened > 0)
        genome->fitness += doorsopened * DOOR_OPENED_REWARD;
    if (pickups > 0)
        genome->fitness += pickups * ITEM_PICKUP_REWARD;
    //std::cout << static_cast<std::ostringstream*>(&(std::ostringstream() << genome->fitness))->str() << std::endl;
    if (leveldone)
        genome->fitness += LVL_DONE_REWARD;
    
    if (genome->fitness > Genus::maxFitness)
        Genus::maxFitness = genome->fitness;
    //if (genome->fitness > species->topGenome.fitness)   //this is messed stuff up i think
    //    species->topGenome = *genome;
    if (genome->fitness == 0)
        genome->fitness = -1;
}

int NEATDoop::getDistance(int x1, int x2, int y1, int y2) {
    int dist = 0;
    int xDist = std::abs(x2 - x1) - 1;
    
    dist = std::abs(y2 - y1);
    if (xDist > 0) 
       dist += xDist;
       
    return dist;   
}

bool NEATDoop::fitnessAlreadyMeasured() {
    Species& species = *Genus::currSpeciesItr;
    Genome& genome = *Genus::currGenomeItr;

    return genome.fitness != 0;
}

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
    srand(time(NULL));  //TODO: is this necessary?
     
    for (int i = 0; i < POPULATION; i++) {  //initial population
        Genome basic;
        //basic.maxNeuron = TOTAL_INPUTS;     // See wl_def.h for a description on this constant.
        basic.mutate();
        Genus::addToSpecies(basic);
    }
    Genus::currGenome = 0;
    Genus::currSpecies = 0;
    Genus::currSpeciesItr = Genus::species.begin();
    Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();

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
    int rightmost = 0;  //???
    frames = 0;

    Genome* genome = &(*Genus::currGenomeItr);
    genome->generateNetwork();
    if (!initRun)
       evaluateCurrent();
    initRun = false;
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
                    Keyboard[dirscan[di_south]] = false;
                    break;
                case TURN_LEFT:
                    if (!controls[TURN_RIGHT])
                        Keyboard[dirscan[di_west]] = true;
                    else
                        Keyboard[dirscan[di_west]] = false;
                    break;
                case TURN_RIGHT:
                    if (!controls[TURN_LEFT])
                        Keyboard[dirscan[di_east]] = true;
                    else
                        Keyboard[dirscan[di_east]] = false;
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

    if (controls[FORWARD] &&
            (controls[TURN_LEFT] || controls[TURN_RIGHT])) {
        if (circletimeoutset) {
            if (timeouttics >= 100)
                killattempt = true;
        } else {
            circletimeoutset = true;
        }
    }

    //delete[] controls;   //Free the memory that was allocated
    for (bool *usedMem = &controls[0]; usedMem < &controls[9]; usedMem++)
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
            //std::cout << "New Generation: " << std::endl;
            //saveBestGenome();
            Genus::newGeneration();
            Genus::currSpecies = 0;
            Genus::currSpeciesItr = Genus::species.begin();
        }
        Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();
        Genus::currGenome = 0;
    }
}

void NEATDoop::saveBestGenome() {
    std::list<Species>::iterator speciesItr;
    /*char buffer[32];
    snprintf(buffer, sizeof(char) * 32, "gen_%i.txt", Genus::generation);

    std::filebuf fb;
    fb.open(buffer, std::ios::out);
    std::ostream genomefile(&fb);*/

    Genome best;
    best.fitness = -2;  //TODO: consider refactoring if penalties added to scoring func
    for (speciesItr = Genus::species.begin(); speciesItr != Genus::species.end(); speciesItr++) {
        if (speciesItr->topGenome.fitness > best.fitness)
            best = speciesItr->topGenome;
    }
    //genomefile << best.backup();
    //std::cout << best.backup() << std::endl;
    //fb.close();
}

void NEATDoop::readInGenome() {

}
