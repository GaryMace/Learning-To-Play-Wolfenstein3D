//
// Created by gary on 08/01/17.
//
#include <iostream>
#include "Genome.h"

int main() {
    Genome g1;
    Genome g2;
    g2 = g1.clone();

    bool identical = true;
    for (int i = 0; i < MUTATION_TYPES; i++)
        if (g1.mutationRates[i] != g2.mutationRates[i])
            identical = false;
    std::cout << "Test Genome::clone (expected 1): " << identical << std::endl;

    std::cout << "g1 network size on init (expected 0): " << g1.network.size() << std::endl;
    g1.generateNetwork();

    std::vector<double> inputs = {1,2,3,4};
    std::vector<double> out = g1.evaluateNetwork(inputs);
    std::cout << "g1 evalNetwork size (expected 8): " << out.size() << std::endl;
    std::cout << "g1 output vals (expected eight default vals i.e. 0): ";
    for (double outVal : out)
        std::cout << outVal << " ";
    std::cout << std::endl;

    std::cout << "g1 network size instantiated (expected 12): " << g1.network.size() << std::endl;

    std::cout << g1.backup() << std::endl;
    return 0;
}