//
// Created by gary on 08/01/17.
//

#ifndef TESTBENCH_NEATDOOP_H
#define TESTBENCH_NEATDOOP_H

#include "wl_def.h"

class NEATDoop {
private:
    int directions[4] = {
            sc_UpArrow, sc_RightArrow, sc_DownArrow, sc_LeftArrow};
    int buttons[NUMBUTTONS] = {
            sc_Control, sc_Alt, sc_LShift, sc_Space, sc_1, sc_2, sc_3, sc_4};

public:

};
#endif //TESTBENCH_NEATDOOP_H
