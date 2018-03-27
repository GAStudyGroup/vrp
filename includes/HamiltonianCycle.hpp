#ifndef HAMILTONIAN_CYCLE_HPP
#define HAMILTONIAN_CYCLE_HPP

#include <string>
#include <utility>
#include <vector>
#include <map>

#include "Tour.hpp"

using std::string;
using std::pair;
using std::vector;
using std::map;
using std::find;

class HamiltonianCycle {
public:
    using parentsHamiltonian = pair<vector<string>, vector<string>>;
    static parentsHamiltonian toHamiltonianCycle(Tour, Tour);
    
private:
    using ResolveScore = struct ResolveScore {
        int subRed;
        int subBlue;
        int score;
        ResolveScore(int subRed , int subBlue,int score)
        :subRed(subRed)
        ,subBlue(subBlue)
        ,score(score){}
    };

    using Correlation = struct Correlation{
        int score;
        int numberInOut;

        Correlation(int score, int numberInOut)
        :score(score), 
        numberInOut(numberInOut){};
    };
    using correlationMap = map<int, vector<Correlation>>;
    using choosenSubs = vector<pair<int, int>>;
    using subTours = vector<vector<int>>;

    void generateRanking();

    void choosenToursToMap();

    parentsHamiltonian rebuildTours();

    vector<string> createDepotCopies(vector<string>);

    unsigned correlationDuplicate{0};
    unsigned correlationIn_Out{0};
    choosenSubs choosen;
    correlationMap ranking;
    vector<ResolveScore> resolveScore;
    subTours redSubs;
    subTours blueSubs;
    string depotCopyToken{"'"};
};

#endif