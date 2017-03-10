//
// Created by gary on 08/01/17.
//
#include <sstream>
#include "Gene.h"


Gene Gene::clone() {
    Gene copy;
    copy.enabled = enabled;
    copy.input = input;
    copy.output = output;
    copy.innovation = innovation;
    copy.weight = weight;

    return copy;
}

std::string Gene::backup() {
    std::string out = "\n\t\t\t\t\t\tGene{";
    std::string str =
            static_cast<std::ostringstream*>(&(std::ostringstream() << input))->str();
    out += "input=" + str + ",";
    str =
            static_cast<std::ostringstream*>(&(std::ostringstream() << output))->str();
    out += "\n\t\t\t\t\t\t\toutput=" + str + ",";
    str =
            static_cast<std::ostringstream*>(&(std::ostringstream() << enabled))->str();
    out += "\n\t\t\t\t\t\t\tenabled=" + str + ",";
    str =
            static_cast<std::ostringstream*>(&(std::ostringstream() << innovation))->str();
    out += "\n\t\t\t\t\t\t\tinnovation=" + str + ",";
    str =
            static_cast<std::ostringstream*>(&(std::ostringstream() << weight))->str();
    out += "\n\t\t\t\t\t\t\tweight=" + str + "}";

    return out;
}

std::string Gene::backupnew() {
            std::string out = static_cast<std::ostringstream*>(&(std::ostringstream() << input))->str() + ",";
            out += static_cast<std::ostringstream*>(&(std::ostringstream() << output))->str() + ",";
            out += static_cast<std::ostringstream*>(&(std::ostringstream() << enabled))->str() + ",";
            out += static_cast<std::ostringstream*>(&(std::ostringstream() << innovation))->str() + ",";
            out += static_cast<std::ostringstream*>(&(std::ostringstream() << weight))->str();
    return out;
}

bool Gene::compare(const Gene &g1, const Gene &g2) {
    return g1.output < g2.output;
}
