//
// Created by gary on 19/11/16.
//
#include "NEURON.h"
#include <iostream>
#include <fstream>
using namespace std;

void Neuron::foo() {
	ofstream myfile;
  	myfile.open ("example.txt");
  	myfile << "Hi Arthur!! .\n";
  	myfile.close();
}

int Neuron::bar(int x, int y) {
	ofstream myfile;
  	myfile.open ("arith.txt");
  	myfile << x+y;
  	myfile.close();
    return x+y;
}
