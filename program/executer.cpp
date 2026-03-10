#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include <fcntl.h>
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <string>

static std::vector<std::string> history;


void Executer::execute(const std::vector<std::string> &tokens)
{
   
std::string command;

for (const auto &t : tokens)
    command += t + " ";

history.push_back(command);

if (tokens[0] == "history")
{
    for (size_t i = 0; i < history.size(); i++)
        std::cout << i + 1 << " " << history[i] << std::endl;

    return;
}

	 if (Builtins::handle(tokens))
        	return;

std::vector<const char*> argv;
std::string outputFile;
bool redirect = false;
bool background = false;

    for (size_t i = 0; i < tokens.size(); ++i)
{
    if (tokens[i] == ">")
    {
        redirect = true;

        if (i + 1 < tokens.size())
            outputFile = tokens[i + 1];
        else
            std::cerr << "Syntax error: no file after >" << std::endl;

        break;
    }
    else if (tokens[i] == "&")
    {
        background = true;
        break;
    }
    else
    {
        argv.push_back(tokens[i].c_str());
    }
}

argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        if (redirect)
{
    int fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0)
    {
        std::cerr << "failed to open file: " << outputFile << std::endl;
        exit(EXIT_FAILURE);
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
    else
{
    if (!background)
        waitpid(pid, nullptr, 0);
}
}
