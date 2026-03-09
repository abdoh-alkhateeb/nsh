#include "builtins.hpp"
#include "unistd.h"
#include "stdlib.h" // idk why using stdlib.h which is for c instead of c++ cstdlib, will ignore for now.
#include <iostream>

bool Builtins::handleCd(const std::vector<std::string> &tokens) {
    size_t argc = tokens.size();
    std::string path = argc == 1 ? "~" : tokens[1];
    if (argc > 2)
        std::cerr << "cd: too many arguments" << std::endl;
    else {
        int status = chdir(path == "~" ? getenv("HOME") : path.c_str());
        if (status != 0) {
            std::string msg = "failed to change directory";

            if (errno == ENOENT) msg = "no such file or directory";
            else if (errno == EACCES) msg = "permission denied";

            std::cerr << "cd: " << msg << ": " << path << std::endl;
        }
    }
    return true;
}

bool Builtins::handle(const std::vector<std::string> &tokens) {
    const std::string& mainCommand = tokens[0];

    if (mainCommand == "exit") exit(EXIT_SUCCESS);
    if (mainCommand == "cd") handleCd(tokens);

    return false;
}
