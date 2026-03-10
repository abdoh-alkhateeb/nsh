#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <cstring>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;

    for (const std::string &token : tokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        int redirectIndex = -1;

        for (size_t i = 0; i < tokens.size(); i++)
        {
            if (tokens[i] == ">")
            {
                redirectIndex = i;
                break;
            }
        }

        if (redirectIndex != -1 && redirectIndex + 1 < tokens.size())
        {
            int fd = open(tokens[redirectIndex + 1].c_str(),
                          O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0)
            {
                std::cerr << "failed to open file" << std::endl;
                exit(1);
            }

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
    else // parent process (pid > 0)
        waitpid(pid, nullptr, 0);
}
