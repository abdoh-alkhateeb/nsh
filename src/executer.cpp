#include "executer.hpp"

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

#include "builtins.hpp"

using namespace std;

string Executer::parseOutFile(vector<string>& tokens) {
  for (size_t i = 0; i < tokens.size(); i++) {
    if (tokens[i] == ">") {
      if (i + 1 < tokens.size()) {
        string outFile = tokens[i + 1];
        tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
        return outFile;
      }
    }
  }
  return "";
}

bool Executer::parseBackground(vector<string>& tokens) {
  if (!tokens.empty() && tokens.back() == "&") {
    tokens.pop_back();
    return true;
  }
  return false;
}

void Executer::execute(const vector<string>& tokens) {
  vector<string> args = tokens;

  bool background = parseBackground(args);
  string outFile = parseOutFile(args);

  if (Builtins::handle(args)) {
    return;
  }

  vector<const char*> argv;
  for (const string& token : args) {
    argv.push_back(token.c_str());
  }
  argv.push_back(nullptr);

  pid_t pid = fork();

  if (pid < 0) {
    cerr << args[0] << ": failed to execute command" << endl;
  } else if (pid == 0) {
    if (!outFile.empty()) {
      int fd = open(outFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0) {
        cerr << "nsh: failed to open file: " << outFile << endl;
        exit(1);
      }
      dup2(fd, STDOUT_FILENO);
      close(fd);
    }

    int status = execvp(argv[0], const_cast<char* const*>(argv.data()));

    if (status != 0) {
      string msg = "failed to execute command";
      if (errno == ENOENT) msg = "command not found";
      cerr << args[0] << ": " << msg << endl;
    }
    exit(1);
  } else {
    if (background) {
      cout << "[" << pid << "] running in background" << endl;
    } else {
      waitpid(pid, nullptr, 0);
    }
  }
}
