#include <algorithm>
#include <cmath>

#include "HamiltonianCycle.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"
#include "Distance.hpp"

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::toHamiltonianCycle(Tour red, Tour blue){
    HamiltonianCycle obj;
    obj.correlationDuplicate = (red.getRoute().size()*0.2);
    obj.correlationIn_Out = obj.correlationDuplicate*3; 

    obj.redSubs = red.explodeSubTours();
    obj.blueSubs = blue.explodeSubTours();

    obj.generateRanking();  

    obj.choosenToursToMap();

    std::sort(obj.choosen.begin(), obj.choosen.end(), [](auto &left, auto &right){ return(left.first < right.first); });

    parentsHamiltonian newTours = obj.rebuildTours(
                            red.getEmptySubtoursNumber(),
                            blue.getEmptySubtoursNumber());

    newTours.first = obj.createDepotCopies(newTours.first);
    newTours.second = obj.createDepotCopies(newTours.second);

    return(newTours);
}

void HamiltonianCycle::generateRanking(){
    // Generate a ranking with correlations beetween all tours
    int redSubsSize{(int)redSubs.size()}, blueSubsSize{(int)blueSubs.size()};
    choosenSubs choosen;
    for(int red{0}; red<redSubsSize; red++){
        for(int blue{0}; blue<blueSubsSize; blue++){

            ranking[red].push_back(Correlation(0, 0));
            int redInsideSize{(int)redSubs[red].size()};
            int blueInsideSize{(int)blueSubs[blue].size()};

            for(int redElement{0}; redElement<redInsideSize; redElement++){

                // Find in SubBlue the red element.
                auto position = std::distance(blueSubs[blue].begin(), find(blueSubs[blue].begin(), blueSubs[blue].end(), redSubs[red][redElement]));

                // If element found 
                if(!(position>=blueInsideSize)){
                    // Verificate if the element found is a border
                    if((position==0 || position==(redInsideSize-1))&&(redElement==0 || redElement==(redInsideSize-1))){
                        ranking[red][blue].score+=correlationIn_Out;
                        ranking[red][blue].numberInOut++;
                    }
                    ranking[red][blue].score+=correlationDuplicate;
               }
            }
        }
    }
}

void HamiltonianCycle::choosenToursToMap(){
    vector<int> redNotChoosen, blueNotChoosen;
    // will load all values of ranking in Struct to Resolve and make the best choices
    for(unsigned i{0}; i<redSubs.size(); i++){
        redNotChoosen.push_back(i);
    }
    for(unsigned i{0}; i<blueSubs.size(); i++){
        blueNotChoosen.push_back(i);
    }
    for(int red : redNotChoosen){
        for(int blue : blueNotChoosen){
            resolveScore.push_back(ResolveScore(red, blue, ranking[red][blue].score));
        }
    }

    // Sort values in ResolveScore to organize by score beetween subTours correlations
    std::sort(resolveScore.begin(), resolveScore.end(), [](auto &left, auto &right){ return(left.score > right.score); });

    for(auto it : resolveScore){
        // Verify if the tours correlation has not been choosen yet.
        vector<int>::iterator findRedNotChoosen{find(redNotChoosen.begin(), redNotChoosen.end(), it.subRed)};
        vector<int>::iterator findBlueNotChoosen{find(blueNotChoosen.begin(), blueNotChoosen.end(), it.subBlue)};

        bool inRedNotChoosen{findRedNotChoosen != redNotChoosen.end()};
        bool inBlueNotChoosen{findBlueNotChoosen != blueNotChoosen.end()};

        if(inRedNotChoosen && inBlueNotChoosen){
            // insert in choosen structure
            choosen.push_back(std::make_pair(it.subRed, it.subBlue));

            blueNotChoosen.erase(findBlueNotChoosen);
            redNotChoosen.erase(findRedNotChoosen);
        }
    }
}

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::rebuildTours(const int redEmpty, const int blueEmpty) {
    parentsHamiltonian parents;

    parents = buildChoosenSubs();
    parents = restoreEmptySubtours(parents, redEmpty, blueEmpty);

    return(parents);
}

vector<string> HamiltonianCycle::createDepotCopies(vector<string> tour){
    vector<string> copiesTour;
    string token{depotCopyToken};
    const string depotId{std::to_string(Globals::customerMap.getDepotId())};

    for(string customer : tour){
        if(!customer.compare(depotId)){
            copiesTour.push_back(customer+token);
            token += depotCopyToken;
        } else {
            copiesTour.push_back(customer);
        }
    }
    return(copiesTour);
}

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::buildChoosenSubs() {
    parentsHamiltonian tours;
    int choosenSize{(int)choosen.size()};
    vector<int> alreadyRebuildRed;
    vector<int> alreadyRebuildBlue;
    int depotId{Globals::customerMap.getDepotId()};

    for(int i{0}; i<choosenSize; i++){
        tours.first.push_back(std::to_string(depotId));
        tours.second.push_back(std::to_string(depotId));

        alreadyRebuildRed.push_back(choosen[i].first);
        for(int redC : redSubs[choosen[i].first]){
            tours.first.push_back(std::to_string(redC));
        }
        alreadyRebuildBlue.push_back(choosen[i].second);
        for(int blueC : blueSubs[choosen[i].second]){
            tours.second.push_back(std::to_string(blueC));
        }
    }

    // Sort desc to delete subTours already used
    std::sort(alreadyRebuildRed.begin(), alreadyRebuildRed.end(), [](int left, int right) { return(left>right); });
    std::sort(alreadyRebuildBlue.begin(), alreadyRebuildBlue.end(), [](int left, int right) { return(left>right); });
    for(unsigned i{0}; i<alreadyRebuildRed.size(); i++){
        redSubs.erase(redSubs.begin()+alreadyRebuildRed[i]);
        blueSubs.erase(blueSubs.begin()+alreadyRebuildBlue[i]);
    }

    if(!redSubs.empty()){
        for(vector<int> red : redSubs){
            tours.first.push_back(std::to_string(depotId));
            for(int redC : red){
                tours.first.push_back(std::to_string(redC));
            }
        }
    }
    if(!blueSubs.empty()){
        for(vector<int> blue : blueSubs){
            tours.second.push_back(std::to_string(depotId));
            for(int blueC : blue){
                tours.second.push_back(std::to_string(blueC));
            }
        }
    }

    return(tours);
}

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::restoreEmptySubtours(parentsHamiltonian parents, int redEmpty, int blueEmpty) {
    string depotId{std::to_string(Globals::customerMap.getDepotId())};
    int redSize{(int)parents.first.size()}, blueSize{(int)parents.second.size()};

    if(redSize != blueSize) {
        int dif{std::abs((int)redSize - (int)blueSize)};
        if(redSize > blueSize) {
            while(dif>0) {
                parents.second.push_back(depotId);
                dif--;
                blueEmpty--;
            }
        } else {
            while(dif>0) {
                parents.first.push_back(depotId);
                dif--;
                redEmpty--;
            }
        }
    }

    if(redEmpty!=0 && blueEmpty!=0) {
        while(redEmpty>0){
            parents.first.push_back(depotId);
            redEmpty--;
        }
        while(blueEmpty>0){
            parents.second.push_back(depotId);
            blueEmpty--;
        }
    }

    return(parents);
}