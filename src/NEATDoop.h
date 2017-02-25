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

    void setGenomeFitness ();
    void initialiseGenus ();
    void initialiseRun ();
    void clearControls ();
    void evaluateCurrent ();
    void setUpController (bool* controls);

    void nextGenome();

    NEATDoop() : timeout(20), genusSetUp(false) { }
};
#endif //TESTBENCH_NEATDOOP_H


//Check out line ~1145 of wl_agent.cpp for end of level pressing and shit