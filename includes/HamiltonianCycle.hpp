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
    Adaptação do VRP para utilizar o GPX.

    Devido as limitações impostas pelo GPX, é necessário fazer uma reconstrução do grafo gerado pelo VRP. 

    A adaptação consiste em transformar cada volta ao Depósito em uma aresta para um nó diferente, esse nó simboliza o depósito porém é representado de uma maneira completamente unica, seguindo as limitações de Ciclo Hamiltoniano do GPX.

    A classe executa a transição de um objeto Tour para um vector<string> contendo todos os depósitos com uma marcação para unificação, o token é '.

    A ideia se baseia no mesmo conceito tratado dentro do GPX para os nós Ghost.
*/

class HamiltonianCycle {
public:
    
    // "parentsHamiltonian" is a data structure basic used to combine two vectors
    using parentsHamiltonian = pair<vector<string>, vector<string>>;
    
    // Basic method to call all implementations to execute the algorithm
    static parentsHamiltonian toHamiltonianCycle(Tour, Tour);
    
private:
    // Store all content of correlation, used by algorithm to decide the subtour is a better choice than others
    using ResolveScore = struct ResolveScore {
        int subRed;
        int subBlue;
        int score;

        ResolveScore(int subRed , int subBlue,int score)
        :subRed(subRed)
        ,subBlue(subBlue)
        ,score(score){}
    };

    // Store the information of correlation beetween two subtours
    using Correlation = struct Correlation{
        int score;
        int numberInOut;

        Correlation(int score, int numberInOut)
        :score(score), 
        numberInOut(numberInOut){};
    };
    // Map to organize all correlations
    using correlationMap = map<int, vector<Correlation>>;
    // Vector to store choices made by algorithm, first=redSub | second=blueSub
    using choosenSubs = vector<pair<int, int>>;
    using subTours = vector<vector<int>>;

    // Execute verification All X All in the subtours and generate a list with grades of correlation
    void generateRanking();

    // Using the ranking, will choose the bests combinations of subs[red - blue]
    void choosenToursToMap();

    // Rebuild the tours following the choices already made
    parentsHamiltonian rebuildTours();

    // With tour ready, search all elements that are Depots and generate Copies using token
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