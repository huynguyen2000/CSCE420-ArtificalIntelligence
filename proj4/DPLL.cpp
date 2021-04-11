#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <fstream>

#include "parser.h"

using namespace std;

void print_results(unordered_map<string, bool> model, int num_calls, bool useHeuristic, ofstream &myFile){
  myFile << "------------------------------------------------------------------------" << endl;
  myFile << "Unit clause heauristic: " << (useHeuristic ? "true" : "false") << endl;
  myFile << "Total number of DPLL calls: " << num_calls << endl;
  myFile << "Solution: " << endl;
  for (auto const& pair: model){
    if (pair.second == 1) myFile << pair.first << endl;
  }
}

struct literal {
  string symbol;
  int clause_num;
  bool value;
};

literal findUnitClause(vector<Expr*> &clauses, unordered_set<string> &symbols, unordered_map<string, bool> &model){
  Expr* curr;
  bool clause_value;
  int num_unassigned;
  literal ret;

  for (int i = 0; i < clauses.size(); i += 1){
    clause_value = false;
    num_unassigned = 0;
    curr = clauses[i];
    for (int j = 1; j < clauses.at(i)->sub.size(); j += 1){
      if (num_unassigned > 1 or clause_value) break;

      if (curr->sub.at(j)->kind == ATOM){
        if (symbols.find(curr->sub.at(j)->sym) != symbols.end()){
          // unassigned literal found
          ret.symbol = curr->sub.at(j)->sym;
          ret.clause_num = i;
          ret.value = true;
          num_unassigned += 1;
        }
        else {
          // literal already assigned, find out if true or false
          clause_value = clause_value or model[curr->sub.at(j)->sym];
        }
      }
      else {  // negations
        if (symbols.find(curr->sub.at(j)->sub.at(1)->sym) != symbols.end()){
          ret.symbol = curr->sub.at(j)->sub.at(1)->sym;
          ret.clause_num = i;
          ret.value = false;
          num_unassigned += 1;
        }
        else {
          clause_value = clause_value or !model[curr->sub.at(j)->sub.at(1)->sym];
        }
      }
    }
    if (num_unassigned == 1 and !clause_value){
      return ret;
    }
  }
  ret = {"", -1, NULL};
  return ret;
}

// return boolean if clause is 
int evaluateClause(Expr* &curr, unordered_set<string> &symbols, unordered_map<string, bool> &model){
  bool clause_value = false;
  for (int i = 1; i < curr->sub.size(); i += 1){
    if (curr->sub.at(i)->kind == ATOM){
      if (symbols.find(curr->sub.at(i)->sym) != symbols.end()){
        // unassigned literal found
        return -1;
      }
      else {
        // literal already assigned, find out if true or false
        clause_value = clause_value or model[curr->sub.at(i)->sym];
      }
    }
    else {  // negations
      if (symbols.find(curr->sub.at(i)->sub.at(1)->sym) != symbols.end()){
        return -1;
      }
      else {
        clause_value = clause_value or !model[curr->sub.at(i)->sub.at(1)->sym];
      }
    }
  }
  return clause_value;
}

int DPLL_BaseCase(vector<Expr*> &clauses, unordered_set<string> &symbols, unordered_map<string, bool> &model, ofstream &myFile){
  int true_clauses = 0;
  int clause_value;
  bool false_clause = false;
  for (int i = 0; i < clauses.size(); i += 1){
    clause_value = evaluateClause(clauses.at(i), symbols, model);
    if (clause_value == 1) true_clauses += 1;
    if (clause_value == 0) false_clause = true;
  }
  myFile << "Clauses satisfied: " << true_clauses << endl;
  if (false_clause) return 0;
  if (true_clauses == clauses.size()) return 1;
  return -1;
}

