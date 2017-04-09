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
= {'-'} NEATDoop::setGenomeFitness
=
= Called once a genomes' network has finished playing the game. This function gives a score to the Genome
= based on game statistics with the following weightings.
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

    genome->fitness = (MAX_DISTANCE - distFromEnd) * TRAVEL_REWARD;
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

/*
=================================
=
= {'-'} NEATDoop::getDistance
=
= Get the Manhattan distance between two points
=
=================================
 */
int NEATDoop::getDistance(int x1, int x2, int y1, int y2) {
    int dist = 0;
    int xDist = std::abs(x2 - x1) - 1;
    
    dist = std::abs(y2 - y1);
    if (xDist > 0) 
       dist += xDist;
       
    return dist;   
}

/*
=================================
=
= {'-'} NEATDoop::fitnessAlreadyMeasured
=
= Check if curr Genome was already evaluated
=
=================================
 */
bool NEATDoop::fitnessAlreadyMeasured() {
    Species& species = *Genus::currSpeciesItr;
    Genome& genome = *Genus::currGenomeItr;

    return genome.fitness != 0;
}

/*
=================================
=
= {'-'} NEATDoop::initialiseGenus
=
= Set up the neural network given it hasn't been done yet already.
=
=================================
 */
void NEATDoop::initialiseGenus() {
    srand(time(NULL));                      //Random seed, debugging would take the same seed to reproduce bugs

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
= {'-'} NEATDoop::playBest
=
= Only adds a single Genome to the population, which was in a file the user passed
= as input when launching the game.
=
=================================
 */
void NEATDoop::playBest() {
    Genus::addToSpecies(Genus::best);       //Add the best Genome (from the input file)                                 
    
    Genus::currGenome = 0;
    Genus::currSpecies = 0;
    Genus::currSpeciesItr = Genus::species.begin();
    Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();

    initialiseRun();
}

/*
=================================
=
= {'-'} NEATDoop::InitialiseRun
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

    evaluateCurrent();
    initRun = false;
}

/*
=================================
=
= {'-'} NEATDoop::evaluateCurrent
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
= {'-'} NEATDoop::setUpController
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
        if ((int) *control) {                                               //If network says push a button
            switch (i) {                                                    //Find what button to press
                case FORWARD:
                    Keyboard[dirscan[di_north]] = true;                     //dirscan[di_..] is an index into Keyboard arr
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
                case USE_ACTION:                                            //Opens doors
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
            buttonstate[i] = false;                                         //All controls not pressed set to false
    }
    if (Keyboard[dirscan[di_west]] && Keyboard[dirscan[di_east]]) {         //Don't let it turn left and right at the same time
        Keyboard[dirscan[di_west]] = false;
        Keyboard[dirscan[di_east]] = false;
    }

    if (Keyboard[dirscan[di_north]] &&                                      //If the AI is pressing buttons that will cause it to run in a circle, start a circle time out counter
            (Keyboard[dirscan[di_east]] || Keyboard[dirscan[di_west]])) {
            if (!circletimeoutset) {
                circletimeoutset = true;
            }   
    } else {                                                                //Stop the circle time out counter, the AI isn't pressing buttons that cause circles anymore
        circletimeoutset = false;
        timeouttics = 0;
    }
    
    if (circletimeoutset && timeouttics >= 500)
       killattempt = true;

    delete[] controls;                                                      //Free the memory that was allocated
}

/*
=================================
=
= {'-'} NEATDoop::nextGenome
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
        Genus::currSpecies++;

        if (Genus::currSpeciesItr == Genus::species.end()) {
            std::cout << "\nGen: " << Genus::generation << std::endl;
            saveBestGenome();
            Genus::newGeneration();

            Genus::currSpecies = 0;
            Genus::currSpeciesItr = Genus::species.begin();
        }
        Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();
        Genus::currGenome = 0;
    }
}

/*
=================================
=
= {'-'} NEATDoop::saveBestGenome
=
= Find the best Genome in population and save it to the stdout.txt file.. 
=
=================================
 */
void NEATDoop::saveBestGenome() {
    std::list<Genome> population;
    std::list<Species>::iterator speciesItr;
    std::list<Genome>::iterator genomeItr;

    int i = 0;
    Genome best;
    Genome worst;
    worst.fitness = 50000;
    best.fitness = -2;

    for (speciesItr = Genus::species.begin(); speciesItr != Genus::species.end(); speciesItr++) {
        if (speciesItr->topGenome.fitness > best.fitness) {
            best = speciesItr->topGenome;
        }

        for (genomeItr = speciesItr->genomes.begin(); genomeItr != speciesItr->genomes.end(); genomeItr++) {
            if (genomeItr->fitness < worst.fitness) {
                worst = *genomeItr;
            }
            population.push_back(*genomeItr);
        }
    }

    std::cout << "Best from Gen" << std::endl;
    std::cout << best.encode() << std::endl;

    std::cout << "Median from Gen" << std::endl;
    population.sort(Genome::compare);
    genomeItr = population.begin();
    int size = (int) population.size();
    int counter = 0;
    while (counter++ < size / 2) {
        genomeItr++;
    }
    std::cout << genomeItr->encode() << std::endl;

    std::cout << "Worst from Gen" << std::endl;
    std::cout << worst.encode() << std::endl;
}

/*
=================================
=
= {'-'} NEATDoop::split
=
= Code taken from the below link. This is not my code for splitting a string..
= http://stackoverflow.com/questions/236129/split-a-string-in-c 
=
=================================
 */
template<typename Out> void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::list<std::string> split(const std::string &s, char delim) {
    std::list<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

/*
=================================
=
= {'-'} NEATDoop::readInGenome
=
= Ask for filename. Input wouldn't clear properly when I tried to let the user re-enter a filename
= causing the file to be opened but the contents to be loaded incorrectly.
= The solution used is to just exit.. Be Grand
=
=================================
 */
void NEATDoop::readInGenome() {
    std::ifstream input;
    std::string filename;

    std::cout << "Enter the file name: ";
    std::cin >> filename;

    playbest = true;
    input.open(filename.c_str());
    
    while(!input.is_open()) {
        input.clear();
        std::cin.clear();

        std::cout << "Incorrect filename, please relaunch and try again. This window will now close...";
        Sleep(3000);
        exit(1);
    }

    std::string line;
    int idx = 0;
    
    input >> Genus::best.fitness;                                           //read known content in
    input >> Genus::best.globalRank;
    input >> Genus::best.maxNeuron;
    input >> line;

    std::list<std::string> mutationRates = split(line, ',');
    for (std::list<std::string>::iterator it = mutationRates.begin(); it != mutationRates.end(); it++)
        Genus::best.mutationRates[idx++] = std::atof(it->c_str());

    Gene newGene;
    std::list<Gene> genes;      
    while (input >> line) {                                                 //Keep reading in Genes while they exist
        if (!line.empty()) {
            std::list<std::string> geneLine = split(line, ',');
            std::list<std::string>::iterator it = geneLine.begin();

            newGene.input = (int) std::atof((it++)->c_str());
            newGene.output = (int) std::atof((it++)->c_str());
            newGene.enabled = (bool) std::atof((it++)->c_str());
            newGene.innovation = (int) std::atof((it++)->c_str());
            newGene.weight = std::atof((it)->c_str());
            genes.push_back(newGene);
        }
    }
    Genus::best.addGenesToSelf(genes);                                      //Add the genes read in to the best Genome
}