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
    bool initRun;
    int timeout;
    int savedGenomes;

    void setGenomeFitness ();
    int  getDistance (int x1, int x2, int y1, int y2);
    bool fitnessAlreadyMeasured ();
    void initialiseGenus ();
    void playBest ();
    void initialiseRun ();
    void evaluateCurrent ();
    void setUpController (bool* controls);

    void nextGenome();
    void saveBestGenome();
    void readInGenome();

    NEATDoop() : timeout(50), genusSetUp(false), initRun(false){ }
};
#endif //TESTBENCH_NEATDOOP_H
