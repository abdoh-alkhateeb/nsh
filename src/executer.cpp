#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>


void Executer::execute(const std::vector<std::string> &tokens)
{
    if (tokens.empty())
        return;

    Builtins::add_history(tokens);

    bool background = false;
    std::string outfile;
    std::vector<const char *> argv;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "&" && i == tokens.size() - 1)
        {
            background = true;
            break;
        }

        if (tokens[i] == ">")
        {
            if (i + 1 >= tokens.size())
            {
                std::cerr << "syntax error: expected file after >" << std::endl;
                return;
            }

            outfile = tokens[i + 1];
            break;
        }

        argv.push_back(tokens[i].c_str());
    }

    argv.push_back(nullptr);

    if (argv.empty())
        return;

    if (!background && Builtins::handle(tokens))
        return;

    pid_t pid = fork();

    if (pid == 0)
    {
        if (!outfile.empty())
        {
            int fd = open(outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0)
            {
                std::cerr << "failed to open file: " << outfile << std::endl;
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(argv[0], const_cast<char *const *>(argv.data()));

        std::string msg = "failed to execute command";

        if (errno == ENOENT)
            msg = "command not found";

        std::cerr << argv[0] << ": " << msg << std::endl;

        exit(EXIT_FAILURE);
    }
    else if (pid == -1)
    {
        std::cerr << "failed to execute command" << std::endl;
    }
    else
    {
        if (!background)
        {
            waitpid(pid, nullptr, 0);
        }
        else
        {
            std::cout << "[process running in background pid=" << pid << "]" << std::endl;
        }
    }
}