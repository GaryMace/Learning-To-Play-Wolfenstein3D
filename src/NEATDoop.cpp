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
    Genus::currSpeciesItr = Genus::species.begin();
    Genus::currGenomeItr = Genus::species.begin()->genomes.begin();
    std::cout << "start run" << std::endl;
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

    for (bool *freeMem = &controls[0]; freeMem < &controls[9]; freeMem++)   //Free the memory that was allocated
        delete freeMem;

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
            std::cout << "New Generation: " << std::endl;
            Genus::newGeneration();
            Genus::currSpeciesItr = Genus::species.begin();
            Genus::currGenomeItr = Genus::species.begin()->genomes.begin();
        } else {
            std::cout << "Next genome" << std::endl;
            Genus::currGenomeItr = Genus::currSpeciesItr->genomes.begin();
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
