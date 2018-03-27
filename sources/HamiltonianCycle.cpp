#include <algorithm>

#include "HamiltonianCycle.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"


bool validaMerdaChoosen(vector<pair<int,int>> merda){

    vector<int> reds;
    vector<int> blues;

    for(auto it : merda){
        reds.push_back(it.first);
        blues.push_back(it.second);
    }
    std::sort(reds.begin(), reds.end());
    std::sort(blues.begin(), blues.end());

    bool redB{std::adjacent_find(reds.begin(), reds.end())==reds.end()};
    bool blueB{std::adjacent_find(blues.begin(), blues.end())==blues.end()};

    return(redB && blueB);
}

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::toHamiltonianCycle(Tour red, Tour blue){
    HamiltonianCycle obj;
    obj.correlationDuplicate = (red.getRoute().size()*0.2);
    obj.correlationIn_Out = obj.correlationDuplicate*3; 

    cout << "EXPLODE"<<endl;
    obj.redSubs = explodeSubTours(red.getRoute(), Configs::customerMap.getDepotId());
    obj.blueSubs = explodeSubTours(blue.getRoute(), Configs::customerMap.getDepotId());

    cout << "GENERATE RANKING"<<endl;
    obj.generateRanking();

    cout << "TOURS_TO_MAP"<<endl;
    obj.choosenToursToMap();


    std::sort(obj.choosen.begin(), obj.choosen.end(), [](auto &left, auto &right){ return(left.first < right.first); });


    cout << "FINAL CHOOSEN" << endl;
    for(auto it : obj.choosen){
        cout << it.first << " " << it.second << endl;
    }


    if(!validaMerdaChoosen(obj.choosen)){
        cout << "DEU MERDA NA CHOOSEN"<<endl;
        exit(0);
    }

    parentsHamiltonian newTours = obj.rebuildTours();

    newTours.first = obj.createDepotCopies(newTours.first);
    newTours.second = obj.createDepotCopies(newTours.second);

    cout << "NewRedTour :";
    for(auto it : newTours.first){
        cout << it << " ";
    }
    cout << endl;
    cout << "NewBlueTour :";
    for(auto it : newTours.second){
        cout << it << " ";
    }
    cout << endl;
    return(newTours);
}

void HamiltonianCycle::generateRanking(){
    unsigned redSubsSize{redSubs.size()}, blueSubsSize{blueSubs.size()};
    choosenSubs choosen;

    for(unsigned red=0; red<redSubsSize; red++){
        for(unsigned blue=0; blue<blueSubsSize; blue++){
            ranking[red].push_back(Correlation(0, 0));
            unsigned redInsideSize{redSubs[red].size()};
            unsigned blueInsideSize{redSubs[blue].size()};

            cout << "SubRed: ";
            for(int c : redSubs[red]){
                cout << c <<" ";
            }cout << endl;
            cout << "SubBlue: ";
            for(int c : blueSubs[blue]){
                cout << c <<" ";
            }cout << endl;

            for(unsigned redElement=0; redElement<redInsideSize; redElement++){
                //cout << "Procurar " << redSubs[red][redElement] << " ";

                auto position = std::distance(blueSubs[blue].begin(), find(blueSubs[blue].begin(), blueSubs[blue].end(), redSubs[red][redElement]));

                if(!(position>=blueInsideSize)){
                    //cout << "achou ";
                    if((position==0 || position==(redInsideSize-1))&&(redElement==0 || redElement==(redInsideSize-1))){
                        //cout << " InOut ";
                        ranking[red][blue].score+=correlationIn_Out;
                        ranking[red][blue].numberInOut++;
                    }
                    ranking[red][blue].score+=correlationDuplicate;
               }
               cout << endl;
            }
            cout << "r " << red << " b " << blue << " " << ranking[red].size()<<endl;
            cout << "SCORE " << ranking[red][blue].score << endl;
            cout << "N_InOut " << ranking[red][blue].numberInOut << endl;
        }
    }
}

void HamiltonianCycle::choosenToursToMap(){
    vector<int> redNotChoosen, blueNotChoosen;
    redNotChoosen.reserve(redSubs.size());
    for(unsigned i=0; i<redSubs.size(); i++){
        redNotChoosen.push_back(i);
    }

    cout << "Ranking size " << ranking.size() << endl;
    blueNotChoosen = redNotChoosen;
    cout << "Antes do print"<<endl;
    for(int red : redNotChoosen){
        for(int blue : blueNotChoosen){
            resolveScore.push_back(ResolveScore(red, blue, ranking[red][blue].score));
        }
    }

    std::sort(resolveScore.begin(), resolveScore.end(), [](auto &left, auto &right){ return(left.score > right.score); });


    cout << "ResolveScore"<<endl;
    for(auto it : resolveScore){
        cout << it.subRed << " " << it.subBlue << " Score " << it.score<<endl;
    }

    for(auto it : resolveScore){
        vector<int>::iterator findRedNotChoosen{find(redNotChoosen.begin(), redNotChoosen.end(), it.subRed)};
        vector<int>::iterator findBlueNotChoosen{find(blueNotChoosen.begin(), blueNotChoosen.end(), it.subBlue)};

        bool inRedNotChoosen{findRedNotChoosen != redNotChoosen.end()};
        bool inBlueNotChoosen{findBlueNotChoosen != blueNotChoosen.end()};

        if(inRedNotChoosen && inBlueNotChoosen){
            choosen.push_back(std::make_pair(it.subRed, it.subBlue));

            blueNotChoosen.erase(findBlueNotChoosen);
            redNotChoosen.erase(findRedNotChoosen);
        }
    }
}

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::rebuildTours(){
    int depotId{Configs::customerMap.getDepotId()};
    parentsHamiltonian tours;

    for(auto itChoosen : choosen){
        tours.first.push_back(std::to_string(depotId));
        tours.second.push_back(std::to_string(depotId));

        for(int redC : redSubs[itChoosen.first]){
            tours.first.push_back(std::to_string(redC));
        }
        for(int blueC : blueSubs[itChoosen.second]){
            tours.second.push_back(std::to_string(blueC));
        }
    }
    return(tours);
}

vector<string> HamiltonianCycle::createDepotCopies(vector<string> tour){
    vector<string> copiesTour;
    string token{depotCopyToken};
    const string depotId{std::to_string(Configs::customerMap.getDepotId())};

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