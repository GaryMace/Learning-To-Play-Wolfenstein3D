//
// Created by gary on 08/01/17.
//

#ifndef TESTBENCH_NEATDOOP_H
#define TESTBENCH_NEATDOOP_H

#include <vector>
#include "Def.h"

class NEATDoop {
public:
    bool genusSetUp;
    int timeout;
    int savedGenomes;

    void setGenomeFitness ();
    bool fitnessAlreadyMeasured ();
    void initialiseGenus ();
    void initialiseRun ();
    void evaluateCurrent ();
    void setUpController (bool* controls);

    void nextGenome();
    void saveBestGenome();
    void readInGenome();

    NEATDoop() : timeout(20), genusSetUp(false) { }
};
#endif //TESTBENCH_NEATDOOP_H