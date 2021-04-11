#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <stdlib.h>

using namespace std;

class State{
  private:
    vector<vector<char>> blocks;

  public:
    State(string s);
    void print(ofstream& writefile);
    bool match(State* s);
    string hash();
    vector<State*> sucessors();
    float heuristic(State& goal_state);
};