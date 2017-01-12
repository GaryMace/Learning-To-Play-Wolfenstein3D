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

std::string Gene::backup() {
    std::string out = "\n\t\t\t\t\t\tGene{";

    out += "input=" + std::to_string(input) + ",";
    out += "\n\t\t\t\t\t\t\toutput=" + std::to_string(output) + ",";
    out += "\n\t\t\t\t\t\t\tenabled=" + std::to_string(enabled) + ",";
    out += "\n\t\t\t\t\t\t\tinnovation=" + std::to_string(innovation) + ",";
    out += "\n\t\t\t\t\t\t\tweight=" + std::to_string(weight) + "}";

    return out;
}

bool Gene::compare(const Gene &g1, const Gene &g2) {
    return g1.output < g2.output;
}
