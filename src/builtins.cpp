#include "builtins.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <errno.h>      // for errno
#include <string.h>     // for strerror? Actually we use errno constants
#include <vector>

// Define the static history vector
std::vector<std::string> Builtins::history;

void Builtins::addToHistory(const std::string &cmd)
{
    // Only add non‑empty commands
    if (!cmd.empty())
        history.push_back(cmd);
}

void Builtins::printHistory()
{
    // Print each command with a line number
    for (size_t i = 0; i < history.size(); ++i) {
        std::cout << i + 1 << "  " << history[i] << std::endl;
    }
}

bool Builtins::handle(const std::vector<std::string> &tokens)
{
    if (tokens.empty()) return false;   // safety

    if (tokens[0] == "exit") {
        exit(EXIT_SUCCESS);
    }
    else if (tokens[0] == "cd") {
        // ... existing cd code (unchanged) ...
        size_t argc = tokens.size();
        std::string path = (argc == 1) ? "~" : tokens[1];

        if (argc > 2) {
            std::cerr << "cd: too many arguments" << std::endl;
        } else {
            const char* dir = (path == "~") ? getenv("HOME") : path.c_str();
            int status = chdir(dir);
            if (status != 0) {
                std::string msg = "failed to change directory";
                if (errno == ENOENT)
                    msg = "no such file or directory";
                else if (errno == EACCES)
                    msg = "permission denied";
                std::cerr << "cd: " << msg << ": " << path << std::endl;
            }
        }
        return true;
    }
    else if (tokens[0] == "history") {
        printHistory();
        return true;
    }

    return false;
}
