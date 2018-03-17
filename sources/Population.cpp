#include "Population.hpp"

Population::Population(){}
Population::Population(vector<Population::Tour>& pop):pop(pop){}

void Population::addNewTour(Population::Tour& tour){ pop.push_back(tour); }

vector<Population::Tour>& Population::getPop(){ return(pop); }

std::ostream& operator<<(std::ostream& output, Population& pop)
{ // Overload de operador para impressão da população
    int i{ 0 };
    for (Population::Tour t : pop.pop) {
        output << "Tour " << i << ": ";
        for(int id : t){
            output << id << " ";
        }
        ++i;
        output << "\n";
    }
    return (output);
}
