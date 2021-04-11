#include "NatDed.h"

using namespace std;

/*
e1: P -> Q
e2: P
------
re: Q
*/
Expr* ModusPonens(Expr* e1, Expr* e2){
  if (
    e1->kind == LIST and
    e1->sub.at(0)->sym.compare("implies") == 0 and
    Eq(e1->sub.at(1), e2)
  ){
    return new Expr(e1->sub.at(2));
  }
  throw RuleApplicationError("Modus Ponens");
  return nullptr;
}
void ModusPonens_test(string s1, string s2){
  cout << "Modus Ponens: " << endl;
  cout << s1 << endl;
  cout << s2 << endl;
  cout << "------------" << endl;
  cout << ModusPonens(parse(s1), parse(s2))->toString() << endl;
}


/*
e1: P v Q
e2: -P if idx = 1
    -Q if idx = 2
------
re: Q if idx = 1
    P if idx = 2
*/
Expr* DisjunctiveSyllogism(Expr* e1, Expr* e2, int idx){
  if (
    e1->kind == LIST and
    e1->sub.at(0)->sym.compare("or") == 0
  ){
    if (idx == 1 and !Eq(e1->sub.at(1), e2)) return new Expr(e1->sub.at(2));
    else if (idx == 2 and !Eq(e1->sub.at(2), e2)) return new Expr(e1->sub.at(1));
  }
  throw RuleApplicationError("Disjunctive Syllogism");
  return nullptr;
}
void DisjunctiveSyllogism_test(string s1, string s2, int idx){
  cout << "Disjunctive Syllogism: " << endl;
  cout << s1 << endl;
  cout << s2 << endl;
  cout << "------------" << endl;
  cout << DisjunctiveSyllogism(parse(s1), parse(s2), idx)->toString() << endl;
}


/*
e1: P0 ^ P1 ^ P2 ^ ...
------
re: Pi
*/
Expr* AndElim(Expr* e, int idx){
  if (
    e->kind == LIST and
    e->sub.at(0)->sym.compare("and") == 0 and
    idx > 0 and
    idx < e->sub.size()
  ){
    return new Expr(e->sub.at(idx));
  }
  throw RuleApplicationError("And Elimination");
  return nullptr;
}
void AndElim_test(string s, int idx){
  cout << "And Elimination: " << endl;
  cout << s << endl;
  cout << "------------" << endl;
  cout << AndElim(parse(s), idx)->toString() << endl;
}


/*
v0: P0
v1: P1
v2: P2
...
------
re: P0 ^ P1 ^ P2 ^ ...
*/
Expr* AndIntro(vector<Expr*> v){
  string s = "(and ";
  for (int i = 0; i < v.size(); i += 1){
    s += v.at(i)->toString() + (i == v.size()-1 ? ")" : " ");
  }
  return parse(s);
}
void AndIntro_test(vector<string> v){
  cout << "And Introduction: " << endl;
  vector<Expr*> v_e;
  for (int i = 0; i < v.size(); i += 1){
    cout << v.at(i) << endl;
    v_e.push_back(parse(v.at(i)));
  }
  cout << "------------" << endl;
  cout << AndIntro(v_e)->toString() << endl;
}


/*
e1: P0
e2: [P1, P2, ...]
------
re: P0 v P1 v P2 v ...
*/
Expr* OrIntro(Expr* e, vector<Expr*> v){
  string s = "(or ";
  s += e->toString() + " ";
  for (int i = 0; i < v.size(); i += 1){
    s += v.at(i)->toString() + (i == v.size()-1 ? ")" : " ");
  }
  return parse(s);
}
void OrIntro_test(string s, vector<string> v){
  cout << "Or Introduction: " << endl;
  vector<Expr*> v_e;
  cout << s << endl;
  for (int i = 0; i < v.size(); i += 1){
    cout << v.at(i) << endl;
    v_e.push_back(parse(v.at(i)));
  }
  cout << "------------" << endl;
  cout << OrIntro(parse(s), v_e)->toString() << endl;
}


/*
e1: --P0
------
re: P0
*/
Expr* DoubleNegation(Expr* e){
  if (
    e->kind == LIST and
    e->sub.at(0)->sym.compare("not") == 0 and
    e->sub.at(1)->kind == LIST and
    e->sub.at(1)->sub.at(0)->sym.compare("not") == 0
  ){
    return new Expr(e->sub.at(1)->sub.at(1));
  }
  throw RuleApplicationError("Double Negation");
  return nullptr;
}
void DoubleNegation_test(string s){
  cout << "Double Negation: " << endl;
  cout << s << endl;
  cout << "------------" << endl;
  cout << DoubleNegation(parse(s))->toString() << endl;
}


/*
e1: -(P v Q)
------
re: -P ^ -Q
*/
Expr* DML_NotOverOr(Expr* e){
  if (
    e->kind == LIST and
    e->sub.at(0)->sym.compare("not") == 0 and
    e->sub.at(1)->kind == LIST and
    e->sub.at(1)->sub.at(0)->sym.compare("or") == 0
  ){
    string p = e->sub.at(1)->sub.at(1)->toString();
    string q = e->sub.at(1)->sub.at(2)->toString();
    return parse("(and (not " + p + ") (not " + q + "))");
  }
  throw RuleApplicationError("De Morgan's Law, Not over Or");
  return nullptr;
}
void DML_NotOverOr_test(string s){
  cout << "De Morgan's Law, Not over Or: " << endl;
  cout << s << endl;
  cout << "------------" << endl;
  cout << DML_NotOverOr(parse(s))->toString() << endl;
}


/*
e1: -(P ^ Q)
------
re: -P v -Q
*/
Expr* DML_NotOverAnd(Expr* e){
  if (
    e->kind == LIST and
    e->sub.at(0)->sym.compare("not") == 0 and
    e->sub.at(1)->kind == LIST and
    e->sub.at(1)->sub.at(0)->sym.compare("and") == 0
  ){
    string p = e->sub.at(1)->sub.at(1)->toString();
    string q = e->sub.at(1)->sub.at(2)->toString();
    return parse("(or (not " + p + ") (not " + q + "))");
  }
  throw RuleApplicationError("De Morgan's Law, Not over And");
  return nullptr;
}
void DML_NotOverAnd_test(string s){
  cout << "De Morgan's Law, Not over And: " << endl;
  cout << s << endl;
  cout << "------------" << endl;
  cout << DML_NotOverAnd(parse(s))->toString() << endl;
}