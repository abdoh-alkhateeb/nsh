#include "builtins.hpp"
#include "unistd.h"
#include "stdlib.h"
#include <iostream>

// Useful for history function.
std::vector<std::string> inputs;


bool Builtins::handle(const std::vector<std::string> &tokens)
{
    // To be used when history function is used
    std::string whole_command;
    for (const std::string& token : tokens) {
        whole_command += token;
    }
    inputs.push_back(whole_command);


    if (tokens[0] == "exit")
        exit(EXIT_SUCCESS);
    else if (tokens[0] == "cd")
    {
        // argc stores the number of words input by the user.
        size_t argc = tokens.size();

        // if the user inputs no argument after cd, then the user is redirected to their home directory.
        std::string path = argc == 1 ? "~" : tokens[1];

        if (argc > 2)
            // Cerr is an output stream like cout, it's used for outputting errors.
            std::cerr << "cd: too many arguments" << std::endl;
        else
        {  
            // Changes the directory to the desired path
            int status = chdir(path == "~" ? getenv("HOME") : path.c_str()); 

            // Something went wrong... Seems likie chdir returns a value when an error takes place.
            if (status != 0)
            {
                std::string msg = "failed to change directory";
                // Errno is a "variable" (not really) the library unistd.h manipulates once something goes wrong.
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
        int i = 0;
        for (std::string& command : inputs) {
            std::cout << i++ << " " << command << std::endl;
        }
        return true;
    }

    return false;
}
