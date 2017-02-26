//
// Created by gary on 08/01/17.
//
#include "Neuron.h"
#include <cmath>
#include <sstream>


std::string Neuron::backup() {
    std::string out = "\n\t\t\t\t\t\t\tNeuron{";

    std::string str =
            static_cast<std::ostringstream*>(&(std::ostringstream() << value))->str();
    out += "value=" + str + ",";
    out += "\n\t\t\t\t\t\t\t\tinputs={";
    for (geneItr = inputs.begin(); geneItr != inputs.end(); geneItr++)
            out += geneItr->backup() + ",";
    out = out.substr(0, out.length() - 1) + "}";  //Remove last ","

    out += "}\n\t\t\t\t\t\t\t}\n";
    return out;
}

double Neuron::sigmoid(double x) {
    return 2.0 / (1.0 + std::exp(-4.9 * x)) - 1.0;
}

void Neuron::~Neuron() {
    delete value;
    delete inputs;
    delete geneItr;
}