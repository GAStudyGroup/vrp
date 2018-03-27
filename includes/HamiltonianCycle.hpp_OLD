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
/* 
    Adaptação para o VRP

    Devido a limitação do GPX para funcionar em Grafos Hamiltonianos, é necessário que o Tour geradopara o VRP seja  transformado. 
    Dessa maneira, o GPX irá tratar todos o depósito como vários pontos separados, que estão nomesmo local.

    Teoria baseada no passo onde o GPX gera os "ghosts" para tentar aumentar as partições geradas. 
*/
namespace HamiltonianCycle {
    // DataType to keep all values of correlation
    using Correlation = struct Correlation{
        int score;
        int numberInOut;

        Correlation(int score, int numberInOut)
        :score(score), 
        numberInOut(numberInOut){};
    };

    using ResolveScore = struct ResolveScore {
        int posRed;
        int posBlue;
        int score;

        ResolveScore(int posRed, int posBlue, int score)
        :posRed(posRed)
        ,posBlue(posBlue)
        ,score(score){}
    };

    using SecondResolve = struct SecondResolve {
        int posRed;
        int posBlue;
        int subRed;
        int subBlue;
        int score;

        SecondResolve(int posRed, int posBlue, int subRed , int subBlue,int score)
        :posRed(posRed)
        ,posBlue(posBlue)
        ,subRed(subRed)
        ,subBlue(subBlue)
        ,score(score){}
    };
    // Id of main subtour (using Red) and all correlations of all subtours blue
    using correlationMap = map<int, vector<Correlation>>;
    using parentsHamiltonian = pair<vector<string>, vector<string>>;
    using choosenSubs = vector<pair<int, int>>;
    using subTours = vector<vector<int>>;

    parentsHamiltonian toHamiltonianCycle(Tour, Tour);

    choosenSubs mapDepots(subTours&, subTours&);

    choosenSubs validateChoosenSubs(choosenSubs);

    choosenSubs fillWithSecondOptions(choosenSubs, const unsigned);

    parentsHamiltonian rebuildTours(subTours&, subTours&, choosenSubs&);

    vector<string> createDepotCopies(vector<string>);

    // Utilities functions
    choosenSubs findEqualsChooses(choosenSubs&, int);
};

#endif
