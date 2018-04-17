#include <algorithm>
#include "Trim.hpp"
#include "TourRepairer.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"

Tour Trim::applyTrim(Tour& tour){
    if(willApply(tour)){
        //std::cout<<"Aplicando"<<std::endl;
        auto subtours= TourRepairer::splitSubTours(tour.getRoute());
        //TourRepairer::printSubtours(subtours);
        //std::cout<<"Depois"<<std::endl;
        subtours=moveCustomers(subtours,getExtraSubsN(tour));
        tour=TourRepairer::tourRebuilder(subtours);        
        return tour;
    }else{
        //std::cout<<"Não Aplicando"<<std::endl;
        return tour;
    }    
}
vector<vector<int>> Trim::moveCustomers(vector<vector<int>> subtours,int extraN){
    auto backup= subtours;
    vector<int> oldTour=TourRepairer::tourRebuilder(subtours);
    vector<vector<int>> extraTours;
    vector<int> tourIds= getToursToTrim(subtours,extraN);
    if(tourIds.size()==0){
        return subtours;
    }
    for(int i=tourIds.size()-1;i>=0;i--){
        extraTours.push_back(subtours[tourIds[i]]);
        subtours[tourIds[i]].clear();
    }
    for(auto sub:subtours){
        if(sub.size()==0){
            subtours.erase(std::find(subtours.begin(),subtours.end(),sub));
        }
    }
    std::sort(extraTours.begin(),extraTours.end(),[](auto &a, auto&b){
        return TourUtils::getSubCharge(b) > TourUtils::getSubCharge(a);
    });  
    bool flag=false;
    for(auto &extra: extraTours){
        for(int id: extra){
            for(auto &sub :subtours){
                if(flag==true){
                    break;
                }
                if(!TourRepairer::willOverload(sub,id) && id!=Globals::customerMap.getDepotId()){
                    //std::cout<<"ID:"<<id<<  std::endl;
                    //Improve heuristic here                    
                    sub.push_back(id);
                    extra.erase(std::find(extra.begin(),extra.end(),id));
                    flag=true;
                }                 
            } 
        }
        flag=false;              
    }
    for(auto extra:extraTours){
        subtours.push_back(extra);           
    }
    //TourRepairer::printSubtours(subtours);
    vector<int> newTour= TourRepairer::tourRebuilder(subtours);
    // Tour aux = Tour(newTour);
    // std::cout <<aux<<std::endl;
    if(TourUtils::getSubDistance(newTour)<TourUtils::getSubDistance(oldTour)){
        // std::cout<<"Melhorou"<<std::endl;
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
    if(tour.explodeSubTours().size()<=Globals::customerMap.getMnv()){
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
    n= tour.explodeSubTours().size() - Globals::customerMap.getMnv();
    return n;
}


