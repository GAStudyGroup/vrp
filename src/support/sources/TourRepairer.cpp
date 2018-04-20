#include<iostream>
#include<string>
#include<algorithm>

#include "TourRepairer.hpp"
#include "Configs.hpp"
#include "CustomerMap.hpp"
#include "TourUtils.hpp"

using std::cout;
using std::endl;

Tour TourRepairer::repairTourV3(Tour& original){
    //cout<<"Iniciando Reparação"<<endl;
    auto subtours= splitSubTours(original.getRoute());
    //sortSubsByCharge(subtours);
    changeCustomers(subtours);    
    original=tourRebuilder(subtours);
    return original;
}
Tour TourRepairer::repairTourV4(Tour& original){
    //cout<<"Iniciando Reparação"<<endl;
    auto subtours= splitSubTours(original.getRoute());
    //sortSubsByCharge(subtours);
    changeCustomersV4(subtours);    
    original=tourRebuilder(subtours);
    return original;
}
//Applies V4.0
void TourRepairer::changeCustomersV4(vector<vector<int>>&subtours){
    changeCustomersMachineV4(subtours,&getLightestCustomer);
    changeCustomersMachineV4(subtours,&getHeaviestCustomer);
}
//Applies V3.0
void TourRepairer::changeCustomers(vector<vector<int>>& subtours){
    changeCustomersMachine(subtours,&getHeaviestCustomer);
    changeCustomersMachine(subtours,&getLightestCustomer);
}
//Version 4.0 Searchs for the best of all tours to insert the customer
void TourRepairer::changeCustomersMachineV4(vector<vector<int>>& subtours, 
    int getCustomerToChange(vector<int>)){
//Getting all overloaded subs
    vector<int> OverIds=getOverloadedSubs(subtours);
    //Possible tours
    vector<std::pair<int,vector<int>>> possible;
    for(int id: OverIds){
        int toChangeId=getCustomerToChange(subtours[id]);
        for(unsigned i=0;i<subtours.size();i++){ //Goes through all possible subtours
            if(i!= (unsigned)(id)){ //If the value is not itself
                for(unsigned j=0;j<subtours.size();j++){ //Checks all possible positions
                    if(!willOverload(subtours[i],toChangeId) && (toChangeId!=
                    Globals::customerMap.getDepotId())){ //Validate
                        vector<int> aux = subtours[i]; // This aux stores the possible move
                        //eraseElement(subtours[id],toChangeId);
                        aux.emplace(aux.begin()+
                                checkBestPosition(aux,toChangeId),toChangeId);
                        possible.push_back(std::make_pair(i,aux));
                    }
                }
                
            }
        }
        if(possible.size()>0){
            eraseElement(subtours[id],toChangeId);
            auto bestPossible=getBestPossibleTour(possible,subtours);
            subtours[bestPossible.first]=bestPossible.second;
            possible.clear();
        }
    }
}
//Version 3.0 of function, searchs only among subtours
void TourRepairer::changeCustomersMachine(vector<vector<int>>& subtours, 
    int getCustomerToChange(vector<int>)){
    vector<int> overIds=getOverloadedSubs(subtours);
        for(int id: overIds){//Ids dos sobrecaregados
            for(unsigned i=0;i<subtours.size();i++){ //Itera por todos os outros subs
                if(i!=(unsigned)id){ //Não altera em relação a ele mesmo
                    for(unsigned j=0;j<subtours[id].size();j++){ //Testa todas possibilidades 1 mais pesado, 2 etc..
                        int idtoChange=getCustomerToChange(subtours[id]);
                        if(!willOverload(subtours[i],idtoChange) && (idtoChange!=
                            Globals::customerMap.getDepotId())){
                                eraseElement(subtours[id],idtoChange);
                                //Adicionar aqui possível heuristica de dist
                                //subtours[i].push_back(idtoChange);
                                subtours[i].emplace(subtours[i].begin()+
                                checkBestPosition(subtours[i],idtoChange),idtoChange);
                        }
                    }
                }
            }        
        }
}
//Returns the best tour possible for the customer to be inserted
std::pair<int,vector<int>> TourRepairer::getBestPossibleTour(
     vector<std::pair<int,vector<int>>>  possible, vector<vector<int>> subtours){    
    vector<std::pair<int,vector<int>>> rebuilt;
    for(unsigned i=0; i<possible.size();i++){
        rebuilt.push_back(std::make_pair(possible[i].first,tourRebuilder(subtours)));
    }
    std::sort(rebuilt.begin(),rebuilt.end(),[](auto &a, auto &b){
        return TourUtils::getSubDistance(a.second) < TourUtils::getSubDistance(b.second);
    });   
    return possible[rebuilt[0].first];
 }
