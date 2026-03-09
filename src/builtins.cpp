#include "builtins.hpp"
#include "unistd.h"
#include "stdlib.h"
#include <iostream>
#include <string>
#include <vector>

bool Builtins::handle(const std::vector<std::string> &tokens)
{   
    std::vector<std::string> commands;
    commands.push_back(tokens[0]);

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
    } else if (tokens[0] == "history"){
        for(int i=0; i<commands.size(); i++){
            std::cout << commands[i] << endl;
        }
    }

    return false;
}
