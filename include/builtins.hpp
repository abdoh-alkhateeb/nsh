#pragma once

#include <string>
#include <vector>

using namespace std;

class Builtins {
 public:
  static bool handle(const vector<string>& tokens);
  static void addToHistory(const string& input);

 private:
  static vector<string> history;
};
