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
    if (Builtins::handle(tokens, {}))
        return;

    std::vector<const char *> argv;

    for (const std::string &token : tokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

	if (pid == 0)
{
    for (int i = 0; i < (int)tokens.size(); i++)
    {
        if (tokens[i] == ">")
        {
            int fd = open(tokens[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
            break;
        }
    }

    std::vector<const char *> clean_argv;
    for (int i = 0; i < (int)tokens.size(); i++)
    {
        if (tokens[i] == ">" || tokens[i] == "&") break;
        clean_argv.push_back(tokens[i].c_str());
    }
    clean_argv.push_back(nullptr);

    int status = execvp(clean_argv[0], const_cast<char *const *>(clean_argv.data()));
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
    bool background = false;
    for (const std::string &token : tokens)
        if (token == "&")
            background = true;

    if (!background)
        waitpid(pid, nullptr, 0);
}
}
	
