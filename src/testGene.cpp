//
// Created by gary on 08/01/17.
//
#include <iostream>
#include "Gene.h"

int main() {
    Gene g;

    std::cout << "Gene enabled (expected 1): " << g.enabled << std::endl;

    g.enabled = false;
    std::cout << "Gene enabled (after edit, expected 0): " << g.enabled << std::endl;

    g.input = 2;
    std::cout << "Gene input (expected 2): " << g.input << std::endl;
    g.output = 3;
    std::cout << "Gene output (expected 3): " << g.output << std::endl;

    std::cout << "Gene innovation (expected 0): " << g.innovation << std::endl;

    g.weight = 2.4;
    std::cout << "Gene weight (expected 2.4): " << g.weight << std::endl;

    std::cout << "Gene backup:\n" << g.backup() << std::endl;
    std::cout << "Gene encoding (expected 2,3,0,0,2.4):\n" << g.encode() << std::endl;
    return 0;
}