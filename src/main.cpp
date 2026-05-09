#include <iostream>

#include "executer.hpp"
#include "parser.hpp"
#include "builtins.hpp"

using namespace std;

int main() {
  while (true) {
    cout << "nsh> ";
    string input;
    getline(cin, input);

    if (input.empty()) {
      continue;
    }

    Builtins::addToHistory(input);

    vector<string> tokens = Parser::parseInput(input);
    Executer::execute(tokens);
  }
}
