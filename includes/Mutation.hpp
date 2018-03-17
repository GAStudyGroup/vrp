#ifndef MUTATION_H
#define MUTATION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>

#include "Map.hpp"
#include "Utils.hpp"
#include "MutationUtils.hpp"
using namespace std;

vector<int> Mutation(vector<int>& , Map&);

pair<vector<int>,bool> firstMove(vector<int>, Map&,int,int);

pair<vector<int>,bool> secondAndThirdMove(vector<int>,Map&, int,int,int);

pair<vector<int>,bool> secondMove(vector<int>,Map&, int,int);

pair<vector<int>,bool> thirdMove(vector<int>,Map&, int,int);

pair<vector<int>,bool> fourthMove(vector<int>,Map&, int,int);

pair<vector<int>,bool> fifthMove(vector<int>,Map&, int,int);

pair<vector<int>,bool> sixthMove(vector<int>,Map&, int,int);

pair<vector<int>,bool> seventhAndEighthAndNineMove(vector<int>,Map&,int,int,int);

pair<vector<int>,bool> seventhMove(vector<int>,Map&,int,int);

pair<vector<int>,bool> eighthMove(vector<int>,Map&,int,int);

pair<vector<int>,bool> ninethMove(vector<int>,Map&,int,int);

vector<int> evaluateMutation(vector<int>,Map&);

vector<pair<vector<int>,bool> (*)(vector<int>,Map&,int,int)> getMoves();


#endif