#include <algorithm>
#include "Trim.hpp"
#include "TourRepairer.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"

using std::cout;
using std::endl;
Tour Trim::applyTrim(Tour& tour){
    if(willApply(tour)){
        //std::cout<<"Aplicando"<<std::endl;
        auto subtours= TourRepairer::splitSubTours(tour.getRoute());
        //std::cout<<"Depois"<<std::endl;
        // TourRepairer::printSubtours(subtours);
        subtours=moveCustomers(subtours,getExtraSubsN(tour));
        tour=TourRepairer::tourRebuilder(subtours);        
        return tour;
    }else{
        //std::cout<<"Não Aplicando"<<std::endl;
        return tour;
    }    
}
vector<vector<int>> Trim::moveCustomers(vector<vector<int>> subtours,int extraN){
    auto backup = subtours;
    auto tourIds= getToursToTrim(subtours,extraN);
    vector<vector<int>> extraTours;

    // cout<<"Tour ids: ";
    // printVector(tourIds);
    // cout<<endl;
    //Colocando os tours extras 
    for(int id:tourIds){
        extraTours.push_back(subtours[id]);
    }
    // cout<<"Tours extra"<<endl;
    // TourRepairer::printSubtours(extraTours);
    //Removendo os tours extras
    for(auto extra:extraTours){
        subtours.erase(std::find(subtours.begin(),subtours.end(),extra));
    }
    // cout<<"Subtours all"<<endl;
    // TourRepairer::printSubtours(subtours);
    
    bool alreadyInserted=false;
    for(auto &extra: extraTours){
        for(int id: extra){
            for(auto &sub: subtours){
                if(alreadyInserted){
                    break;
                }
                if(!TourRepairer::willOverload(sub,id) && id!=Globals::customerMap.getDepotId()){
                    //sub.push_back(id);
                    sub.emplace(sub.begin()+TourRepairer::checkBestPosition(sub,id),id);
                    extra.erase(std::find(extra.begin(),extra.end(),id));
                    alreadyInserted=true;
                }   
            }            
        }
        alreadyInserted=false;
    }
    for(auto extra:extraTours){
        subtours.push_back(extra);
    }
    vector<int> newTour=TourRepairer::tourRebuilder(subtours);
    vector<int> oldTour=TourRepairer::tourRebuilder(backup);
    if(TourUtils::getSubDistance(newTour)<TourUtils::getSubDistance(oldTour)){
        return subtours;
    }
    return backup;
}

vector<int> Trim::getToursToTrim(vector<vector<int>> subtours, int extraN){
    vector<int> ids;
    vector<std::pair<int,vector<int>>> ranking;
    int id=0;
    for(auto sub: subtours){
        ranking.push_back(make_pair(id,sub));        
        id++;
    }
    std::sort(ranking.begin(),ranking.end(),[](auto &a, auto &b){
        return TourUtils::getSubCharge(a.second) < TourUtils::getSubCharge(b.second);
    });
    for (auto val: ranking){
        if(TourUtils::getSubCharge(val.second)>0){
            extraN--;
            ids.push_back(val.first);
        }
        if(extraN==0){
            break;
        }
    }
    // for(auto i: ids){
    //     std::cout<< "Tour: "<<i<<std::endl;
    // }
    return ids;
}

bool Trim::willApply(Tour& tour){
    if(tour.explodeSubTours().size()<=((unsigned)Globals::customerMap.getMnv())){
        return false;
    }
    auto split= TourRepairer::splitSubTours(tour.getRoute());
    if(TourRepairer::getOverloadedSubs(split).size()>0){
        return false;
    }
    return true;
}
int Trim::getExtraSubsN(Tour& tour){
    int n=0;
    n=  tour.explodeSubTours().size() - Globals::customerMap.getMnv();
    return n;
}


void Trim::printVector(vector<int> vet){
    for(auto el:vet){
        std::cout<< el <<" ";
    }
}