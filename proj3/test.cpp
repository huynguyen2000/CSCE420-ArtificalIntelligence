#include "NatDed.h"

using namespace std;

int main(){

  ModusPonens_test("(implies P Q)", "P");
  cout << endl;
  DisjunctiveSyllogism_test("(or (not P) Q)", "P", 1);
  cout << endl;
  AndElim_test("(and P0 P1 P2)", 2);
  cout << endl;
  AndIntro_test(vector<string>({"P0", "P1", "P2"}));
  cout << endl;
  OrIntro_test("P0", vector<string>({"P1", "P2"}));
  cout << endl;
  DoubleNegation_test("(not (not P))");
  cout << endl;
  DML_NotOverOr_test("(not (or P Q))");
  cout << endl;
  DML_NotOverAnd_test("(not (and P Q))");

  return 0;
}