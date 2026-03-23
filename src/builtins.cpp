#include "builtins.hpp"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>

static std::vector<std::string> history;

bool Builtins::handle(const std::vector<std::string> &tokens)
{
    if (tokens.empty())
        return true;

    history.push_back(tokens[0]);

    if (tokens[0] == "exit")
        exit(EXIT_SUCCESS);

    else if (tokens[0] == "history")
    {
        for (size_t i = 0; i < history.size(); i++)
            std::cout << i + 1 << " "
                      << history[i] << std::endl;

        return true;
    }

    else if (tokens[0] == "cd")
    {
        std::string path =
            tokens.size() == 1 ? getenv("HOME")
                               : tokens[1];

        if (chdir(path.c_str()) != 0)
            std::cerr << "cd failed\n";

        return true;
    }

    return false;
}
