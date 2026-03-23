#include "executer.hpp"
#include "builtins.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (tokens.empty())
        return;

    if (Builtins::handle(tokens))
        return;

    std::vector<std::string> args = tokens;

    bool background = false;

    if (args.back() == "&")
    {
        background = true;
        args.pop_back();
    }

    int redirectIndex = -1;

    for (size_t i = 0; i < args.size(); i++)
    {
        if (args[i] == ">")
        {
            redirectIndex = i;
            break;
        }
    }

    int fd = -1;

    if (redirectIndex != -1)
    {
        std::string filename = args[redirectIndex + 1];

        fd = open(filename.c_str(),
                  O_WRONLY | O_CREAT | O_TRUNC,
                  0644);

        args.resize(redirectIndex);
    }

    std::vector<const char *> argv;

    for (const std::string &s : args)
        argv.push_back(s.c_str());

    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid == 0)
    {
        if (fd != -1)
        {
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(argv[0],
               const_cast<char *const *>(argv.data()));

        std::cerr << "command failed\n";
    }
    else
    {
        if (!background)
            waitpid(pid, nullptr, 0);
    }
}
