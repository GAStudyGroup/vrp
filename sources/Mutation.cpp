#include "Mutation.hpp"

vector<int> Mutation(vector<int>& tour, Map& map){    
    return evaluateMutation(tour,map);
}
vector<int> evaluateMutation(vector<int> tour,Map& map){
  pair<vector<int>,bool> result;
    vector<int> initial = tour;
    int it=0;
    for (auto value: tour){
        for(auto move: getMoves()){
            result=move(initial,map,it,value);
            if(result.second){
                return result.first;
            }
        }
        it++;
    }  
    return tour;
}
//If the node is a client, remove it then insert it after the next
    pair<vector<int>,bool> firstMove(vector<int> tour,Map& map,int it,int value){
    vector<int> initial=tour; 
    pair<vector<int>,bool> result=setInitialResult(tour);
    //cout<<"Fitness antes do movimento "<< iFitness <<endl;
    if(value!=map.getDepotId()){ //Only does the move it isn't a depot   
        for(unsigned int i=0;i<initial.size()-1;i++){
            if(initial[i]==value){
                continue;
            }else{
                initial.erase(find(initial.begin(),initial.end(),value));
                initial.emplace(initial.begin()+i,value);                
            }
            //printVector(initial,map); //Debug
            result=basicFitnessEvaluation(tour,initial,map);
            // printVector(initial,map); //Debug
            if(result.second){
                break;
            }
        }                
    }else{
        result.first=initial;
        result.second=false;
    }
    //cout<<"Fitness depois do movimento "<<bFitness<<endl;
    return result;
}

//Second and third moves move: If u and x are clients, remove them then insert (u; x) after v

pair<vector<int>,bool> secondAndThirdMove(vector<int> tour,Map& map, int it,int value,int mode){
    //cout<<"iterator: " <<it <<endl;
    pair<vector<int>,bool> result=setInitialResult(tour);
    int valU=0;
    int valX=0;
    if(it==map.getDepotId() || it==(int)tour.size()-1){
        return result;
    }else{
        for(unsigned int i=0;i<result.first.size()-1;i++){
                valU=result.first[i];
                valX=result.first[i+1];
                //cout<<"VALU: "<<valU << " VALX: "<< valX<< " IT: "<<it<<endl;
                if(valU==map.getDepotId() || valX==map.getDepotId() || valU==value || valX==value){
                    continue;
                }
                result.first.erase(find(result.first.begin(),result.first.end(),valU));
                result.first.erase(find(result.first.begin(),result.first.end(),valX));
                if(mode==2){
                    result.first.emplace(result.first.begin()+it,valU);
                    result.first.emplace(result.first.begin()+(it+1),valX);    
                }else{
                    result.first.emplace(result.first.begin()+it,valX);
                    result.first.emplace(result.first.begin()+(it+1),valU);    
                }
                
                // /printVector(result.first);
                result=basicFitnessEvaluation(tour,result.first,map);
                if(result.second){
                    return result;
                }
                result.first=tour;
        }
        result.second=false;
        return  result;
    }
}
pair<vector<int>,bool> secondMove(vector<int> tour,Map& map, int it,int value){
    return(secondAndThirdMove(tour,map,it,value,2));
}

pair<vector<int>,bool> thirdMove(vector<int> tour,Map& map, int it,int value){
    return(secondAndThirdMove(tour,map,it,value,3));
}
pair<vector<int>,bool> fourthMove(vector<int> tour,Map& map, int it,int value){
    pair<vector<int>,bool> result=setInitialResult(tour);
    int auxId=0;
    if(value!=map.getDepotId()){
        for(unsigned int i=0; i<tour.size();i++){
            if(result.first[i]!=map.getDepotId() && result.first[i]!=value){
                auxId=result.first[i];
                result.first[i]=value;
                result.first[it]=auxId;
                //printVector(result.first,map);
                result=basicFitnessEvaluation(tour,result.first,map);
                if(result.second){
                    return result;
                }
                result.first=tour;
            }
        
        }
    }
        return result;
}
// If  u; x and v are clients, swap (u; x) and v,