//Split the tour onto subtours
vector<vector<int>> TourRepairer::splitSubTours(vector<int>& tour){
    vector<vector<int>> subtours= Tour(tour).explodeSubTours();
    for(unsigned i=0;i<subtours.size();i++){
        subtours[i].emplace(subtours[i].begin(),Globals::customerMap.getDepotId());
    }
    int emptyRoutes=Configs::truckNumber-subtours.size();
    vector<int> aux;
    aux.push_back(Globals::customerMap.getDepotId());
    if(emptyRoutes>0){
        for(int i=0;i<emptyRoutes;i++){
            subtours.push_back(aux);
        }
    }
    return subtours;
}
//Rebuilds tour after changes
vector<int> TourRepairer::tourRebuilder(vector<vector<int>>& subtours){
    vector<int> newTour;
    for(auto subtour:subtours){
        for(auto customer: subtour){
            newTour.push_back(customer);
        }
    }
    return newTour;
}
//Prints the subtours vector
void TourRepairer::printSubtours(vector<vector<int>>& subtours){
    int it=0;
    for(auto subtour: subtours){
        cout<<"SubTour "<<it<<": ";
        for(auto customer: subtour){
            cout<<customer<<" ";
        }
        cout<<" Charge: "<<TourUtils::getSubCharge(subtour);
        //cout<<" Heaviest Customer: "<<getHeaviestCustomer(subtour);
        it++;
        cout<<endl;
    }
    cout<<endl;
}
//Gets the ids of the subtours that are overloaded
vector<int> TourRepairer::getOverloadedSubs(vector<vector<int>>& subtours){
    vector<int> SubsIds;
    int id=-1;;
    for(auto subtour: subtours){
        id++;
        if(TourUtils::getSubCharge(subtour)>Globals::customerMap.getTruckCapacity()){
            SubsIds.push_back(id);
        }
    }
    return SubsIds;
}
//Sorts the subtours vector by their charge
void TourRepairer::sortSubsByCharge(vector<vector<int>>& subtours){
    std::sort(subtours.begin(),subtours.end(),
     [](vector<int>& a, vector<int>& b) {        
        return  TourUtils::getSubCharge(a) < TourUtils::getSubCharge(b);
    });
    
}
//Returns the customer with the heavistes demand
int TourRepairer::getHeaviestCustomer(vector<int> subtour){
    std::sort(subtour.begin(),subtour.end(),[](int &a, int &b){
        return getCustomerDemand(a) > getCustomerDemand(b);
    });
    return subtour[0];
}
//Returns the customer with the lowest demand 
int TourRepairer::getLightestCustomer(vector<int> subtour){
    std::sort(subtour.begin(),subtour.end(),[](int &a, int &b){
        return getCustomerDemand(a) < getCustomerDemand(b);
    });
    int customerId=subtour[0]; //Sempre pega o menor
    return customerId;
}

//Erase a customer from a subtour
void TourRepairer::eraseElement(vector<int>& subtour,int element){
    subtour.erase(find(subtour.begin(),subtour.end(),element));
}

//Acesses the demand of a customer
int TourRepairer::getCustomerDemand(int id){
    return Globals::customerMap.getCustomer(id).getDemand();
}
//Checks if a given customer will overload the subttour
bool TourRepairer::willOverload(vector<int>& subtour, int id){
    int totalCharge= (TourUtils::getSubCharge(subtour) + getCustomerDemand(id));
    if(totalCharge>Globals::customerMap.getTruckCapacity()){
        // cout<<"Over"<<endl;
        return true;
    }    
    return false;
}
//Checks the best position in a subtour to insert the customer
int TourRepairer::checkBestPosition(vector<int> subtour,int customer){
    if(subtour.size()==1){
        return 1;
    }
    vector<std::pair<int,vector<int>>> lista;
    for(unsigned i=1;i<subtour.size();i++){
        vector<int> temp = subtour;
        temp.emplace(temp.begin()+i,customer);
        lista.push_back(std::make_pair(i,temp));
    }
    std::sort(lista.begin(),lista.end(),[](auto &a, auto &b){
        return TourUtils::getSubDistance(a.second) < TourUtils::getSubDistance(b.second);
    });
    return lista[0].first;
}