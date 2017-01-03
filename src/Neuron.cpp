//
// Created by gary on 19/11/16.
//
#include "Neuron.h"


double Neuron::sigmoid(double x) {
	return 2.0 / (1.0 + exp(-4.9 * x)) - 1.0;
}
