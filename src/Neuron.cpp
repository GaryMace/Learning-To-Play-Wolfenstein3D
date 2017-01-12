//
// Created by gary on 08/01/17.
//
#include "Neuron.h"
#include <cmath>

std::string Neuron::backup() {
    std::string out = "\n\t\t\tNeuron{";

    out += "value=" + std::to_string(value) + ",";
    out += "\n\t\t\t\tinputs={";
    for (int i = 0; i < inputs.size(); i++) {
        Gene gene = inputs[i];
        if (i == inputs.size() - 1)
            out += gene.backup() + "}";
        else
            out += gene.backup() + ",";
    }

    out += "}\n";
    return out;
}

double Neuron::sigmoid(double x) {
    return 2.0 / (1.0 + std::exp(-4.9 * x)) - 1.0;
}
