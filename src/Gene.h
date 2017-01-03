class Gene {
public:
    int input;
    int output;
    double weight;
    bool enabled = true;
    int innovation;

    Gene clone();
    static bool compare(const Gene &g1, const Gene &g2);
};