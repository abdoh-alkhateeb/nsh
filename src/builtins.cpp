#include "builtins.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <errno.h>

std::vector<std::string> Builtins::history;

bool Builtins::handle(const std::vector<std::string> &tokens)
{
    if (tokens.empty())
        return false;

    if (tokens[0] == "exit")
    {
        exit(EXIT_SUCCESS);
    }
    else if (tokens[0] == "cd")
    {
        size_t argc = tokens.size();
        std::string path = argc == 1 ? "~" : tokens[1];

        if (argc > 2)
        {
            std::cerr << "cd: too many arguments" << std::endl;
        }
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
        for (size_t i = 0; i < history.size(); i++)
        {
            std::cout << i + 1 << "  " << history[i] << std::endl;
        }

        return true;
    }

    return false;
}

void Builtins::add_history(const std::vector<std::string> &tokens)
{
    std::stringstream command;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        command << tokens[i];

        if (i != tokens.size() - 1)
            command << " ";
    }

    history.push_back(command.str());
}