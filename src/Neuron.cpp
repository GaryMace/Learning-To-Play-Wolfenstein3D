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
    for (int i = 0; i < inputs.size(); i++) {
        Gene gene = inputs[i];
        if (i == inputs.size() - 1)
            out += gene.backup() + "}";
        else
            out += gene.backup() + ",";
    }

    out += "}\n\t\t\t\t\t\t\t}\n";
    return out;
}

double Neuron::sigmoid(double x) {
    return 2.0 / (1.0 + std::exp(-4.9 * x)) - 1.0;
}
