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