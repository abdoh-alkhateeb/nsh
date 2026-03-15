#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>

void Executer::execute(const std::vector<std::string> &tokens)
{
    if (Builtins::handle(tokens))
        return;

    std::vector<const char *> argv;
    bool shouldRedirect = false;
    bool shouldBackground = false;
    for (const std::string &token : tokens){
        if(token == ">"){
           shouldRedirect = true;
            break;
            
        }
        else if  (token == "&"){
            shouldBackground = true;
            break;
        }
    
        argv.push_back(token.c_str());
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    
     if (pid == 0)
    {
        if (shouldRedirect){
    int fd = open("samerfiles.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); 
    dup2(fd, STDOUT_FILENO);
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
     else{
         if(!shouldBackground){
        waitpid(pid, nullptr, 0);
     }
}
