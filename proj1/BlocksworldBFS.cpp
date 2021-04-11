#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>

#include "State.h"
#include "Node.h"

using namespace std;

Node* BFS(Node* root, State* goal, int mx_iter, ofstream& writefile){
  if (root->goal_test(goal)){
    root->print_path(writefile);
    return root;
  }

  queue<Node*> frontier;
  unordered_map<string, Node*> reached;

  Node* curr_node;
  Node* child;
  vector<Node*> children;

  int iter = 1;
  int path_len = 0;
  int mx_qs = 0;
  int num_gtest = 0;

  frontier.push(root);
  reached[root->hash()] = root;

  while (frontier.size() != 0){
    curr_node = frontier.front();
    frontier.pop();
    children = curr_node->successors();

    path_len = max(path_len, curr_node->get_depth());
    iter += 1;
    if (iter == mx_iter){
      // delete all nodes except root
      for (auto j : reached){
        if (j.first.compare(root->hash()) != 0){
          delete j.second;
        }
      }
      // failure message
      writefile << "Failure, max iterations reached | iter=" << iter << " | depth=" << path_len << " | max queue size=" << mx_qs << " | num goal tests=" << num_gtest << endl;
      return nullptr;
    }
    else if (iter % 1000 == 0){
      writefile << "iter=" << iter << " | depth=" << path_len << " | max queue size=" << mx_qs << " | num goal tests=" << num_gtest << endl;
    }

    for (int i = 0; i < children.size(); i += 1){
      child = children.at(i);
      num_gtest += 1;
      if (child->goal_test(goal)){
        // found goal: 
        // print success message and summary statistics
        writefile << "Success | iter=" << iter << " | depth=" << path_len + 1 << " | max queue size=" << mx_qs << " | num goal tests=" << num_gtest << endl;
        // print the path
        child->print_path(writefile);
        // delete all nodes using reached (except root and found goal node, don't add found goal node to reached)
        for (auto j : reached){
          if (j.first.compare(root->hash()) != 0){
            delete j.second;
          }
        }
        // delete all current children not reached yet
        for (int j = children.size() - 1; i < j; j -= 1){
          delete children.at(j);
        }
        // return the node
        return child;
      }
      if (reached.find(child->hash()) == reached.end()){
        reached[child->hash()] = child;
        frontier.push(child);
        mx_qs = max(mx_qs, int(frontier.size()));
      }
    }    
  }
  // delete all nodes except root
  for (auto j : reached){
    if (j.first.compare(root->hash()) != 0){
      delete j.second;
    }
  }
  // failure message
  writefile << "Failure, goal node could not be found | iter=" << iter << " | depth=" << path_len << " | max queue size=" << mx_qs << " | num goal tests=" << num_gtest << endl;
  return nullptr;
}

int main(int argc, char** argv){
  int s, b, n;
  int mx_iter = stoi(argv[2]);
  string root_state = "";
  string goal_state = "";
  string line;
  string filename = argv[1];
  ifstream readfile(filename);

  getline(readfile, line);
  stringstream ss(line);
  ss >> line;
  s = stoi(line);
  ss >> line;
  b = stoi(line);
  ss >> line;
  n = stoi(line);

  getline(readfile, line);

  for (int i = 0; i < s; i += 1){
    root_state += '|';
    getline(readfile, line);
    root_state += line;
  }

  getline(readfile, line);

  for (int i = 0; i < s; i += 1){
    goal_state += '|';
    getline(readfile, line);
    goal_state += line;
  }

  getline(readfile, line);

  readfile.close();

  ofstream writefile(filename.substr(0, filename.size()-3) + "out", ofstream::trunc);

  State* tmp = new State(root_state);
  Node* root = new Node(nullptr, tmp, 0);
  State* goal = new State(goal_state);
  Node* found;
  found = BFS(root, goal, mx_iter, writefile);
  // don't need to delete root_state because already deletes in Node destructor
  if (root){delete root;}
  if (goal){delete goal;}
  if (found){delete found;}

  writefile.close();
}