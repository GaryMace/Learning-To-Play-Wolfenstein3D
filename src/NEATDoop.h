//
// Created by gary on 08/01/17.
//

#ifndef TESTBENCH_NEATDOOP_H
#define TESTBENCH_NEATDOOP_H

#include <vector>

class NEATDoop {
public:
    bool genusSetUp;
    int timeout;

    void initialiseGenus ();
    void initialiseRun ();
    void clearControls ();
    void evaluateCurrent ();
    void setUpController (std::vector<bool> controls);

    void nextGenome();
};
#endif //TESTBENCH_NEATDOOP_H
