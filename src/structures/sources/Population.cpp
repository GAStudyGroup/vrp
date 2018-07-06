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

Tour Population::getBestSolution() {
    Tour best=*std::min_element(this->getPop().begin(),this->getPop().end(),[](auto &a, auto &b){
        return a.getDist() < b.getDist();
    });
    return best;
}

int Population::totalToursValid() {
    int validCount{0};
    for(Tour t : pop) {
        if(t.isValid()) validCount++;
    }
    return(validCount);
}

void Population::printPopulationStats(){
    //Get average distance
    double total=0;
    int validRoutes=0;
    for(auto tour: pop){
        total+=tour.getDist();
        validRoutes+=tour.getValidRoutes();
    }
    std::cout<<"Average Distance: "<<(total/pop.size())<<std::endl;
    std::cout<<"Valid Routes: "<<validRoutes<<std::endl;
}