bool DPLL(vector<Expr*> clauses, unordered_set<string> symbols, unordered_map<string, bool> model, bool useHeuristic, int &num_calls, ofstream &myFile){
  num_calls += 1;
  myFile << "------------------------------------------------------------------------" << endl;
  myFile << "Unassigned: ";
  for (auto it = symbols.begin(); it != symbols.end(); ++it){
    myFile << *it << " ";
  } 
  myFile << endl;
  myFile << "Assigned: ";
  for (auto const& pair: model){
    myFile << pair.first << "=" << pair.second << " ";
  }
  myFile << endl;

  // if every clause in clauses is true in model, then return true
  // if some clause in clauses is false in model, then return false
  // if there are unresolved clauses, then delete any true clauses and continue
  int base_case = DPLL_BaseCase(clauses, symbols, model, myFile);
  if (base_case == 0){
    myFile << "Backtracking" << endl;
    return base_case;
  } 
  else if (base_case == 1) {
    myFile << "Found solution" << endl;
    print_results(model, num_calls, useHeuristic, myFile);
    return base_case;
  }

  // find unit clause, force clause to be true (delete clause from clauses, delete P from symbols, add P to model with chosen value), return DPLL with those changes
  if (useHeuristic){
    literal s = findUnitClause(clauses, symbols, model);
    if (s.symbol != ""){
      // if we found a unit literal, then remove clause, remove from symbol, add to model, and propagate (pass by reference because no choice to make)
      myFile << "Forcing " << s.symbol << "=" << s.value << endl;
      symbols.erase(s.symbol);
      model[s.symbol] = s.value;
      return DPLL(clauses, symbols, model, useHeuristic, num_calls, myFile);
    }
  }

  // find first symbol and return DPLL with symbol as true or DPLL with symbol as false
  assert(symbols.size() != 0);
  string chosen_symbol = *symbols.begin();
  symbols.erase(chosen_symbol);

  model[chosen_symbol] = false;
  myFile << "Trying " << chosen_symbol << "=0" << endl; 
  bool chosen_false = DPLL(clauses, symbols, model, useHeuristic, num_calls, myFile);
  if (chosen_false) return true;

  model[chosen_symbol] = true;
  myFile << "Trying " << chosen_symbol << "=1" << endl; 
  bool chosen_true = DPLL(clauses, symbols, model, useHeuristic, num_calls, myFile);
  if (chosen_true) return true;

  return false;
}

int main(int argc, char** argv){
  
  bool useHeuristic = true;
  string filename = "";
  if (argc > 3){
    cout << "Invalid number of arguments" << endl << "usage: DPLL <filename> [-unit]" << endl;
    return -1;
  }
  else{
    filename = argv[1];
    if (argc == 3) useHeuristic = false;
  }

  vector<Expr*> clauses;
  unordered_set<string> symbols;
  unordered_map<string, bool> model;

  try {
    clauses = load_kb(filename);
  }
  catch(...){
    cout << "Error loading file, make sure that file exists" << endl;
    return -1;
  }

  if (filename.find("Map") != string::npos){
    // code for map coloring
    symbols.insert("WAR"); symbols.insert("WAG"); symbols.insert("WAB");
    symbols.insert("NTR"); symbols.insert("NTG"); symbols.insert("NTB");
    symbols.insert("SAR"); symbols.insert("SAG"); symbols.insert("SAB");
    symbols.insert("QR"); symbols.insert("QG"); symbols.insert("QB");
    symbols.insert("NSWR"); symbols.insert("NSWG"); symbols.insert("NSWB");
    symbols.insert("VR"); symbols.insert("VG"); symbols.insert("VB");
    symbols.insert("TR"); symbols.insert("TG"); symbols.insert("TB");
  }
  else if (filename.find("queens") != string::npos){
    // code for n queens
    int n;
    for (int i = 0; i < filename.size(); i += 1){
      if (isalpha(filename.at(i))){
        n = atoi(filename.substr(4, i-4).c_str());
        break;
      }
    }
    for (int i = 1; i <= n; i += 1){
      for (int j = 1; j <= n; j += 1){
        symbols.insert("Q" + to_string(i) + to_string(j));
      }
    }
  }
  else {
    cout << "Error, invalid file" << endl;
    return -1;
  }

  // start DPLL
  ofstream myFile("outputs/" + filename.substr(4, filename.size()-11) + (useHeuristic ? "" : "-U") + "_output.txt");
  int num_calls = 0;
  int ret = DPLL(clauses, symbols, model, useHeuristic, num_calls, myFile);
  if (ret == 0){
    myFile << "------------------------------------------------------------------------" << endl;
    myFile << "No solutions found" << endl;
    myFile << "Total number of DPLL calls: " << num_calls << endl;
  }
  myFile.close();

  return 0;
}