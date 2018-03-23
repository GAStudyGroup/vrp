#ifndef MUTATION_H
#define MUTATION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>
#include "Tour.hpp"
using namespace std;
class Mutation{
public:
    vector<int> evaluateMutation(vector<int>);
    bool validateTour(vector<int>,int,int);
private:
    //If the node is a client, remove it then insert it after the next
    pair<vector<int>,bool> firstMove(vector<int>, int,int);
    //Second and third moves move: If u and x are clients, remove them then insert (u; x) after v
    pair<vector<int>,bool> secondAndThirdMove(vector<int>, int,int,int);
    //Insert like (u,x)
    pair<vector<int>,bool> secondMove(vector<int>, int,int);
    //Insert like (x,u)
    pair<vector<int>,bool> thirdMove(vector<int>, int,int);
    //Does a simple swap between two clients
    pair<vector<int>,bool> fourthMove(vector<int>, int,int);
    // If  u; x and v are clients, swap (u; x) and v,
    pair<vector<int>,bool> fifthMove(vector<int>, int,int);
    //Realizes a double swap between two clients
    pair<vector<int>,bool> sixthMove(vector<int>, int,int);
    //Extends the 2-opt to the VRP
    pair<vector<int>,bool> seventhAndEighthAndNineMove(vector<int>,int,int,int);

    pair<vector<int>,bool> seventhMove(vector<int>,int,int);

    pair<vector<int>,bool> eighthMove(vector<int>,int,int);

    pair<vector<int>,bool> ninethMove(vector<int>,int,int);
    //Checks if two clients are in the same tours
    bool checkInSameTour(vector<int>, int,int,int);
    //Evaluates the fitness inside every move
    pair<vector<int>,bool> basicFitnessEvaluation(Tour ,Tour);  
    //Creates a empty result pair
    pair<vector<int>,bool> setInitialResult(vector<int>);

    vector<int> doubleSwap(vector<int>,int currentValue,int iterationValue,
        int newValueU, int newValueX,int newValueV,int newValueY);
//vector<pair<vector<int>,bool> (*)(vector<int>,int,int)> getMoves();
};


#endif