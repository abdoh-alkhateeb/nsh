#include "builtins.hpp"

#include <unistd.h>

#include <cstdlib>
#include <iostream>

using namespace std;

vector<string> Builtins::history;

void Builtins::addToHistory(const string& input) {
  history.push_back(input);
}

bool Builtins::handle(const vector<string>& tokens) {
  if (tokens[0] == "exit") {
    exit(EXIT_SUCCESS);
  } else if (tokens[0] == "cd") {
    size_t argc = tokens.size();
    string path = argc == 1 ? "~" : tokens[1];

    if (argc > 2) {
      cerr << "cd: too many arguments" << endl;
    } else {
      int status = chdir(path == "~" ? getenv("HOME") : path.c_str());

      if (status != 0) {
        string msg = "failed to change directory";
        if (errno == ENOENT) msg = "no such file or directory";
        else if (errno == EACCES) msg = "permission denied";
        cerr << "cd: " << msg << ": " << path << endl;
      }
    }
    return true;
  } else if (tokens[0] == "history") {
    for (size_t i = 0; i < history.size(); i++) {
      cout << i + 1 << "  " << history[i] << endl;
    }
    return true;
  }

  return false;
}
