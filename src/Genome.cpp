//
// Created by gary on 31/12/16.
//
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cmath>
//#include <windows.h>
#include "Genome.h"
#include "Genus.h"
#include "wl_def.h"

/*
=================================
=
= {'-'} Genome::backup
=
= Outputs a json style string representation of a genome.
=
=================================
 */
std::string Genome::backup() {
    std::list<Gene>::iterator geneItr;
    std::string out = "\n\t\t\t\tGenome{";
    std::string str;

    str = static_cast<std::ostringstream*>(&(std::ostringstream() << fitness))->str();
    out += "fitness=" + str + ",";
    str = static_cast<std::ostringstream*>(&(std::ostringstream() << globalRank))->str();
    out += "\n\t\t\t\t\tglobalRank=" + str + ",";
    str = static_cast<std::ostringstream*>(&(std::ostringstream() << maxNeuron))->str();
    out += "\n\t\t\t\t\tmaxNeuron=" + str + ",";

    out += "\n\t\t\t\t\tmutationRates={";
    for (int i = 0; i < MUTATION_TYPES; i++) {
        double rate = mutationRates[i];
        str = static_cast<std::ostringstream*>(&(std::ostringstream() << rate))->str();
        if (i == MUTATION_TYPES - 1)
            out += str + "},";
        else
            out += str + ",";
    }

    out += "\n\t\t\t\t\tgenes={";
    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++)
        out += geneItr->backup() + ",";
    out = out.substr(0, out.length() - 1);  //Remove last ","
    out += "\n\t\t\t\t\t},";

    out += "\n\t\t\t\t\tnetwork={";
    for (std::map<int, Neuron>::iterator it = network.begin(); it != network.end(); it++) {
        str = static_cast<std::ostringstream*>(&(std::ostringstream() << it->first))->str();
        out += "\n\t\t\t\t\t\t{key=" + str + ",value=" + it->second.backup() + "\t\t\t\t\t\t},";
    }
    if (!network.empty())
        out = out.substr(0, out.length() - 1);  //Remove last ","

    out += "\n\t\t\t\t\t}\n\t\t\t\t}";  //close network then Genome brackets

    return out;
}

/*
=================================
=
= {'-'} Genome::clone
=
= Returns a deep copy of the genome clone is invoked on.
=
=================================
 */
Genome Genome::clone() {
    std::list<Gene>::iterator geneItr;
    Genome genome;

    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++)
        genome.genes.push_back(*geneItr);
    genome.maxNeuron = maxNeuron;

    for (int i = 0; i < MUTATION_TYPES; i++)
        genome.mutationRates[i] = mutationRates[i];

    return genome;
}

/*
=================================
=
= {'-'} Genome::compare / Genome::compareByPointer
=
= compare is used for sorting in descending order, compareByPointer is used to sort a list of pointers
= in ascending order.
=
=================================
 */
int Genome::compare(const Genome &o1, const Genome &o2) {               // Sort from highest fitness to lowest
    return o1.fitness > o2.fitness;
}

int Genome::compareByPointer(const Genome *o1, const Genome *o2) {      // Sort from lowest fitness to highest
    return o1->fitness < o2->fitness;
}

/*
=================================
=
= {'-'} Genome::initMutationRates
=
= The older version of C++ used for Wolfenstein doesn't allow array instantiation in headers. This method
= is invoked from the constructor as a hack to get around this problem.
=
=================================
 */
void Genome::initMutationRates() {
    mutationRates[CONNECTIONS] = MUTATE_CONNECTIONS_CHANCE;
    mutationRates[LINK] = MUTATE_LINK_CHANCE;
    mutationRates[BIAS] = MUTATE_BIAS_CHANCE;
    mutationRates[NODE] = MUTATE_NODE_CHANCE;
    mutationRates[ENABLE] = MUTATE_ENABLE_CHANCE;
    mutationRates[DISABLE] = MUTATE_DISABLE_CHANCE;
    mutationRates[STEP] = STEP_SIZE;
}

/*
=================================
=
= {'-'} Genome::generateNetwork
=
= Generates the input and output Neurons for a Genome's network. It also adds all the curr Genome's
= Genes to each output Neuron if the Gene is enabled.
=
=================================
 */
