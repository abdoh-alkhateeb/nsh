#include "builtins.hpp"
#include "unistd.h"
#include "stdlib.h"
#include <iostream>
#include <errno.h>
#include <string.h>
#include <cstring>
#include <vector>
#include <readline/history.h>

static std::vector<std::string> command_history;
static const size_t MAX_HISTORY = 1000;

void Builtins::addToHistory(const std::string& cmd) {

    if (cmd.empty()) return;

    if (!command_history.empty() && command_history.back() == cmd) {
        return;
    }

    command_history.push_back(cmd);

    if (command_history.size() > MAX_HISTORY) {
        command_history.erase(command_history.begin());
    }

    add_history(cmd.c_str());
}

bool Builtins::handle(const std::vector<std::string> &tokens)
{
    if (tokens[0] == "exit")
        exit(EXIT_SUCCESS);
    else if (tokens[0] == "cd")
    {
        size_t argc = tokens.size();
        std::string path = argc == 1 ? "~" : tokens[1];

        if (argc > 2)
            std::cerr << "cd: too many arguments" << std::endl;
        else
        {
            int status = chdir(path == "~" ? getenv("HOME") : path.c_str());

            if (status != 0)
            {
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

    else if (tokens[0] == "history")
    {
        if (command_history.empty()) {
            std::cout << "No commands in history" << std::endl;
            return true;
        }

        for (size_t i = 0; i < command_history.size(); ++i) {
            std::cout << (i + 1) << "  " << command_history[i] << std::endl;
        }
        return true;
    }

    return false;
}
