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

Gene Gene::clone() {
    Gene copy;
    copy.enabled = this->enabled;
    copy.input = this->input;
    copy.output = this->output;
    copy.innovation = this->innovation;
    copy.weight = this->weight;

    return copy;
}

bool Gene::compare(const Gene &g1, const Gene &g2) {
    return g1.output < g2.output;
}