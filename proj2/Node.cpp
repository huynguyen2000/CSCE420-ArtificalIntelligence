#include "Node.h"

Node::Node(Node* p, State* b, int d, State& goal){
  parent = p;
  blocks = b;
  depth = d;
  heuristic = b->heuristic(goal);
  fn = float(depth) + heuristic;
}

Node::~Node(){
  delete blocks;
}

bool Node::goal_test(State* s){
  string h1 = blocks->hash();
  string h2 = s->hash();
  if (h1.compare(h2) != 0){
    return false;
  }
  else{
    return true;
  }
}

// returns a vector of pointers to nodes created on the heap
// each node contains a pointer to states created on the heap
// needs to be deleted afterwards
vector<Node*> Node::successors(State& goal){
  vector<State*> child_states = blocks->sucessors();
  vector<Node*> ret;
  for (int i = 0; i < child_states.size(); i += 1){
    Node* tmp = new Node(this, child_states.at(i), depth + 1, goal);
    ret.push_back(tmp);
  }
  return ret;
}

string Node::hash(){
  return blocks->hash();
}

int Node::print_path(ofstream& writefile){
  if (parent){
    parent->print_path(writefile);
  }
  writefile << "Move " << depth << endl;
  blocks->print(writefile);
  return 0;
}

State* Node::get_blocks(){
  return blocks;
}

int Node::get_depth(){
  return depth;
}

float Node::get_fn(){
  return fn;
}
