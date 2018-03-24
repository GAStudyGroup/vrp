#include "Population.hpp"

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
    std::sort(this->getPop().begin(),this->getPop().end(),
     [](Tour& a, Tour& b) {        
        return  a.getFitness() > b.getFitness();
    });
}
