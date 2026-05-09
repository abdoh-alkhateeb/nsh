#include <iostream>

#include "builtins.hpp"
#include "executer.hpp"
#include "parser.hpp"

using namespace std;

int main() {
  while (true) {
    cout << "nsh> " << flush;
    string input;

    if (!getline(cin, input)) {
      break;
    }

    if (input.empty()) {
      continue;
    }

    if (input == "exit") {
      cout << "Goodbye!" << endl;
      break;
    }

    Builtins::addToHistory(input);

    vector<string> tokens = Parser::parseInput(input);
    Executer::execute(tokens);
  }
}
