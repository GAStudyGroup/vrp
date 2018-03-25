#include <algorithm>

#include "HamiltonianCycle.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"

HamiltonianCycle::correlationMap ranking;
string depotCopyToken{"'"};

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::toHamiltonianCycle(Tour red, Tour blue){
    vector<vector<int>> redSubs{explodeSubTours(red.getRoute(), Configs::customerMap.getDepotId())};
    vector<vector<int>> blueSubs{explodeSubTours(blue.getRoute(), Configs::customerMap.getDepotId())};

    choosenSubs choosen = mapDepots(redSubs, blueSubs);
    cout << "PreChoose"<<endl;
    for(auto it : choosen){
        cout << "Red " << it.first << " Blue " << it.second << endl;
    }
    
    choosen = validateChoosenSubs(choosen);

    exit(EXIT_FAILURE);

    parentsHamiltonian newTours = rebuildTours(redSubs, blueSubs, choosen);

    newTours.first = createDepotCopies(newTours.first);
    newTours.second = createDepotCopies(newTours.second);

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

// Will generate a list of pre selected combinations of subTours
HamiltonianCycle::choosenSubs HamiltonianCycle::mapDepots(subTours& redSubs, subTours& blueSubs){
    unsigned redSubsSize{redSubs.size()}, blueSubsSize{blueSubs.size()};

    choosenSubs preChoose;

    cout << "Size " << blueSubsSize << endl;
    for(unsigned r=0; r<redSubsSize; r++){
        cout << "---- RED ----" << endl;
        for(unsigned b=0; b<blueSubsSize; b++){
            ranking[r].push_back(Correlation(0, 0));
            unsigned redInsideSize{redSubs[r].size()}; 
            unsigned blueInsideSize{blueSubs[b].size()};
            cout << "SubRed: ";
            for(int c : redSubs[r]){
                cout << c <<" ";
            }cout << endl;
            cout << "SubBlue: ";
            for(int c : blueSubs[b]){
                cout << c <<" ";
            }
            cout << endl;
            for(unsigned rInside=0; rInside<redInsideSize; rInside++){
                cout << "Procurar " << redSubs[r][rInside] << " ";
                // return the position of element in vector, if return == size, the element not found in vector
                auto position = std::distance(blueSubs[b].begin(), std::find(blueSubs[b].begin(), blueSubs[b].end(), redSubs[r][rInside]));
                if(!(position>=blueInsideSize)){
                    cout << "achou ";
                    if((position==0 || position==(redInsideSize-1))&&(rInside==0 || rInside==(redInsideSize-1))){
                        cout << " InOut ";
                        ranking[r][b].score+=5;
                        ranking[r][b].numberInOut++;
                    }
                    ranking[r][b].score+=2;
               }
               cout << endl;
            }
            cout << "r " << r << " b " << b << " " << ranking[r].size()<<endl;
            cout << "SCORE " << ranking[r][b].score << endl;
            cout << "N_InOut " << ranking[r][b].numberInOut << endl;
        }
        int countB{0}, maxScore{0}; 
        preChoose.push_back(std::make_pair(-1, -1));
        for(auto it : ranking[r]){
            if(maxScore < it.score){
                maxScore = it.score;
                preChoose[r].first=r;
                preChoose[r].second=countB;
            }
            countB++;
        }
    }
    return(preChoose);
}

// Will validate chooses of Map, changing if necessary
HamiltonianCycle::choosenSubs HamiltonianCycle::validateChoosenSubs(choosenSubs& preChoosen){
    choosenSubs equals;
    choosenSubs newChoosen;
    choosenSubs old{preChoosen};

    cout << "----------- CHOOSEN ----------" << endl;
    for(auto it : old){
        cout <<"First " << it.first << " Second " << it.second << endl;
        equals = findEqualsChooses(preChoosen, it.second);
        cout << "PreSize" << preChoosen.size()<<endl;
        cout << equals.size() << endl;
        if(equals.size()>1){
            cout << "Equals: " << equals.size() << endl;
            vector<pair<pair<int,int>, int>> scores;
            for(auto eq : equals){
                scores.push_back(std::make_pair(std::make_pair(eq.first, eq.second), ranking[eq.first][eq.second].score));
            }
            cout << "Sort"<<endl;
            std::sort(scores.begin(), scores.end(), [](auto &right, auto &left){ return(left.second<right.second); });
            cout << "scores ";
            if(!scores.empty()){
                for(auto c : scores){
                    cout << c.second << " ";
                }
                cout << endl;
            }
            /* REEEEEEVEEEEER */
            // Get a list of first best repeated elements
            auto it = std::adjacent_find(scores.begin(), scores.end());
            if(it!=scores.end() && (*it).second == scores.front().second){
                unsigned i{0};
                int maxNinOut{0}, subRedMaxNinOut{0}, subBlueMaxNinOut{0};
                while((*it).second == scores[i].second){ // caso onde as notas sejam repetidas
                    int thisInOut{ranking[scores[i].first.first][scores[i].first.second].numberInOut};
                    if(maxNinOut < thisInOut){
                        maxNinOut = thisInOut;
                        subRedMaxNinOut = scores[i].first.first;
                        subBlueMaxNinOut = scores[i].first.second;
                    }
                }
                newChoosen.push_back(std::make_pair(subRedMaxNinOut, subBlueMaxNinOut));
                /* REEEEEEVEEEEER */
            }else {
                newChoosen.push_back(std::make_pair(scores.front().first.first, scores.front().first.second));
            }
            
        } else {
            if(!equals.empty()) newChoosen.push_back(equals[0]);
        }
    }
    cout << "NewChoosen" << endl;
    for(auto it : newChoosen){
        cout << it.first << " " << it.second << endl;
    }
    return(newChoosen);
}

// Rebuild tour with subs Chooses
HamiltonianCycle::parentsHamiltonian HamiltonianCycle::rebuildTours(subTours& redTours, subTours& blueTours, choosenSubs& choosen) {
    int depotId{Configs::customerMap.getDepotId()};
    parentsHamiltonian tours;

    cout << "RedSubs"<<endl;
    for(auto sub : redTours){
        for(auto c : sub) {
            cout << c << " ";
        }
        cout << endl;
    }
    cout << "BlueSubs"<<endl;
    for(auto sub : blueTours){
        for(auto c : sub) {
            cout << c << " ";
        }
        cout << endl;
    }

    cout << "Choosen"<<endl;
    for(auto it : choosen){
        tours.first.push_back(std::to_string(depotId));
        tours.second.push_back(std::to_string(depotId));
        for(int c : redTours[it.first]){
            tours.first.push_back(std::to_string(c));
        }
        for(int c : blueTours[it.second]){
            tours.second.push_back(std::to_string(c));
        }
    }
    for(auto it : choosen){
        redTours.erase(redTours.begin()+it.first);
        blueTours.erase(blueTours.begin()+it.second);
    }
    if(!redTours.empty() && !blueTours.empty()){
        for(auto it : redTours){
            tours.first.push_back(std::to_string(depotId));
            for(int c : it){
                tours.first.push_back(std::to_string(c));
            }
        }
        for(auto it : blueTours){
            tours.second.push_back(std::to_string(depotId));
            for(int c : it){
                tours.second.push_back(std::to_string(c));
            }
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

// return a list with equals combinations of red->blue
HamiltonianCycle::choosenSubs HamiltonianCycle::findEqualsChooses(choosenSubs& choosen, int element){
    choosenSubs equals;
    unsigned size{choosen.size()};
    vector<int> pos;
    for(unsigned i=0; i<size; i++){
        if(choosen[i].second == element){
            pos.push_back(i);
            equals.push_back(choosen[i]);
        }
    }
    for(unsigned i=pos.size(); i>0; i--){
        cout << "Pos " << pos[i-1] << endl;
        choosen.erase(choosen.begin()+pos[i-1]);
    }
    return(equals);
}