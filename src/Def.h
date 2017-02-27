//
// Created by gary on 08/01/17.
//

#ifndef TESTBENCH_DEF_H
#define TESTBENCH_DEF_H

#define POPULATION 10           //TODO: refactor
#define STALE_SPECIES 15
#define TIMEOUT 20 //was 20
#define MUTATION_TYPES 7        //consider moving later
#define INPUTS 11               // doors, walls, walk space, enemies, ammo, health, keys, guns, push walls, locked doors, elevator
#define SEARCH_GRID 25          //5x5 grid
#define OUTPUTS 10              //{forward, backward, left, right, shoot, space, weap1, weap2, weap3, weap4}

#define DELTA_DISJOINT 2.0
#define DELTA_WEIGHTS  0.4
#define DELTA_THRESHOLD 1.0

// Validation for these values?
#define MUTATE_CONNECTIONS_CHANCE 0.25
#define MUTATE_LINK_CHANCE 2.0
#define MUTATE_NODE_CHANCE 0.5
#define MUTATE_BIAS_CHANCE 0.4
#define MUTATE_ENABLE_CHANCE 0.2
#define MUTATE_DISABLE_CHANCE 0.4
#define CROSSOVER_CHANCE 0.75
#define PERTURBATION_CHANCE 0.9
#define STEP_SIZE 0.1

#define MAX_NODES 100000

#endif //TESTBENCH_DEF_H
