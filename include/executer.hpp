#pragma once

#include <string>
#include <vector>

using namespace std;

class Executer {
 public:
  static void execute(const vector<string>& tokens);

 private:
  static string parseOutFile(vector<string>& tokens);
  static bool parseBackground(vector<string>& tokens);
};
