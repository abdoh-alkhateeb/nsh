#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <fcntl.h>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;
    std::string outfile;
    bool background = false;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == ">")
        {
            outfile = tokens[i + 1];
            break;
        }
        if (tokens[i] == "&")
        {
            background = true;
            break;
        }

        argv.push_back(tokens[i].c_str());
    }

    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        if (!outfile.empty())
        {
            int fd = open(outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(argv[0], const_cast<char *const *>(argv.data()));
        std::cerr << "command not found\n";
    }
    else
    {
        if (!background)
        {
            waitpid(pid, nullptr, 0);
        }
        else
        {
            std::cout << "Process running in background. PID: " << pid << std::endl;
        }
    }
}