pair<vector<int>,bool> fifthMove(vector<int> tour,Map& map, int it,int value){
    pair<vector<int>,bool> result=setInitialResult(tour);
    if(it<(int)tour.size()-1){
        int valU=0;
        int valX=0;
        if(value!=map.getDepotId()){
            for(unsigned int i=0;i<tour.size()-1;i++){
                valU=result.first[i];
                valX=result.first[i+1];
                if(valU == map.getDepotId() || valX == map.getDepotId() ||valU==value ||valX==value){
                 continue;
                }
                //cout<< "Value U: "<<valU << " Value X: "<<valX<<endl;
                //cout<< "Result: "<<result.first[it]<<endl;

                result.first[it]=-result.first[it];
                
                result.first.emplace(find(result.first.begin(),result.first.end(),valU),value);

                result.first.erase(find(result.first.begin(),result.first.end(),valU));
                result.first.erase(find(result.first.begin(),result.first.end(),valX));

                result.first.emplace(find(result.first.begin(),result.first.end(),-value),valU);
                result.first.emplace(find(result.first.begin(),result.first.end(),-value),valX);

                result.first.erase(find(result.first.begin(),result.first.end(),-value));
                //printVector(result.first,map);
                result=basicFitnessEvaluation(tour,result.first,map);
                if(result.second){
                    return result;
                }
                result.first=tour;
            }
        }
    }
    return result;
}

pair<vector<int>,bool> sixthMove(vector<int> tour,Map& map, int it,int value){
    pair<vector<int>,bool> result=setInitialResult(tour);
    int valU=0,valX=0,valV=0,valY=0;
    if(value!=map.getDepotId() && it<(int)(tour.size()-1)){
        valU=result.first[it];
        valX=result.first[it+1];
        for(unsigned int i=0; i< (tour.size()-1);i++){
            valV=result.first[i];
            valY=result.first[i+1];
            if(valV!=map.getDepotId() && valY!=map.getDepotId() && valX!=map.getDepotId() 
            && valU!=valV && valU!=valX && valU!=valY 
            && valX!=valV && valX!=valY
            && valY!=valV){
                result.first=doubleSwap(result.first,it,i,valV,valY,valU,valX);
                result=basicFitnessEvaluation(tour,result.first,map);
                if(result.second){
                   // cout << "ValU: "<<valU << " ValX: "<<valX<<" ValV: "<<valV<<" ValY: "<<valY<<endl; 
                   // printVector(result.first,map);
                    return result;
                }
                result.first=tour;
            }
        }
    }
    return result;
}

pair<vector<int>,bool> seventhAndEighthAndNineMove(vector<int> tour,Map& map, int it,int value, int move){
    pair<vector<int>,bool> result=setInitialResult(tour);
    int ValU=0,ValX=0,ValV=0,ValY=0;
        if(value==map.getDepotId() || it==(int)tour.size()-1){
            return result;
        }else{
            ValU=result.first[it];
            ValX=result.first[it+1];
            if(ValX!=ValU && ValX!=map.getDepotId()){
                for(unsigned int i=0; i<tour.size()-1;i++){
                    ValV=result.first[i];
                    ValY=result.first[i+1];
                    if(ValX==map.getDepotId()||ValV==map.getDepotId() || ValY==map.getDepotId() || ValU==ValV 
                    ||!(ValU!=ValV && ValU!=ValX && ValU!=ValY && ValX!=ValV && ValX!=ValY && ValY!=ValV)){
                        continue;
                    }
                    if(move==7){
                        if(!checkInSameTour(tour,map.getDepotId(),ValU,ValV)){
                            continue;
                        }
                     }
                     else if(move==8 || move==9){
                        if(checkInSameTour(tour,map.getDepotId(),ValU,ValV)){
                            continue;
                        }
                    }
                    if(move==7 || move==8){
                        result.first=doubleSwap(result.first,it,i,ValU,ValV,ValX,ValY);
                    }
                    else if(move==9){
                        result.first=doubleSwap(result.first,it,i,ValU,ValY,ValX,ValV);
                    }
                    result=basicFitnessEvaluation(tour,result.first,map);
                    if(result.second){
                        return result;
                    }
                }
            }
        }

    return result;
}
pair<vector<int>,bool> seventhMove(vector<int> tour,Map& map, int it,int value){
    return seventhAndEighthAndNineMove(tour,map,it,value,7);
}
pair<vector<int>,bool> eighthMove(vector<int> tour,Map& map, int it,int value){
    return seventhAndEighthAndNineMove(tour,map,it,value,8);
}
pair<vector<int>,bool> ninethMove(vector<int> tour,Map& map, int it,int value){
    return seventhAndEighthAndNineMove(tour,map,it,value,9);
}

vector<pair<vector<int>,bool> (*)(vector<int>,Map&,int,int)> getMoves(){
    vector<pair<vector<int>,bool> (*)(vector<int>,Map&,int,int)> moves;
    moves.push_back(&firstMove);
    moves.push_back(&secondMove);
    moves.push_back(&thirdMove);
    moves.push_back(&fourthMove);
    moves.push_back(&fifthMove);
    moves.push_back(&sixthMove);
    moves.push_back(&seventhMove);
    moves.push_back(&eighthMove);
    moves.push_back(&ninethMove);
    return moves;
}
