#include "builtins.hpp"
#include "unistd.h"
#include <shellMessage.hpp>
#include "stdlib.h" // idk why using stdlib.h which is for c instead of c++ cstdlib, will ignore for now.
#include <iostream>

shellMessage Builtins::handleCd(const std::vector<std::string> &tokens) {
    shellMessage msg; // will store all output

    size_t argc = tokens.size();
    std::string path = argc == 1 ? "~" : tokens[1];
    if (argc > 2)
        msg.addStderr("cd: too many arguments\n");
    else {
        int status = chdir(path == "~" ? getenv("HOME") : path.c_str());
        if (status != 0) {
            std::string errMsg = "failed to change directory";

            if (errno == ENOENT) errMsg = "no such file or directory";
            else if (errno == EACCES) errMsg = "permission denied";

            msg.addStderr("cd: " + errMsg + ": " + path + "\n");
        }
    }
    return msg;
}

shellMessage Builtins::handle(const std::vector<std::string> &tokens) {
    const std::string& mainCommand = tokens[0];

    if (mainCommand == "exit") exit(EXIT_SUCCESS);
    if (mainCommand == "cd") return handleCd(tokens);

    return shellMessage(); // return empty shellMessage if no output
}