void Genome::generateNetwork() {
    std::list<Gene>::iterator geneItr;

    for (int i = 0; i < TOTAL_INPUTS; i++) {  //Make Neurons for all inputs
        Neuron n;
        network.insert(std::make_pair(i, n));
    }
    for (int i = 0; i < OUTPUTS; i++) { //Make Neurons for all outputs
        Neuron n;
        network.insert(std::make_pair(MAX_NODES + i, n));  //notice output neurons start right after input neurons here
    }
    if (!genes.empty())
        genes.sort(Gene::compare);

    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++) {
        if (geneItr->enabled) { //If this Gene is enabled
            std::map<int, Neuron>::iterator it = network.find(geneItr->output);
            if (it == network.end()) {  //If key gene.output doesn't exist
                Neuron n;
                network.insert(std::make_pair(geneItr->output, n)); //make new entry
            }
            Neuron& n = network[geneItr->output];   //add this Gene to entry
            n.inputs.push_back(*geneItr);

            std::map<int, Neuron>::iterator it2 = network.find(geneItr->input);
            if (it == network.end()) {  //If key gene.input doesn't exist
                Neuron n2;
                network.insert(std::make_pair(geneItr->input, n2));
            }
        }
    }
}

/*
=================================
=
= {'-'} Genome::evaluateNetwork
=
= Takes an 10x25 matrix of inputs and assigns its values to the corresponding Neurons in a Genome's network.
= Also, The value of a Neuron is assigned the sigmoid of the sum of its inputs and finally if the value on an
= output Neuron is greater than 0 then the corresponding game button to that Neuron is set to be activated.
=
=================================
 */
bool* Genome::evaluateNetwork(int inputs[][SEARCH_GRID]) {
    std::list<Gene>::iterator geneItr;

    for (int i = 0; i < INPUTS; i++)
        for (int j = 0; j < SEARCH_GRID; j++)
            network[(i * SEARCH_GRID) + j].value = inputs[i][j];    //Change input values to network

    for (std::map<int, Neuron>::iterator it = network.begin(); it != network.end(); it++) {
        Neuron& n1 = it->second;    //Neuron& grabs the address of the value from the hashmap
        double sum = 0.0;
        for (geneItr = n1.inputs.begin(); geneItr != n1.inputs.end(); geneItr++) {
            Gene incoming = *geneItr;   //for each Gene of Neurons inputs
            Neuron n2 = network[incoming.input];
            sum += incoming.weight * n2.value;  //Get sum of input Genes to this Neuron
        }
        if (!n1.inputs.empty())
            it->second.value = Neuron::sigmoid(sum);    //which is needed for this step! i.e. in-place map edit
    }

    bool *outputs = new bool[OUTPUTS];
    for (int i = 0; i < OUTPUTS; i++) {
        if (network[MAX_NODES + i].value > 0)
            outputs[i] = true;
        else
            outputs[i] = false;
    }
    return outputs;
}

/*
=================================
=
= {'-'} Genome::nodeMutate
=
= Obtains a random Gene from genes list, disables it if not already adds a new node to the hidden layer.
=
=================================
 */
void Genome::nodeMutate() {
    std::list<Gene>::iterator geneItr;

    if (genes.empty())
        return;

    int randGene =  rand() % (int) genes.size();
    Gene *gene;

    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++) {
        if (randGene-- == 0) {
            gene = &(*geneItr);
            break;
        }
    }
    if (!gene->enabled)
        return;

    gene->enabled = false;
    maxNeuron++;

    Gene gene1 = gene->clone();  //TODO: is this not just making a copy and putting it at the end?
    gene1.output = maxNeuron;
    gene1.weight = 1.0;
    gene1.innovation = Genus::newInnovation();
    gene1.enabled = true;
    genes.push_back(gene1);

    Gene gene2 = gene->clone();
    gene2.input = maxNeuron;
    gene2.innovation = Genus::newInnovation();
    gene2.enabled = true;
    genes.push_back(gene2);
}

/*
=================================
=
= {'-'} Genome::pointMutate
=
= Mutates the weights on a Genome's Genes.
=
=================================
 */
void Genome::pointMutate() {
    std::list<Gene>::iterator geneItr;
    double step = mutationRates[STEP];

    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++) {
        if (Genus::nextDouble() < PERTURBATION_CHANCE)
            geneItr->weight += Genus::nextDouble() * (step * 2) - step;
        else
            geneItr->weight = (Genus::nextDouble() * 4) - 2;
    }
}

/*
=================================
=
= {'-'} Genome::clone
=
= Creates a new link between two Neurons. (Genes are essentially the Synapse of these Networks)
=
=================================
 */
void Genome::linkMutate(bool forceBias) {
    int neuron1 = randomNeuron(false);
    int neuron2 = randomNeuron(true);
    if (neuron1 < TOTAL_INPUTS && neuron2 < TOTAL_INPUTS) //Both input nodes, exit
        return;
    if (neuron2 < TOTAL_INPUTS) {    //swap output and input
        int tempNeuron = neuron1;
        neuron1 = neuron2;
        neuron2 = tempNeuron;
    }
    
    Gene newLink;
    newLink.input = neuron1;
    newLink.output = neuron2;

    if (forceBias)
        newLink.input = TOTAL_INPUTS - 1;

    if (containsLink(newLink))
        return;

    newLink.innovation = Genus::newInnovation();
    newLink.weight = (Genus::nextDouble() * 4) - 2;
    genes.push_back(newLink);
}

