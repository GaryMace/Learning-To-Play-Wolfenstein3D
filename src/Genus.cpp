//
// Created by gary on 02/01/17.
//
#include "Genus.h"

int Genus::innovation = OUTPUTS;

int Genus::newInnovation() {
    return ++innovation;
}

double Genus::nextDouble() {
    return ((double)rand() / RAND_MAX);
}