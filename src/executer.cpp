#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <fcntl.h>  // for open, O_*, mode constants
#include <stdio.h>  // for printf, perror

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;


        
    std::vector<const char *> argv;

    for (const std::string &token : tokens)
        argv.push_back(token.c_str());
    argv.push_back(nullptr);
    
    bool Bg = false;
    if (tokens[tokens.size() - 1] == "&")
    {
        Bg = true;
        argv.pop_back(); // remove nullptr
        argv.pop_back(); // remove "&"
        argv.push_back(nullptr); // re-add terminator
    }
    pid_t pid = fork();

    if (pid == 0)
    {

        if(tokens[tokens.size() - 2] == ">"){
            int file = open(tokens[tokens.size() - 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (file < 0) {
                perror("open failed");
                return;
            }
            if (dup2(file, STDOUT_FILENO) < 0) {
                perror("dup2 failed");
                return;
            }
            close(file);
            argv.pop_back(); // remove nullptr
            argv.pop_back(); // remove filename
            argv.pop_back(); // remove ">"
            argv.push_back(nullptr); // re-add terminator
                
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
    else if (pid == -1)
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else
        if (!Bg){
            waitpid(pid, nullptr, 0);
        }
}

