#pragma once

#include "parser.h"

using namespace std;

Expr* ModusPonens(Expr* e1, Expr* e2);
void ModusPonens_test(string s1, string s2);

Expr* DisjunctiveSyllogism(Expr* e1, Expr* e2, int idx);
void DisjunctiveSyllogism_test(string s1, string s2, int idx);

Expr* AndElim(Expr* e, int idx);
void AndElim_test(string s, int idx);

Expr* AndIntro(vector<Expr*> v);
void AndIntro_test(vector<string> v);

Expr* OrIntro(Expr* e, vector<Expr*> v);
void OrIntro_test(string s, vector<string> v);

Expr* DoubleNegation(Expr* e);
void DoubleNegation_test(string s);

Expr* DML_NotOverOr(Expr* e);
void DML_NotOverOr_test(string s);

Expr* DML_NotOverAnd(Expr* e);
void DML_NotOverAnd_test(string s);