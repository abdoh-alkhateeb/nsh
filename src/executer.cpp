#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <fcntl.h>
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    std::vector<std::string> args = tokens;
    int status;
    bool existA = false;
    if (Builtins::handle(tokens))
        return;

    if (tokens.back() == "&")
    {
        existA = true;
        args.pop_back();
    }
    std::vector<const char *> argv;

    for (const std::string &token : args)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);
    pid_t pid = fork();
    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        bool exist = false;
        int index;
        for (int i = 0; i < tokens.size(); i++)
        {
            if (tokens[i] == ">")
            {
                exist = true;
                index = i;
                break;
            }
        }
        if (exist)
        {
            std::string myfile = tokens[index + 1];
            std::vector<const char *> real_argv;
            for (int i = 0; i < index; i++)
                real_argv.push_back(tokens[i].c_str());
            real_argv.push_back(nullptr);
            int fileDigit = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fileDigit, STDOUT_FILENO);
            close(fileDigit);
            status = execvp(real_argv[0], const_cast<char *const *>(real_argv.data()));
        }
        else
        {
            status = execvp(argv[0], const_cast<char *const *>(argv.data()));
        }
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
        if (!existA)
        {
            waitpid(pid, nullptr, 0);
        }
    }
}
