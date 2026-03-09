#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens) {
    if (Builtins::handle(tokens)) return;

    std::vector<const char *> argv; //arraylist or vector of c strings,

    for (const std::string &token : tokens)
        argv.push_back(token.c_str()); // convert vecto of strings to c strings
    argv.push_back(nullptr); // \0 char

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) { // child proces
        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));
        if (status != 0) {
            std::string msg = "failed to execute command";
            if (errno == ENOENT)
                msg = "command not found";
            std::cerr << tokens[0] << ": " << msg << std::endl;
        }
    }
    else // parent process (pid > 0)
        waitpid(pid, nullptr, 0);
}