void Genome::mutate() {
    for (int i = 0; i < MUTATION_TYPES; i++)
        (rand() % 2 == 1) ? mutationRates[i] *= 0.95 : mutationRates[i] *= 1.05263; //TODO: validation for these values?

    if (Genus::nextDouble() < mutationRates[CONNECTIONS])
        pointMutate();

    double prob = mutationRates[LINK];
    while (prob > 0) {
        if (Genus::nextDouble() < prob){
            linkMutate(false);
        }
        prob--;
    }
    
    prob = mutationRates[BIAS];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            linkMutate(true);
        prob--;
    }
    
    prob = mutationRates[NODE];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            nodeMutate();
        prob--;
    }
    
    prob = mutationRates[ENABLE];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            mutateEnableDisable(true);
        prob--;
    }

    prob = mutationRates[DISABLE];
    while (prob > 0) {
        if (Genus::nextDouble() < prob)
            mutateEnableDisable(false);
        prob--;
    }
}

void Genome::mutateEnableDisable(bool enable) {
    std::list<Gene>::iterator geneItr;
    std::list<Gene*>::iterator geneItr2;
    std::list<Gene*> candidates;  //vector of pointers

    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++)
        if (geneItr->enabled != enable)
            candidates.push_back(&(*geneItr));

    if (candidates.empty())
        return;

    //pointer to obj ref from original vector
    int randGene = rand() % (int) candidates.size();
    Gene *gene;
    for (geneItr2 = candidates.begin(); geneItr2 != candidates.end(); geneItr2++)
        if (randGene-- == 0)
            gene = *geneItr2;

    gene->enabled = !gene->enabled; //change value via pointer
}

bool Genome::containsLink(Gene link) {
    std::list<Gene>::iterator geneItr;

    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++)
        if (geneItr->input == link.input && geneItr->output == link.output)
            return true;
    return false;
}

double Genome::disjoint(Genome genome) {
    std::list<Gene>::iterator geneItr;
    std::list<Gene>::iterator geneItr2; //Gene 2 iterator
    double disjointGenes = 0.0;
    bool isDisjoint = true;

    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++) {          //Gene 1
        for (geneItr2 = genome.genes.begin(); geneItr2 != genome.genes.end(); geneItr2++) { //Gene 2
            if (geneItr->innovation == geneItr2->innovation) {      //Is Gene 1 == Gene 2
                isDisjoint = false;
                break;
            }
        }
        if (isDisjoint) {
            disjointGenes++;
            isDisjoint = true;
        }
    }
    int max = std::max((int) genes.size(), (int) genome.genes.size());
    if (disjointGenes == 0 || max  == 0)
       return 0;
    else
        return disjointGenes / max;
}

//TODO: revisit this method
int Genome::randomNeuron(bool nonInput) {
    std::list<Gene>::iterator geneItr;
    std::map<int, bool> neurons;
    
    if (!nonInput)
        for (int i = 0; i < TOTAL_INPUTS; i++) 
            neurons[i] = true;
    
    for (int i = 0; i < OUTPUTS; i++)
        neurons[MAX_NODES + i] = true;

    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++) {
        if (!nonInput || geneItr->input >= TOTAL_INPUTS)
            neurons[geneItr->input] = true;
        if (!nonInput || geneItr->output >= TOTAL_INPUTS)
            neurons[geneItr->output] = true;
    }

    int randNeuron = rand() % (int) neurons.size();
    for (std::map<int, bool>::iterator it = neurons.begin(); it != neurons.end(); it++)   //get random neuron in there (Not a hidden one)
        if (randNeuron-- == 0)
            return it->first;
    
    return 0;
}

double Genome::weights(Genome genome) {
    std::list<Gene>::iterator geneItr;
    std::list<Gene>::iterator geneItr2; //Gene 2 iterator
    double coincident = 0.0;
    double sum = 0.0;

    for (geneItr = genes.begin(); geneItr != genes.end(); geneItr++) {
        for (geneItr2 = genome.genes.begin(); geneItr2 != genome.genes.end(); geneItr2++) {
            if (geneItr->innovation == geneItr2->innovation) {
                sum += std::fabs(geneItr->weight - geneItr2->weight); //std::fabs is abs() on a float, too bad it doesn't exist pre C++11
                coincident++;
                //std::cout << "weights same" << std::endl;
                break;
            }
        }
    }
    if (sum == 0 && coincident == 0)    //since  0.0/0.0 in C++ is -nan ...
        return 0;
    else
        return sum / coincident;
}

bool Genome::sameSpecies(Genome genome) {
    double dd = disjoint(genome) * DELTA_DISJOINT;
    double dw = weights(genome) * DELTA_WEIGHTS;

    return dd + dw < DELTA_THRESHOLD;
}
