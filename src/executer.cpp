#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <string>
#include "fcntl.h"
#include <cerrno>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<std::string> args = tokens;

    int redirectIndex = -1;

    for (int i = 0; i < args.size(); i++)
    {
        if (args[i] == ">")
        {
            redirectIndex = i;
            break;
        }
    }

    bool background = false;

    if (!args.empty() && args.back() == "&")
    {
        background = true;
        args.pop_back();
    }

    std::vector<const char *> argv;

    for (const std::string &arg : args)
        argv.push_back(arg.c_str());

    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0)
    {
        if (redirectIndex != -1)
        {
            int fd = open(args[redirectIndex + 1].c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);

            dup2(fd, STDOUT_FILENO);
            close(fd);

            argv[redirectIndex] = nullptr;
        }

        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0)
        {
            std::string msg = "failed to execute command";

            if (errno == ENOENT)
                msg = "command not found";

            std::cerr << tokens[0] << ": " << msg << std::endl;
        }
    }
    else if (pid == -1)
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else
    {
        if (!background)
            waitpid(pid, nullptr, 0);
    }
}
