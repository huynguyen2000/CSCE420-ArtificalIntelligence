#pragma once
#include <vector>
#include <string>
#include <fstream>

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
};