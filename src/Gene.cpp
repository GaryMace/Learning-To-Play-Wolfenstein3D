//
// Created by gary on 08/01/17.
//
#include "Gene.h"

Gene Gene::clone() {
    Gene copy;
    copy.enabled = this->enabled;
    copy.input = this->input;
    copy.output = this->output;
    copy.innovation = this->innovation;
    copy.weight = this->weight;

    return copy;
}

bool Gene::compare(const Gene &g1, const Gene &g2) {
    return g1.output < g2.output;
}
