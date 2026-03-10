#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<std::string> commandTokens = tokens;
    bool background = false;

    if (!commandTokens.empty() && commandTokens.back() == "&")
    {
        background = true;
        commandTokens.pop_back();
    }

    std::vector<const char *> argv;

    for (const std::string &token : commandTokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
       std::cerr << commandTokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0)
        {
            std::string msg = "failed to execute command";

            if (errno == ENOENT)
                msg = "command not found";

           std::cerr << commandTokens[0] << ": " << msg << std::endl;
        }
    }
    else // parent process (pid > 0)
    {
        if (!background)
            waitpid(pid, nullptr, 0);
    }
}
