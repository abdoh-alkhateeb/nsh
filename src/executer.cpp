#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fcntl.h>   
#include <string.h>  

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<std::string> args;
    std::string outfile = "";
    bool background = false;

    
    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == ">")
        {
            if (i + 1 < tokens.size())
                outfile = tokens[i + 1];
            i++; 
        }
        else if (tokens[i] == "&")
            background = true;
        else
            args.push_back(tokens[i]);
    }

    std::vector<const char *> argv;
    for (const std::string &token : args)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) 
        std::cerr << args[0] << ": failed to execute command" << std::endl;
    else if (pid == 0)
    {
        
        if (!outfile.empty())
        {
            int fd = open(outfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                std::cerr << "failed to open file: " << outfile << std::endl;
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
            std::cerr << args[0] << ": " << msg << std::endl;
        }
        exit(1);
    }
    else 
    {
        if (background)
            std::cout << "[Background PID: " << pid << "]" << std::endl;
        else
            waitpid(pid, nullptr, 0); 
    }
}