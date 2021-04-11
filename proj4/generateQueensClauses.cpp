#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char** argv){

  if (argc != 2){
    cout << "Error: wrong number of arguments" << endl;
    return -1;
  }

  int n = atoi(argv[1]);

  ofstream MyFile("KBs/" + to_string(n) + "-queens_KB.txt");

  // only one queen per column
  for (int i = 1; i <= n; i += 1){
    for (int j = 1; j <= n; j += 1){
      for (int k = j+1; k <= n; k += 1){
        MyFile << "(or (not Q" + to_string(i) + to_string(j) + ") (not Q" + to_string(i) + to_string(k) + "))" << endl;
      }
    }
    MyFile << "(or";
    for (int j = 1; j <= n; j += 1){
      MyFile << " Q" << to_string(i) + to_string(j);
    }
    MyFile << ")" << endl;;
  }

  // only one queen per row
  for (int i = 1; i <= n; i += 1){
    for (int j = 1; j <= n; j += 1){
      for (int k = j+1; k <= n; k += 1){
        MyFile << "(or (not Q" + to_string(j) + to_string(i) + ") (not Q" + to_string(k) + to_string(i) + "))" << endl;
      }
    }
    MyFile << "(or";
    for (int j = 1; j <= n; j += 1){
      MyFile << " Q" << to_string(j) + to_string(i);
    }
    MyFile << ")" << endl;
  }

  // diagonals right to left
  vector<vector<string>> g(n * 2 - 1);
  for (int i = 0; i < n; i += 1){
    for (int j = 0; j < n; j += 1){
      g.at(i + j).push_back(to_string(j+1) + to_string(i+1));
    }
  }
  // for (int i = 0; i < g.size(); i += 1){
  //   for (int j = 0; j < g.at(i).size(); j += 1){
  //     cout << g.at(i).at(j) << " ";
  //   }
  //   cout << endl;
  // }
  for (int i = 0; i < g.size(); i += 1){
    if (g.at(i).size() == 1) continue;
    for (int j = 0; j < g.at(i).size(); j += 1){
      for (int k = j+1; k < g.at(i).size(); k += 1){
        MyFile << "(or (not Q" + g.at(i).at(j) + ") (not Q" + g.at(i).at(k) + "))" << endl;
      }
    }
    // MyFile << "(or";
    // for (int j = 0; j < g.at(i).size(); j += 1){
    //   MyFile << " Q" << g.at(i).at(j);
    // }
    // MyFile << ")" << endl;
  }


  // diagonals left to right
  g.clear(); g.resize(n * 2 - 1);
  for (int i = 0; i < n; i += 1){
    for (int j = 0; j < n; j += 1){
      g.at(i + j).push_back(to_string(i+1) + to_string(n-j));
    }
  }
  // for (int i = 0; i < g.size(); i += 1){
  //   for (int j = 0; j < g.at(i).size(); j += 1){
  //     cout << g.at(i).at(j) << " ";
  //   }
  //   cout << endl;
  // }
  for (int i = 0; i < g.size(); i += 1){
    if (g.at(i).size() == 1) continue;
    for (int j = 0; j < g.at(i).size(); j += 1){
      for (int k = j+1; k < g.at(i).size(); k += 1){
        MyFile << "(or (not Q" + g.at(i).at(j) + ") (not Q" + g.at(i).at(k) + "))" << endl;
      }
    }
    // MyFile << "(or";
    // for (int j = 0; j < g.at(i).size(); j += 1){
    //   MyFile << " Q" << g.at(i).at(j);
    // }
    // MyFile << ")" << endl;
  }

  MyFile.close();

  return 0;
}