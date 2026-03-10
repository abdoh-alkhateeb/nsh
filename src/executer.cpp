#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include "fcntl.h"
#include <iostream>
#include <vector>

void Executer::execute(std::vector<std::string> tokens)
{
    if (tokens.empty())
        return;

    if (Builtins::handle(tokens))
        return;

    bool background = false;

    if (tokens.back() == "&")
    {
        background = true;
        tokens.pop_back();
    }

    bool redirect = false;
    std::string outfile;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == ">" && i + 1 < tokens.size())
        {
            redirect = true;
            outfile = tokens[i + 1];

            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            break;
        }
    }

    std::vector<const char *> argv;

    for (const std::string &token : tokens)
        argv.push_back(token.c_str());

    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0)
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;

    else if (pid == 0)
    {
        if (redirect)
        {
            int fd = open(outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0)
            {
                perror("open");
                exit(1);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(argv[0], const_cast<char *const *>(argv.data()));

        std::string msg = "failed to execute command";

        if (errno == ENOENT)
            msg = "command not found";

        std::cerr << tokens[0] << ": " << msg << std::endl;
        exit(1);
    }

    else
    {
        if (!background)
            waitpid(pid, nullptr, 0);
        else
            std::cout << "[background pid " << pid << "]" << std::endl;
    }
}