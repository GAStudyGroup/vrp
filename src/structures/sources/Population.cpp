#include "Population.hpp"
#include <algorithm>
Population::Population(){}
Population::Population(vector<Tour>& pop):pop(pop){}

void Population::addNewTour(Tour& tour){ pop.push_back(tour); }

vector<Tour>& Population::getPop(){ return(pop); }

std::ostream& operator<<(std::ostream& output, Population& pop)
{ // Overload de operador para impressão da população
    //int i{ 0 };
    for (Tour t : pop.pop) {
        //output << "Tour " << i << ": ";
        output << t;
        //++i;
       output << "\n";
    }
    return (output);
}

void Population::sortPop(){
    std::sort(pop.begin(), pop.end(),
     [](Tour& a, Tour& b) {        
        return  a.getFitness() > b.getFitness();
    });
}

std::pair<int, int> Population::getBestSolution() {
    int bestDist{std::numeric_limits<int>::max()}, bestPos{0};
    int size{(int)pop.size()};

    for(int i{0}; i<size; i++) {
        int dist{(int)pop[i].getDist()};
        if(dist < bestDist) {
            bestDist = dist;
            bestPos = i; 
        }
    }
    return(std::make_pair(bestPos, bestDist));
}
