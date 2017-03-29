//
// Created by gary on 26/01/17.
//
#include "NEATDoop.h"
#include "wl_def.h"
#include "Genome.h"
#include "Genus.h"

#include <iostream>
#include <sstream>
#include <windows.h>
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
    int kills = gamestate.killcount;

    genome->fitness = (MAX_DISTANCE - distFromEnd) * 10;
    genome->fitness += distFromSpawn * distFromSpawn * TRAVEL_REWARD;
    genome->fitness += pickups * ITEM_PICKUP_REWARD;
    genome->fitness += accuracy * ACCURACY_REWARD;
    genome->fitness += kills * KILL_REWARD;
    genome->fitness += inputsused;

    if (leveldone)
        genome->fitness += LVL_DONE_REWARD;

    if (genome->fitness == 0)
        genome->fitness = -1;

    if (killattempt) {
        if (genome->fitness == Genus::maxFitness) {
            Genus::maxFitness -= 500;
        }
        genome->fitness -= 500;
    }

    if (genome->fitness > Genus::maxFitness)
        Genus::maxFitness = genome->fitness;

    if (genome->fitness > species->topGenome.fitness)
        species->topGenome = *genome;
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
    int timer = time(NULL);
    std::cout << timer << std::endl;
    srand(1490349215);  //1488900454 or 1489693580

    for (int i = 0; i < POPULATION; i++) {  //initial population
        Genome basic;
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
    frames = 0;

    Genome* genome = &(*Genus::currGenomeItr);
    genome->generateNetwork();

    //if (!initRun)
       evaluateCurrent();
    initRun = false;
}

/*
=================================
=
= {'-'} evaluateCurrent
=
= Evaluate the current Genome of a Species with the gameinputs (what Doop can see at this point in time),
= receive what buttons the AI thinks should be pressed and pass these controls to the game.
=
=================================
 */
void NEATDoop::evaluateCurrent() {
    Genome* genome = &(*Genus::currGenomeItr);

    bool *controls = genome->evaluateNetwork(gameinputs);
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

    for (control = &controls[0], i = 0; control != &controls[8]; control++, i++) {
        if ((int) *control) {   //If network says push a button
            switch (i) {        //Find what button to press
                case FORWARD:
                    Keyboard[dirscan[di_north]] = true;  // dirscan[di_..] is an index into Keyboard arr
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
            buttonstate[i] = false;                                 //all controls not pressed set to false
    }
    if (Keyboard[dirscan[di_west]] && Keyboard[dirscan[di_east]]) { //dont let it turn left and right at the same time
        Keyboard[dirscan[di_west]] = false;
        Keyboard[dirscan[di_east]] = false;
    }

    if (Keyboard[dirscan[di_north]] &&
            (Keyboard[dirscan[di_east]] || Keyboard[dirscan[di_west]])) {
            if (!circletimeoutset) {
                circletimeoutset = true;
            }
    } else {
        circletimeoutset = false;
        timeouttics = 0;
    }
    
    if (circletimeoutset && timeouttics >= 500)
       killattempt = true;

    delete[] controls;   //Free the memory that was allocated
}

void printGenus() {
    std::list<Species>::iterator spec;

    std::cout << "Num Spec: " << Genus::species.size() << std::endl;
    for (spec = Genus::species.begin(); spec != Genus::species.end(); spec++) {
        std::cout << "Num genoms: " << spec->genomes.size() << std::endl;
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
        //std::cout << "New Species" << std::endl;
        Genus::currSpeciesItr++;
        Genus::currSpecies++;

        if (Genus::currSpeciesItr == Genus::species.end()) {
            //if (Genus::generation % 10 == 0) {
            std::cout << "Save best geome" << std::endl;
            saveBestGenome();
            std::cout << "done save best geome" << std::endl;
            //}
            std::cout << "Gen: " << Genus::generation << std::endl;
            Genus::newGeneration();
            std::cout << "New Gen done" << std::endl;
            Genus::currSpecies = 0;
            Genus::currSpeciesItr = Genus::species.begin();

            //printGenus();
        }
        Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();
        Genus::currGenome = 0;
    }
}

void NEATDoop::saveBestGenome() {
    std::list<Species>::iterator speciesItr;
    int i = 0;
    /*char buffer[32];
    snprintf(buffer, sizeof(char) * 32, "gen_%i.txt", Genus::generation);

    std::filebuf fb;
    fb.open(buffer, std::ios::out);
    std::ostream genomefile(&fb);*/

    Genome best;
    best.fitness = -2;
    for (speciesItr = Genus::species.begin(); speciesItr != Genus::species.end(); speciesItr++) {
        if (speciesItr->topGenome.fitness > best.fitness) {
            best = speciesItr->topGenome;
        }
    }
    //genomefile << best.backup();
    std::cout << best.backupnew() << std::endl;
    //fb.close();
}

void NEATDoop::readInGenome() {

}
