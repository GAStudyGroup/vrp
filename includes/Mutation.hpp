#ifndef MUTATION_H
#define MUTATION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>

using namespace std;
class Mutation{
public:

    pair<vector<int>,bool> firstMove(vector<int>, int,int);

    pair<vector<int>,bool> secondAndThirdMove(vector<int>, int,int,int);

    pair<vector<int>,bool> secondMove(vector<int>, int,int);

    pair<vector<int>,bool> thirdMove(vector<int>, int,int);

    pair<vector<int>,bool> fourthMove(vector<int>, int,int);

    pair<vector<int>,bool> fifthMove(vector<int>, int,int);

    pair<vector<int>,bool> sixthMove(vector<int>, int,int);

    pair<vector<int>,bool> seventhAndEighthAndNineMove(vector<int>,int,int,int);

    pair<vector<int>,bool> seventhMove(vector<int>,int,int);

    pair<vector<int>,bool> eighthMove(vector<int>,int,int);

    pair<vector<int>,bool> ninethMove(vector<int>,int,int);

    vector<int> evaluateMutation(vector<int>);

//vector<pair<vector<int>,bool> (*)(vector<int>,int,int)> getMoves();
};


#endif