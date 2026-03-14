#include "executer.hpp"
#include "builtins.hpp"
#include "unistd.h"
#include "sys/wait.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <fcntl.h>

void Executer::execute(const std::vector<std::string> &tokens,const std::vector<std::string> &h)
{
    
    if (Builtins::handle(tokens,h))
        return;

    std::vector<const char *> argv;

    for (int i = 0; i < tokens.size(); i++){
    if(tokens[i]==">" || tokens[i]=="&"){
        break;
    }
    argv.push_back(tokens[i].c_str());
    }
    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) // fork failed
        std::cerr << tokens[0] << ": failed to execute command" << std::endl;
    else if (pid == 0) // child process
    {
        for(int i=0; i<tokens.size();i++){
            if(tokens[i]==">"){
            std::string filename = tokens[i+1];
            int FID =open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(FID,STDOUT_FILENO);
            close(FID);
            }
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
    else{ // parent process (pid > 0)
        bool found = false;
        for(int i=0; i<tokens.size();i++){
            if(tokens[i]=="&"){
                found=true;
            }
        } 
        if(!found){
        waitpid(pid, nullptr, 0);
}
}
}

