#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <fcntl.h>
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (tokens.empty())
        return;

    if (Builtins::handle(tokens))
        return;

    // analyze tokens for redirection and background
    bool background = false;
    std::string outFile;
    std::vector<std::string> cmdTokens;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i] == ">")
        {
            if (i + 1 < tokens.size())
                outFile = tokens[i + 1];
            break;
        }
        if (tokens[i] == "&" && i == tokens.size() - 1)
        {
            background = true;
            break;
        }
        cmdTokens.push_back(tokens[i]);
    }

    if (cmdTokens.empty())
        return;

    std::vector<const char *> argv;
    for (const std::string &token : cmdTokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
    {
        std::cerr << cmdTokens[0] << ": failed to execute command" << std::endl;
        return;
    }
    else if (pid == 0) // child process
    {
        // handle output redirection
        if (!outFile.empty())
        {
            int fd = open(outFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                std::cerr << "redirect: unable to open file " << outFile << std::endl;
                // fall through and attempt exec anyway
            }
            else
            {
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
        }

        int status = execvp(argv[0], const_cast<char *const *>(argv.data()));

        if (status != 0)
        {
            std::string msg = "failed to execute command";

            if (errno == ENOENT)
                msg = "command not found";

            std::cerr << cmdTokens[0] << ": " << msg << std::endl;
        }
        exit(EXIT_FAILURE);
    }
    else // parent process
    {
        if (!background)
            waitpid(pid, nullptr, 0);
        // if background, do not wait
    }
}
