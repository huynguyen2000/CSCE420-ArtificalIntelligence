#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "State.h"

using namespace std;

class Node{
  private:
    State* blocks;
    Node* parent;
    int depth;
    
  public:
    Node(Node* p, State* b, int d);
    ~Node();
    bool goal_test(State* s);
    vector<Node*> successors();
    string hash();
    int print_path(ofstream& writefile);
    State* get_blocks();
    int get_depth();
};