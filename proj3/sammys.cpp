#include <iostream>
#include <fstream>

#include "parser.h"
#include "NatDed.h"

using namespace std;

int main(){

  vector<Expr*> KB = load_kb("sammys.kb");
  KB.push_back(ModusPonens(KB[10], KB[2]));
  KB.push_back(ModusPonens(KB[20], KB[5]));
  KB.push_back(DisjunctiveSyllogism(KB[30], KB[31], 2));
  KB.push_back(ModusPonens(KB[27], KB[32]));
  KB.push_back(ModusPonens(KB[6], KB[0]));
  KB.push_back(AndElim(KB[33], 2));
  KB.push_back(DisjunctiveSyllogism(KB[34], KB[35], 1));
  KB.push_back(ModusPonens(KB[9], KB[1]));
  KB.push_back(ModusPonens(KB[23], KB[36]));
  KB.push_back(AndElim(KB[38], 1));
  KB.push_back(DisjunctiveSyllogism(KB[37], KB[39], 2));

  ofstream out("sammys.txt");
  streambuf *coutbuf = cout.rdbuf();
  cout.rdbuf(out.rdbuf());
  
  show_kb(KB);

  cout.rdbuf(coutbuf);

  return 0;
}