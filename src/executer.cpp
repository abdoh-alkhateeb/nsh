#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    int redirectIndex = -1;

    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == ">")
        {
            redirectIndex = i;
            break;
        }
    }

    std::string outputFile = "";

    if (redirectIndex != -1)
    {
        if (redirectIndex + 1 >= tokens.size())
        {
            std::cerr << "No output file provided" << std::endl;
            return;
        }

        outputFile = tokens[redirectIndex + 1];
    }

    bool background = false;

    if (!tokens.empty() && tokens.back() == "&")
        background = true;

    std::vector<const char *> argv;

    int limit = tokens.size();

    if (redirectIndex != -1)
        limit = redirectIndex;
    else if (background)
        limit--;

    for (int i = 0; i < limit; i++)
        argv.push_back(tokens[i].c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        if (outputFile != "")
        {
            int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0)
            {
                std::cerr << "Failed to open file" << std::endl;
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
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
    {
        if (!background)
            waitpid(pid, nullptr, 0);
    }
}
