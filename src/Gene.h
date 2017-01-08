//
// Created by gary on 07/01/17.
//

class Gene {
public:
    int input;  //in-node
    int output; //out-node
    double weight;  //connection weight
    bool enabled = true;    //enable-bit
    int innovation; //connection ID

    Gene clone();
    static bool compare(const Gene &g1, const Gene &g2);
};