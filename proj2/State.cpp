#include "State.h"

State::State(string s){
  int c = -1;
  for (int i = 0; i < s.size(); i += 1){
    if (s.at(i) == '|'){
      blocks.push_back(vector<char>());
      c += 1;
    } 
    else {
      blocks.at(c).push_back(s.at(i));
    }
  }
}

void State::print(ofstream& writefile){
  for (int i = 0; i < blocks.size(); i += 1){
    for (int j = 0; j < blocks.at(i).size(); j += 1){
      writefile << blocks.at(i).at(j);
    }
    writefile << endl;
  }
  writefile << ">>>>>>>>>>" << endl;
}

bool State::match(State* s){
  string h1 = hash();
  string h2 = s->hash();
  if (h1.compare(h2) != 0){
    return false;
  }
  else{
    return true;
  }
}

string State::hash(){
  string ret = "";
  for (int i = 0; i < blocks.size(); i += 1){
    ret += '|';
    for (int j = 0; j < blocks.at(i).size(); j += 1){
      ret += blocks.at(i).at(j);
    }
  }
  return ret;
}

// returns a vector of pointers to States initialized on the heap
// have to delete afterwards
vector<State*> State::sucessors(){
  vector<State*> ret;
  string tmp_string;
  State* tmp_state;
  
  // what block we're moving
  for (int i = 0; i < blocks.size(); i += 1){
    
    if (blocks.at(i).size() == 0){
      continue;
    }

    // where we are moving it
    for (int j = 0; j < blocks.size(); j += 1){

      // don't move it to where it was
      if (j == i){
        continue;
      }

      tmp_string = "";

      // begin constructing the string
      for (int k = 0; k < blocks.size(); k += 1){

        tmp_string += '|';

        if (k == i){

          for (int l = 0; l < blocks.at(k).size() - 1; l += 1){
            tmp_string += blocks.at(k).at(l);
          } 

        }

        else{

          for (int l = 0; l < blocks.at(k).size(); l += 1){
            tmp_string += blocks.at(k).at(l);
          } 

          if (k == j){
            tmp_string += blocks.at(i).back();
          }
          
        }

      }

      tmp_state = new State(tmp_string);
      ret.push_back(tmp_state);

    }

  }
  
  return ret;
}

float State::heuristic(State& goal_state){

  // heuristic 0: trivial, will always return 0
  float ret = 0.0;
  
  // heuristic 1: +1 for each block out of position
  // for (int i = 0; i < blocks.size(); i += 1){
  //   for (int j = 0; j < blocks.at(i).size(); j += 1){
  //     try {
  //       if (blocks.at(i).at(j) != goal.blocks.at(i).at(j)) ret += 1;
  //     }
  //     catch (const out_of_range& e){
  //       ret += 1;
  //     }
  //   }
  // }
  
  // heuristic 2:
  unordered_map<char, unordered_map<string, char>> curr_info;
  unordered_map<char, unordered_map<string, char>> goal_info;
  unordered_map<string, char> tmp;

  vector<vector<char>> curr(blocks);
  vector<vector<char>> goal(goal_state.blocks);

  for (int i = 0; i < curr.size(); i += 1){
    for (int j = 0; j < curr.at(i).size(); j += 1){
      if (j == 0){
        tmp.clear();
        tmp["below"] = '|';
        tmp["move"] = '0';
        tmp["stack"] = to_string(i)[0];
        curr_info[curr.at(i).at(j)] = tmp;
      }
      else{
        tmp.clear();
        tmp["below"] = curr.at(i).at(j-1);
        tmp["move"] = '0';
        tmp["stack"] = to_string(i)[0];
        curr_info[curr.at(i).at(j)] = tmp;
      }
    }
  }

  for (int i = 0; i < goal.size(); i += 1){
    for (int j = 0; j < goal.at(i).size(); j += 1){
      if (j == 0){
        tmp.clear();
        tmp["below"] = '|';
        tmp["move"] = '0';
        tmp["stack"] = to_string(i)[0];
        goal_info[goal.at(i).at(j)] = tmp;
      }
      else{
        tmp.clear();
        tmp["below"] = goal.at(i).at(j-1);
        tmp["move"] = '0';
        tmp["stack"] = to_string(i)[0];
        goal_info[goal.at(i).at(j)] = tmp;
      }
    }
  }

  bool bad;
  for (int i = 0; i < curr.size(); i += 1){
    bad = false;
    for (int j = 0; j < curr.at(i).size(); j += 1){
      if (curr_info[curr.at(i).at(j)]["below"] != goal_info[curr.at(i).at(j)]["below"]){
        if (bad) curr_info[curr[i][j]]["move"] = '2';
        else{
          curr_info[curr.at(i).at(j)]["move"] = '1';
          if (i != (int)goal_info[curr.at(i).at(j)]["stack"] - 48){
            for (int k = 0; k < curr.at((int)goal_info[curr.at(i).at(j)]["stack"] - 48).size(); k += 1){
              if (goal_info[curr.at((int)goal_info[curr.at(i).at(j)]["stack"] - 48).at(k)]["stack"] == i){
                curr_info[curr.at(i).at(j)]["move"] = '2';
                break;
              }
            }
          }
        }
      }
      else{
        if (curr_info[curr.at(i).at(j)]["below"] == '|');
        else if (curr_info[curr_info[curr.at(i).at(j)]["below"]]["move"] != '0'){
          curr_info[curr.at(i).at(j)]["move"] = '2';
        }
      }
    }
  }

  for (auto i = curr_info.begin(); i != curr_info.end(); ++i){
    ret += (float)((int)i->second["move"] - 48);
  }

  return ret;
}