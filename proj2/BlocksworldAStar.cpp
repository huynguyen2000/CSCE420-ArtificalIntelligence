#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <chrono>

#include "State.h"
#include "Node.h"

using namespace std;

class CustomCompare {
  public:
  bool operator()(Node* lhs, Node* rhs){
    return lhs->get_fn() > rhs->get_fn();
  }
};

Node* BFS(Node* root, State* goal, int mx_iter, ofstream& writefile){
  if (root->goal_test(goal)){
    root->print_path(writefile);
    return root;
  }

  priority_queue<Node*, vector<Node*>, CustomCompare> frontier;
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
    curr_node = frontier.top();
    frontier.pop();
    children = curr_node->successors(*goal);

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
      cout << "iter=" << iter << " | depth=" << path_len << " | max queue size=" << mx_qs << " | num goal tests=" << num_gtest << " | fn=" << curr_node->get_fn() << endl;
      writefile << "iter=" << iter << " | depth=" << path_len << " | max queue size=" << mx_qs << " | num goal tests=" << num_gtest << " | fn=" << curr_node->get_fn() << endl;
    }

    for (int i = 0; i < children.size(); i += 1){
      child = children.at(i);
      num_gtest += 1;
      if (child->goal_test(goal)){
        // found goal: 
        // print success message and summary statistics
        cout << "Success | iter=" << iter << " | depth=" << path_len + 1 << " | max queue size=" << mx_qs << " | num goal tests=" << num_gtest << endl;
        writefile << "Success | iter=" << iter << " | depth=" << path_len + 1 << " | max queue size=" << mx_qs << " | num goal tests=" << num_gtest << " | fn=" << curr_node->get_fn() << endl;
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
  State* goal = new State(goal_state);
  Node* root = new Node(nullptr, tmp, 0, *goal);
  Node* found;

  auto start = chrono::steady_clock::now();
  found = BFS(root, goal, mx_iter, writefile);
  auto end = chrono::steady_clock::now();
  auto diff = end - start;
  cout << chrono::duration<double, milli> (diff).count() << " ms" << endl;

  // don't need to delete root_state because already deletes in Node destructor
  if (root){delete root;}
  if (goal){delete goal;}
  if (found){delete found;}

  writefile.close();
}