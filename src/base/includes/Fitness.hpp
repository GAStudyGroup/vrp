#ifndef FITNESS_HPP
#define FITNESS_HPP

#include <vector>

using std::vector;

namespace OurFitness{
    double ourFitness(vector<int>&);
    // return a fitness value of a route inside the tour
    double subFitness(vector<int>&, double&);
    // return a fitness value, with penalty, of a route inside the tour
    double subFitnessPenalty(vector<int>&, double&);
}

namespace AdvancedFitness{
    //More sophisticated method to calculate fitness
    double advancedFitness(vector<int>&);
    double calcAlpha();
    double calcPenalty(vector<int>&);
}

namespace newFitness{
    double newFitness(vector<int>&);
    double calcPenalty(vector<int>&);
    int calcPenaltyMultiplicator(vector<int>&);
}
#endif